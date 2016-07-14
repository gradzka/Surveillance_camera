#pragma once
#include "Camera_model.h"

class DLink :
	public Camera_model
{
public:
	DLink();
	~DLink();
	virtual void fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets);
	virtual void get_frame(string login, string password, string address_IP);
	virtual void set_position(string login, string password, string address_IP, unsigned int preset_number, vector <Preset> &vector_of_presets);
private:
	bool check_connection(string login, string password, string address_IP);
	void help_fill_vector_of_presets(string &line_in_file, char *char_line_in_file, char *substring_of_char_line_in_file, vector <Preset> &vector_of_presets);
};

