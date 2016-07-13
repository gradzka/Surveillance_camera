#include "TP_Link.h"


TP_Link::TP_Link()
{
}

TP_Link::~TP_Link()
{
}

void TP_Link::fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets)
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/operator/param?action=list&group=PTZ.PresetPos\" -O Config_presets_" + address_IP + ".txt 2> NUL";
	system(http_query.c_str());

	ifstream file_config_presets;
	string filename_config_presets="Config_presets_" + address_IP + ".txt";
	file_config_presets.open(filename_config_presets.c_str(), ios::in);

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
						loaded_preset.set_No("");
						loaded_preset.set_preset_name(substring_of_char_line_in_file);
						vector_of_presets.push_back(loaded_preset);
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
void TP_Link::get_frame(string login, string password, string address_IP)
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/jpg/image.jpg\" -O " + get_filename(address_IP) + " 2> NUL";
	system(http_query.c_str());
}

void TP_Link::set_position(string login, string password, string address_IP, unsigned int preset_number, vector <Preset> &vector_of_presets)
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/operator/ptzset?gotoserverpresetname=" + vector_of_presets[preset_number].get_preset_name() + "\" --spider 2> NUL";
	//cout << http_query << endl;
	system(http_query.c_str());
}