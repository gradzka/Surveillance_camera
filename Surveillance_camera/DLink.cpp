#include "DLink.h"


DLink::DLink(string ID, string address_IP, string login, string password, string model) : Camera_model(ID, address_IP, login, password, model)
{
}


DLink::~DLink()
{
}

void DLink::help_fill_list_of_presets(string &line_in_file, char *char_line_in_file, char *substring_of_char_line_in_file)
{
	Preset loaded_preset;
	bool No_or_preset_name = true; //if "true" then "No" else if "false" then "preset_name"

	strcpy(char_line_in_file, line_in_file.c_str());
	substring_of_char_line_in_file = strtok(char_line_in_file, "-");

	while (substring_of_char_line_in_file != NULL)
	{
		if (No_or_preset_name == true)
		{
			loaded_preset.No = substring_of_char_line_in_file;
			No_or_preset_name = false;
		}
		else
		{
			loaded_preset.preset_name = substring_of_char_line_in_file;
			No_or_preset_name = true;
			list_of_presets.push_back(loaded_preset);
		}
		//cout << substring_of_char_line_in_file << "\n";
		substring_of_char_line_in_file = strtok(NULL, "-");	
	}
}
void DLink::fill_list_of_presets()
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/ptz_dlink.js?2.1.25\" -O Config_presets_" + address_IP+ ".txt 2> NUL";
	system(http_query.c_str());

	ifstream file_config_presets;
	string filename_config_presets="Config_presets_" + address_IP + ".txt";
	file_config_presets.open(filename_config_presets.c_str(), ios::in);
	
	if (file_config_presets.good())
	{
		string line_in_file= "";
		char char_line_in_file[41];
		char *substring_of_char_line_in_file = NULL;

		while (!file_config_presets.eof())
		{
			getline(file_config_presets, line_in_file, ';'); //omit addiotional information from file before list of presets
			
			if (strstr(line_in_file.c_str(), "var l_presetNameAll") != NULL)
			{

				while (strstr(line_in_file.c_str(), ",\"--Preset List--") == NULL)
				{
					getline(file_config_presets, line_in_file, ';');		
					//cout << line_in_file << endl;
					if (strstr(line_in_file.c_str(), ",\"--Preset List--") != NULL)
					{
						line_in_file.erase(line_in_file.begin() + line_in_file.find_first_of("\""), line_in_file.end());
						help_fill_list_of_presets(line_in_file, char_line_in_file, substring_of_char_line_in_file);
						break;
					}
					help_fill_list_of_presets(line_in_file, char_line_in_file, substring_of_char_line_in_file);
				}

				file_config_presets.close();
				break;
			}
		}
	}
	else
	{
		perror("Error! It is problem with Config_presets_IP.txt file");
		exit(1);
	}
}
void DLink::get_frame()
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/dms\" -O " + get_filename() + " 2> NUL";
	//cout << http_query << endl;
	system(http_query.c_str());
}
