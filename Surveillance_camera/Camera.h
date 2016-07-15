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
#include <windows.h>
#pragma warning (disable:4996)
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
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
	static unsigned int max_number_of_presets;

public:
	Camera(string ID, string address_IP, string login, string password, string model);
	~Camera();
	//access to private fields  
	string get_ID();
	string get_address_IP();
	string get_login();
	string get_password();
	vector <Preset> get_vector_of_presets();

	void create_folder();
	void delete_screenshots(unsigned int time_archiving);
	unsigned int get_number_of_presets();
	static unsigned int get_max_number_of_presets();

	void get_frame();
	void set_position(unsigned int preset_number);

private:
	Camera_model *get_camera_model_pointer(string model, string ID, string address_IP, string login, string password);
};
