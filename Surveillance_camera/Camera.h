#pragma once
#include <fstream>
#include <iostream>
#include <list>
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
public:
	string ID;
	string address_IP;
	string login;
	string password;
	list <Preset> list_of_presets;
	Camera_model *return_camera_model_pointer(string model_login, string ID, string address_IP, string login, string password);
public:
	Camera_model *model;
	Camera(string ID, string address_IP, string login, string password, string model);
	~Camera();
	void create_folder();
	void delete_screenshots(int time_archiving);
};

