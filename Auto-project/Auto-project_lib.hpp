#include <iostream>
#include "stdafx.h"
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <boost/filesystem.hpp>

using namespace std;
using namespace cv;
using namespace boost::filesystem;

class IndexDir{
	// Class that stores the interesting folders for the project.
private:
	string _project_folder;  // directory where the project is run. TODO: set using visual studio macros.
	string _dropbox_folder;  // directory for the project in dropbox.
	string _images_folder;  // directory for input images.
	string _output_folder;  // directory to save outputs.
public:
	void set_project_folder(string project_folder){ _project_folder = project_folder; };
	void set_dropbox_folder(string dropbox_folder){ _dropbox_folder = dropbox_folder; };
	void set_images_folder(string images_folder){ _images_folder = images_folder; };
	void set_output_folder(string output_folder){ _output_folder = output_folder; };

	string get_project_folder(){ return _project_folder; };
	string get_dropbox_folder(){ return _dropbox_folder; };
	string get_images_folder(){ return _images_folder; };
	string get_output_folder(){ return _output_folder; };
};

// Opencv example code to open an image and show it in the screen.
int open_an_image_and_show(string directory, string name);


/* Information regarding the images that will be detected and localized.*/
class MyImage
{
	path _image_folder;
	path _image_name;
	path _image_path;
	
public:
	Mat img;
	/* Constructor */
	MyImage(path folder);

	/* Load the Object image into the program.*/
	int loadImage(path filename);

	/* Get image original file name without extension. */
	string getImageName(){ return _image_name.stem().string(); };
};


/* Operations related to detection of the Object in the Scene.*/
class DetectionOperations
{
	path image_saved_name;
	int min_hessian;
	Mat H;
	vector<KeyPoint> keypoints_object, keypoints_scene;
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	double max_dist = 0;
	double min_dist = 100;
	vector< DMatch > good_matches;
	Mat img_keypoints_object; Mat img_keypoints_scene;
	Mat img_matches;
	vector<Point2f> object_v;
	vector<Point2f> scene_v;
	Point2f center;
	
public:
	/* Do surf detection between object and scene given objects. */
	Point2f doSurfDetection(MyImage object, MyImage scene, bool is_save = false, path output_folder = "");
};

/* Operations related to the localization of the Object in the Scene regarding the camera position.*/
class LocalizationOperations{};
