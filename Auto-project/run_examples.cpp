
#include "stdafx.h"
#include "Auto-project_lib.hpp"
#include "SURF_robocv.hpp"
#include "homografia.hpp"

int main()
{
	// Create a class to save the input/output folders
	class IndexDir index_dir;

	index_dir.set_project_folder("C:/Project/code/Auto-project/Auto-project");
	index_dir.set_dropbox_folder("C:/Users/EOdR/Dropbox/Auto-project");
	index_dir.set_images_folder(index_dir.get_dropbox_folder() + "/resources/images/");

	//open_an_image_and_show(index_dir.get_images_folder(), "mandrill.jpg");

	//robocv_code(index_dir.get_images_folder(), "passport.jpg");
	homografia(index_dir.get_images_folder() + "passport03.jpg", index_dir.get_images_folder() + "passport.jpg");
}