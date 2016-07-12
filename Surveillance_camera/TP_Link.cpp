#include "TP_Link.h"


TP_Link::TP_Link(string ID, string address_IP, string login, string password, string model, string access_name_for_stream) : Camera_model(ID, address_IP, login, password, model, access_name_for_stream)
{
}


TP_Link::~TP_Link()
{
}

void TP_Link::fill_list_of_presets()
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/operator/param?action=list&group=PTZ.PresetPos\" -O Config_presets_" + address_IP + ".txt 2> NUL";
	system(http_query.c_str());

	ifstream file_config_presets;
	file_config_presets.open("Config_presets_" + address_IP + ".txt", ios::in);

	if (file_config_presets.good())
	{
		string line_in_file("");
		char char_line_in_file[100];
		char *substring_of_char_line_in_file = NULL;

		Preset loaded_preset;

		while (!file_config_presets.eof())
		{
			getline(file_config_presets, line_in_file); //omit addiotional information from file before list of presets

			if (strstr(line_in_file.c_str(), "Label") != NULL)
			{
				strcpy(char_line_in_file, line_in_file.c_str());
				substring_of_char_line_in_file = strtok(char_line_in_file, "=");

				while (substring_of_char_line_in_file != NULL)
				{
					substring_of_char_line_in_file = strtok(NULL, ",");

					if (substring_of_char_line_in_file == NULL)
					{
						break;
					}
					else
					{
						loaded_preset.No = "";
						loaded_preset.preset_name = substring_of_char_line_in_file;
						list_of_presets.push_back(loaded_preset);
						//cout << substring_of_char_line_in_file << "\n";
					}
				}
			}
		}
	file_config_presets.close();

	}
	else
	{
		perror("Error! It is problem with Config_presets_IP.txt file");
		exit(1);
	}
	
}
void TP_Link::get_frame()
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/jpg/image.jpg\" -O " + get_filename() + " 2> NUL";
	system(http_query.c_str());
}