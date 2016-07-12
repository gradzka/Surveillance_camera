#pragma once
#include "Camera_model.h"
#include <iostream>
using namespace std;
class DLink :
	public Camera_model
{
public:
	DLink(string ID, string address_IP, string login, string password, string model);
	~DLink();
	void help_fill_list_of_presets(string &line_in_file, char *char_line_in_file, char *substring_of_char_line_in_file);
	virtual void fill_list_of_presets();
	virtual void get_frame();
};

