#include "Preset.h"


Preset::Preset()
{
}


Preset::~Preset()
{
}

void Preset::set_No(string No)
{
	this->No = No;
}
void Preset::set_preset_name(string preset_name)
{
	this->preset_name = preset_name;
}

string Preset::return_No()
{
	return No;
}
string Preset::return_preset_name()
{
	return preset_name;
}