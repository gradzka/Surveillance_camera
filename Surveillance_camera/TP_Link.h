#pragma once
#include "Camera_model.h"

class TP_Link :
	public Camera_model
{
public:
	TP_Link();
	~TP_Link();
	virtual void fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets);
	virtual void get_frame(string login, string password, string address_IP);
	virtual void set_position(string login, string password, string address_IP, int preset_number, vector <Preset> &vector_of_presets);
};

