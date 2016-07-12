#pragma once
#include "Camera_model.h"

class TP_Link :
	public Camera_model
{
public:
	TP_Link();
	~TP_Link();
	virtual void fill_list_of_presets(string login, string password, string address_IP,list <Preset> &list_of_presets);
	virtual void get_frame(string login, string password, string address_IP);
};

