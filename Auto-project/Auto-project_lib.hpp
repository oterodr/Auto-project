#include <iostream>
#include "stdafx.h"
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

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


int open_an_image_and_show(string directory, string name);
// Opencv example code to open an image and show it in the screen.