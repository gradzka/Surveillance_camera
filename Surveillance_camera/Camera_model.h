#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <string.h>
#include <stdlib.h>
#include "time.h"

#include "Preset.h"

#pragma warning (disable:4996)
using namespace std;

class Camera_model
{
public:
	Camera_model();
	~Camera_model();
	string get_filename(string address_IP);
	virtual void fill_list_of_presets(string login, string password, string address_IP, list <Preset> &list_of_presets) = 0;
	virtual void get_frame(string login, string password, string address_IP) = 0;
};

