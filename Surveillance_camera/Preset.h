#pragma once
#include <string>

using namespace std;

class Preset
{
private:
	string No;
	string preset_name;
public:
	Preset();
	~Preset();
	void set_No(string No);
	void set_preset_name(string preset_name);
	string return_No();
	string return_preset_name();
};

