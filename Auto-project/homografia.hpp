// It requires boost compiled libraries.

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;
using namespace boost::filesystem;


int homografia(path image_scene, path image_object, bool is_save = false, path output_folder = "");
