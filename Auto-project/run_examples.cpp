
#include "stdafx.h"
#include "Auto-project_lib.hpp"
//#include "SURF_robocv.hpp"
//#include "homografia.hpp"

void exitProgram(int code)
{
	if (code == -1)
		cout << "Program aborted." << endl;
	else if(code == 0)
		cout << "Program end." << endl;

	system("pause"); //wait for a key press
	exit(0);
}

int main()
{
	// Create a class to save the input/output folders
	class IndexDir index_dir;

	index_dir.set_project_folder("C:/Project/code/Auto-project/Auto-project");
	index_dir.set_dropbox_folder("C:/Users/EOdR/Dropbox/Auto-project");
	index_dir.set_images_folder(index_dir.get_dropbox_folder() + "/resources/images");
	index_dir.set_output_folder("C:/Users/EODR/Dropbox/Auto-project/output");
	//open_an_image_and_show(index_dir.get_images_folder(), "mandrill.jpg");

	//robocv_code(index_dir.get_images_folder(), "/passport.jpg");
	//homografia(index_dir.get_images_folder() + "/euro06.jpg", index_dir.get_images_folder() + "/euro.jpg");
	//homografia(index_dir.get_images_folder() + "/passport01.jpg", index_dir.get_images_folder() + "/passport.jpg");
	//homografia(index_dir.get_images_folder() + "/windows15.jpg", index_dir.get_images_folder() + "/windows_closed.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/windows05.jpg", index_dir.get_images_folder() + "/windows_semi_closed.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/windows15.jpg", index_dir.get_images_folder() + "/windows_open.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/windows16.jpg", index_dir.get_images_folder() + "/windows_closed2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/windows05.jpg", index_dir.get_images_folder() + "/windows_semi_closed2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/windows15.jpg", index_dir.get_images_folder() + "/windows_open2.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/sport03.jpg", index_dir.get_images_folder() + "/sport5.png", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/playera03.jpg", index_dir.get_images_folder() + "/playera.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/billete01.jpg", index_dir.get_images_folder() + "/billete.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/billete02.jpg", index_dir.get_images_folder() + "/billete.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/billete03.jpg", index_dir.get_images_folder() + "/billete.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas01.jpg", index_dir.get_images_folder() + "/Jtrebol.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas02.jpg", index_dir.get_images_folder() + "/Jtrebol.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas04.jpg", index_dir.get_images_folder() + "/Jtrebol.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas05.jpg", index_dir.get_images_folder() + "/Jtrebol.jpg", true, index_dir.get_output_folder());
	////homografia(index_dir.get_images_folder() + "/cartas03.jpg", index_dir.get_images_folder() + "/Jtrebol.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas05.jpg", index_dir.get_images_folder() + "/Kpicas.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas05.jpg", index_dir.get_images_folder() + "/Qcorazones.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas06.jpg", index_dir.get_images_folder() + "/ASdiamantes.jpg", true, index_dir.get_output_folder());
	//homografia(index_dir.get_images_folder() + "/cartas07.jpg", index_dir.get_images_folder() + "/AStreboles.jpg", true, index_dir.get_output_folder());
	

	// Working with classes
	
	// Create an object and a scene. Load the images.
	MyImage object(index_dir.get_images_folder());
	MyImage scene(index_dir.get_images_folder());

	if (object.loadImage("Jtrebol.jpg") || scene.loadImage("cartas03.jpg"))
	{
		cout << "Error reading images." << endl;
		exitProgram(-1);
	}

	// Do the detection
	DetectionOperations detector;
	detector.doSurfDetection(object, scene);

	// Do the localization
	LocalizationOperations localizer;
	
}