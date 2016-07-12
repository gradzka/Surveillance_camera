#pragma once
#include "Camera_model.h"

class TP_Link :
	public Camera_model
{
public:
	TP_Link(string ID, string address_IP, string login, string password, string model);
	~TP_Link();
	virtual void fill_list_of_presets();
	virtual void get_frame();
};

