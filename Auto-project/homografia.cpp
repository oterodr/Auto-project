#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <boost/filesystem.hpp>
#include "homografia.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;


int homografia(path image_scene, path image_object, bool is_save, path output_folder)
{
	path image_saved_name;

	Mat img_object = imread(image_object.string(), -1);
	Mat img_scene = imread(image_scene.string(), CV_LOAD_IMAGE_GRAYSCALE);

	if (img_object.data == NULL || img_scene.data == NULL)
	{
		cout << !img_scene.data + " error reading images" << endl;
		system("pause"); //wait for a key press
		return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	cout << "Step 1: Detect the keypoints using SURF Detector" << endl;
	int minHessian = 3000;		// higher for less points

	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);
	printf("-- number of keypoints scene: %d \n", keypoints_scene.size());
	printf("-- number of keypoints object: %d \n", keypoints_object.size());

	//-- Step 2: Calculate descriptors (feature vectors)
	cout << "Step 2: Calculate descriptors (feature vectors)" << endl;
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	printf("-- number of descriptors scene: %d \n", descriptors_scene.size());
	printf("-- number of descriptors object: %d \n", descriptors_object.size());

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cout << "Step 3: Matching descriptor vectors using FLANN matcher" << endl;
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	//-- Quick calculation of max and min distances between keypoints
	cout << "Quick calculation of max and min distances between keypoints" << endl;
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);
	
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	cout << "Draw only 'good' matches" << endl;
	std::vector< DMatch > good_matches;
	
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_keypoints_object; Mat img_keypoints_scene;
	drawKeypoints(img_object, keypoints_object, img_keypoints_object, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(img_scene, keypoints_scene, img_keypoints_scene, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	Mat img_matches;
	drawMatches(img_keypoints_object, keypoints_object, img_keypoints_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	printf("-- number of good matches: %d (%d)\n", good_matches.size(), matches.size());

	//-- Localize the object
	cout << "Localize the object" << endl;
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	cout << "Get the corners from object" << endl;
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	printf("-- Scene corners 0: %d \n", scene_corners[0]);
	printf("-- Scene corners 1: %d \n", scene_corners[1]);
	printf("-- Scene corners 2: %d \n", scene_corners[2]);
	printf("-- Scene corners 3: %d \n", scene_corners[3]);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	cout << "Draw lines between the corners" << endl;
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	cout << "Show detected matches" << endl;
	imshow("Good Matches & Object detection", img_matches);

	//-- Save the image if the boolean is_save is true

	if (is_save)
	{
		image_saved_name = output_folder.parent_path().string() + "/" + path(image_object.stem().string() + "__" + image_scene.stem().string() + ".jpg").string();
		imwrite(image_saved_name.string(), img_matches);
		cout << "Output image saved in: " + image_saved_name.string() << endl;
	}

	waitKey(0);
	return 0;
}

