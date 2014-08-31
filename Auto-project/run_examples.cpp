
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
	index_dir.set_output_folder("C:/Users/EODR/Dropbox/Auto-project/output/");
	//open_an_image_and_show(index_dir.get_images_folder(), "mandrill.jpg");

	//robocv_code(index_dir.get_images_folder(), "passport.jpg");
	//homografia(index_dir.get_images_folder() + "euro06.jpg", index_dir.get_images_folder() + "euro.jpg");
	//homografia(index_dir.get_images_folder() + "passport01.jpg", index_dir.get_images_folder() + "passport.jpg");
	//homografia(index_dir.get_images_folder() + "windows15.jpg", index_dir.get_images_folder() + "windows_closed.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "windows05.jpg", index_dir.get_images_folder() + "windows_semi_closed.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "windows15.jpg", index_dir.get_images_folder() + "windows_open.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "windows16.jpg", index_dir.get_images_folder() + "windows_closed2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "windows05.jpg", index_dir.get_images_folder() + "windows_semi_closed2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "windows15.jpg", index_dir.get_images_folder() + "windows_open2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "sport03.jpg", index_dir.get_images_folder() + "sport5.png", true, index_dir.get_output_folder());
	homografia(index_dir.get_images_folder() + "playera03.jpg", index_dir.get_images_folder() + "playera.jpg", true, index_dir.get_output_folder());
	
}