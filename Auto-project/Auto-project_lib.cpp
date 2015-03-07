
#include "stdafx.h"
#include "Auto-project_lib.hpp"


int open_an_image_and_show(string directory, string name)
{
	Mat img = imread(directory + name, CV_LOAD_IMAGE_UNCHANGED);  //read the image data in the file "MyPic.JPG" and store it in 'img'

	if (img.empty()) //check whether the image is loaded or not
	{
		cout << "Error : Image cannot be loaded..!!" << endl;
		system("pause"); //wait for a key press
		return -1;
	}

	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
	imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window

	waitKey(0); //wait infinite time for a keypress

	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"

	return 0;
}

MyImage::MyImage(path folder)
{
	_image_folder = folder;
}

int MyImage::loadImage(path filename)
{
	_image_name = filename;
	_image_path = _image_folder / _image_name;
	img = imread(_image_path.string(), -1);
	if (img.data == NULL)
	{
		cout << "-- ERROR: Image " << _image_name.string() << " could NOT be loaded." << endl;
		return -1;
	}
	else
	{
		cout << "-- Image " << _image_name.string() << " loaded." << endl;
		return 0;
	}
}

Point2f DetectionOperations::doSurfDetection(MyImage object, MyImage scene, bool is_save, path output_folder)
{
	//-- Check if images are valid
	if (!object.img.data || !scene.img.data)
	{
		cout << !scene.img.data + " error reading images" << endl;
		system("pause"); //wait for a key press
		return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	cout << "Step 1: Detect the keypoints using SURF Detector" << endl;
	
	min_hessian = 3000;  // higher for less points
	SurfFeatureDetector detector(min_hessian);

	detector.detect(object.img, keypoints_object);
	detector.detect(scene.img, keypoints_scene);
	printf("-- number of keypoints scene: %d \n", keypoints_scene.size());
	printf("-- number of keypoints object: %d \n", keypoints_object.size());

	//-- Step 2: Calculate descriptors (feature vectors)
	cout << "Step 2: Calculate descriptors (feature vectors)" << endl;

	extractor.compute(object.img, keypoints_object, descriptors_object);
	extractor.compute(scene.img, keypoints_scene, descriptors_scene);

	printf("-- number of descriptors scene: %d \n", descriptors_scene.size());
	printf("-- number of descriptors object: %d \n", descriptors_object.size());

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cout << "Step 3: Matching descriptor vectors using FLANN matcher" << endl;
	
	matcher.match(descriptors_object, descriptors_scene, matches);

	//-- Quick calculation of max and min distances between keypoints
	cout << "Quick calculation of max and min distances between keypoints" << endl;
	
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

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	drawKeypoints(object.img, keypoints_object, img_keypoints_object, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(scene.img, keypoints_scene, img_keypoints_scene, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	drawMatches(img_keypoints_object, keypoints_object, img_keypoints_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	printf("-- number of good matches: %d (%d)\n", good_matches.size(), matches.size());
	
	// Check the number of good matches, if more than 4 continue
	if (good_matches.size() < 5)
	{
		printf("-- Insuficient number of good matches\n");
		return -1;
	}

	//-- Localize the object
	cout << "Localize the object" << endl;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		object_v.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene_v.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	H = findHomography(object_v, scene_v, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	cout << "Get the corners from object" << endl;
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(object.img.cols, 0);
	obj_corners[2] = cvPoint(object.img.cols, object.img.rows); obj_corners[3] = cvPoint(0, object.img.rows);
	vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);
	printf("-- Scene corners 0: %f, %f \n", scene_corners[0].x, scene_corners[0].y);
	printf("-- Scene corners 1: %f, %f \n", scene_corners[1].x, scene_corners[1].y);
	printf("-- Scene corners 2: %f, %f \n", scene_corners[2].x, scene_corners[2].y);
	printf("-- Scene corners 3: %f, %f \n", scene_corners[3].x, scene_corners[3].y);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	cout << "Draw lines between the corners" << endl;
	line(img_matches, scene_corners[0] + Point2f(object.img.cols, 0), scene_corners[1] + Point2f(object.img.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(object.img.cols, 0), scene_corners[2] + Point2f(object.img.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(object.img.cols, 0), scene_corners[3] + Point2f(object.img.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(object.img.cols, 0), scene_corners[0] + Point2f(object.img.cols, 0), Scalar(0, 255, 0), 4);

	//-- Get center and draw it
	center.x = (scene_corners[0].x + scene_corners[1].x + scene_corners[2].x + scene_corners[3].x) / 4.;
	center.y = (scene_corners[0].y + scene_corners[1].y + scene_corners[2].y + scene_corners[3].y) / 4.;
	circle(img_matches, center + Point2f(object.img.cols, 0), 10, Scalar(0, 0, 255), 4);

	//-- Show detected matches
	cout << "Show detected matches" << endl;
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);

	//-- Save the image if the boolean is_save is true

	if (is_save)
	{
		image_saved_name = output_folder.parent_path().string() + "/" + path(object.getImageName() + "__" + scene.getImageName() + ".jpg").string();
		imwrite(image_saved_name.string(), img_matches);
		cout << "Output image saved in: " + image_saved_name.string() << endl;
	}

	//-- Return the center of the object found in the scene.
	return center;
}
