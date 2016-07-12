#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include "time.h"
#include <vector>

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
	string model;
	string access_name_for_stream;
public:
	Camera(string ID, string address_IP, string login, string password, string model, string access_name_for_stream);
	~Camera();
	void create_folder();
	string get_filename();
};

