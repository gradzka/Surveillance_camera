#pragma once
#include "Camera.h"

struct Preset
{
	string No;
	string preset_name;
};

class Camera_model :
	public Camera
{
public:
	list <Preset> list_of_presets;
public:
	Camera_model(string ID, string address_IP, string login, string password, string model, string access_name_for_stream);
	~Camera_model();
	virtual void fill_list_of_presets() = 0;
	virtual void get_frame() = 0;
};

