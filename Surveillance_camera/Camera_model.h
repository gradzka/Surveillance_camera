#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include "time.h"
#include "Preset.h"

#pragma warning (disable:4996)
using namespace std;

class Camera_model
{
public:
	Camera_model();
	~Camera_model();
	virtual void fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets) = 0;
	virtual void get_frame(string login, string password, string address_IP) = 0;
	virtual void set_position(string login, string password, string address_IP, unsigned int preset_number, vector <Preset> &vector_of_presets) = 0;
	
	#ifdef _WIN32
	static void find_path_to_wget();
	#endif
	static string path_to_wget;
protected:
	string get_filename(string address_IP);
	bool is_file_empty(ifstream &file);
	static string quiet_wget;

};

