#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <vector>

#include "Camera_model.h"
#include "DLink.h"
#include "TP_Link.h"
#include "Preset.h"

#ifdef _WIN32 //If this is Winodows system
#include "io.h"
#pragma warning (disable:4996)
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace std;

class Camera
{
private:
	string ID;
	string address_IP;
	string login;
	string password;
	vector <Preset> vector_of_presets;
	Camera_model *model;
	static int max_number_of_presets;
public:
	Camera(string ID, string address_IP, string login, string password, string model);
	~Camera();
	//access to private fields
	string return_ID();
	string return_address_IP();
	string return_login();
	string return_password();
	vector <Preset> return_vector_of_presets();
	Camera_model *return_model();

	void create_folder();
	void delete_screenshots(int time_archiving);
	int get_number_of_presets();
	static int return_max_number_of_presets();
private:
	Camera_model *return_camera_model_pointer(string model, string ID, string address_IP, string login, string password);
};

