#include "TP_Link.h"

TP_Link::TP_Link()
{
}

TP_Link::~TP_Link()
{
}

void TP_Link::fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets)
{
	string http_query = path_to_wget+"wget --tries=1 --timeout=10 \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/operator/param?action=list&group=PTZ.PresetPos\" -O Config_presets_" + address_IP + ".txt" + quiet_wget;
	system(http_query.c_str());
	
	ifstream file_config_presets;
	string filename_config_presets="Config_presets_" + address_IP + ".txt";
	file_config_presets.open(filename_config_presets.c_str(), ios::in);

	if (file_config_presets.good()==true)
	{
		if (is_file_empty(file_config_presets) == true)
		{
			string perror= "Can't connect with " + address_IP + ". Check your Internet connection or data in \"Config_login.txt\"!\n";
			file_config_presets.close();
			if (remove(filename_config_presets.c_str()) != 0)
			{
				perror = perror + filename_config_presets + "Error deleting file\n";
			}
			throw perror;
		}
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
	
	if (remove(filename_config_presets.c_str()) != 0)
	{
		string perror = filename_config_presets + "Error deleting file\n";
		throw perror;
	}

	}
	else
	{
		throw "Error! Can't download list of preset. Check if you have wget installed or \"WGET_DIR\" environment variable!\n set properly!\n";
	}
	
}
void TP_Link::get_frame(string login, string password, string address_IP)
{
	string http_query = path_to_wget + "wget --tries=1 --timeout=10 \"http://" + login + ":" + password + "@" + address_IP + "/jpg/image.jpg\" -O " + get_filename(address_IP) + quiet_wget;
	system(http_query.c_str());
}

void TP_Link::set_position(string login, string password, string address_IP, unsigned int preset_number, vector <Preset> &vector_of_presets)
{
	string http_query = path_to_wget + "wget --tries=1 --timeout=10 \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/operator/ptzset?gotoserverpresetname=" + vector_of_presets[preset_number].get_preset_name() + "\" --spider" + quiet_wget;
	//cout << http_query << endl;
	system(http_query.c_str());
}