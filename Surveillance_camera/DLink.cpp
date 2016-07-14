#include "DLink.h"

DLink::DLink()
{
}

DLink::~DLink()
{
}

void DLink::help_fill_vector_of_presets(string &line_in_file, char *char_line_in_file, char *substring_of_char_line_in_file, vector <Preset> &vector_of_presets)
{
	Preset loaded_preset;
	bool No_or_preset_name = true; //if "true" then "No" else if "false" then "preset_name"

	strcpy(char_line_in_file, line_in_file.c_str());
	substring_of_char_line_in_file = strtok(char_line_in_file, "-");

	while (substring_of_char_line_in_file != NULL)
	{
		if (No_or_preset_name == true)
		{
			loaded_preset.set_No(substring_of_char_line_in_file);
			No_or_preset_name = false;
		}
		else
		{
			loaded_preset.set_preset_name(substring_of_char_line_in_file);
			No_or_preset_name = true;
			vector_of_presets.push_back(loaded_preset);
		}
		//cout << substring_of_char_line_in_file << "\n";
		substring_of_char_line_in_file = strtok(NULL, "-");	
	}
}
bool DLink::check_connection(string login, string password, string address_IP)
{
	ifstream file_check;
	string http_query = "";

	//get one frame to check if you log in successfully - wget doesn't require login and password
	//if frame is empty login data is incorrect
	//this frame is only temporary, it must be deleted

	http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/dms\" -O TMP.jpg 2> NUL";
	system(http_query.c_str());

	file_check.open("TMP.jpg", ios::in);

	if (is_file_empty(file_check) == true)
	{
		
		file_check.close();
		if (remove("TMP.jpg") != 0)
		{
			cout << "TMP.jpg" << endl;
			perror("Error deleting file\n");
			exit(1);
		}
		
		return false;
	}
	else
	{
		file_check.close();
		if (remove("TMP.jpg") != 0)
		{
			cout << "TMP.jpg" << endl;
			perror("Error deleting file\n");
			exit(1);
		}

		return true;
	}
	
}
void DLink::fill_vector_of_presets(string login, string password, string address_IP, vector <Preset> &vector_of_presets)
{
	string http_query = "wget --tries=2 \"http://" + address_IP + "/ptz_dlink.js?2.1.25\" -O Config_presets_" + address_IP + ".txt 2> NUL";
	system(http_query.c_str());
	//
	if (check_connection(login, password, address_IP) == false)
	{
		cout << "\nCan't connect with " + address_IP + ". Check your Internet connection or IP address in \"Config_login.txt\"!" << endl;
		exit(1);
	}
	else
	{
		//
		ifstream file_config_presets;
		string filename_config_presets = "Config_presets_" + address_IP + ".txt";
		file_config_presets.open(filename_config_presets.c_str(), ios::in);

		if (file_config_presets.good())
		{
			if (is_file_empty(file_config_presets) == true)
			{
				cout << "\nCan't connect with " + address_IP + ". Check your Internet connection or IP address in \"Config_login.txt\"!" << endl;
				file_config_presets.close();
				if (remove(filename_config_presets.c_str()) != 0)
				{
					cout << filename_config_presets.c_str() << endl;
					perror("Error deleting file\n");
					exit(1);
				}
				exit(1);
			}

			string line_in_file = "";
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
							help_fill_vector_of_presets(line_in_file, char_line_in_file, substring_of_char_line_in_file, vector_of_presets);
							break;
						}
						help_fill_vector_of_presets(line_in_file, char_line_in_file, substring_of_char_line_in_file, vector_of_presets);
					}

					file_config_presets.close();

						if (remove(filename_config_presets.c_str()) != 0)
						{
						cout << filename_config_presets.c_str() << endl;
						perror("Error deleting file\n");
						exit(1);
						}
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
}
void DLink::get_frame(string login, string password, string address_IP)
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/dms\" -O " + get_filename(address_IP) + " 2> NUL";
	//cout << http_query << endl;
	system(http_query.c_str());
}

void DLink::set_position(string login, string password, string address_IP, unsigned int preset_number, vector <Preset> &vector_of_presets)
{
	string http_query = "wget \"http://" + login + ":" + password + "@" + address_IP + "/cgi-bin/longcctvpst.cgi?action=goto&name=" + vector_of_presets[preset_number].get_preset_name() + "&number=" + vector_of_presets[preset_number].get_No() + "\" --spider 2> NUL";
	//cout << http_query << endl;
	system(http_query.c_str());
}