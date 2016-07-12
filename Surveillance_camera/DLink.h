#pragma once
#include "Camera_model.h"
#include <iostream>

using namespace std;

class DLink :
	public Camera_model
{
public:
	DLink();
	~DLink();
	virtual void fill_list_of_presets(string login, string password, string address_IP, list <Preset> &list_of_presets);
	virtual void get_frame(string login, string password, string address_IP);
private:
	void help_fill_list_of_presets(string &line_in_file, char *char_line_in_file, char *substring_of_char_line_in_file, list <Preset> &list_of_presets);
};

