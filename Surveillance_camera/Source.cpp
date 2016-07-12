#include <iostream>
#include <fstream>
#include <map>
#include "time.h"

#include "Camera.h"
#include "DLink.h"
#include "TP_Link.h"

using namespace std;

void fill_map_of_cameras(map <string, Camera_model*> &map_of_cameras, int argc, char *argv[]);
int read_config_archiving_intervals(int &time_archiving); //returns time that 

int main(int argc, char *argv[])
{
	map <string, Camera_model*> map_of_cameras; //map of pointers to Camera_model objects
	fill_map_of_cameras(map_of_cameras, argc, argv);

	map<string, Camera_model*>::iterator iterator;
	for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
	{
		iterator->second->create_folder();
	}

	//Read from "Config_archiving_interval.txt"
	int time_archiving = 0;
	int time_interval = 0;

	time_interval = read_config_archiving_intervals(time_archiving);
	//cout << "T_A: " << time_archiving << " T_I: " << time_interval << endl;

	//time to check if interval has passed
	time_t interval_start = 0;
	time_t interval_passed = 0;

	//main loop
	while (true)
	{
		for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
		{
			iterator->second->get_frame();
			iterator->second->delete_screenshots(time_archiving);
		}

		interval_start = time(0);
		interval_passed = 0;
		while (interval_passed < time_interval)
		{
			interval_passed = time(0) - interval_start;
		}
	}

	//Clear
	map_of_cameras.clear();
	return 0;
}

void fill_map_of_cameras(map <string, Camera_model*> &map_of_cameras, int argc, char *argv[])
{
	ifstream file_config_login;
	string rubbish_line = "";
	string ID = "";
	string address_IP = "";
	string login="";
	string password="";
	string model_login="";

	file_config_login.open("Config_login.txt", ios::in);
	
	if (file_config_login.is_open())
	{
		getline(file_config_login, rubbish_line); //omit additional line in Config_login.txt
		
		// the option with no parameters in program call
		if (argc <= 1)
		{
			while (!file_config_login.eof())
			{
				file_config_login >> ID >> address_IP >> login >> password >> model_login;
				Camera_model *camera_model;
				if (model_login == "TP-Link")
				{
					camera_model = new TP_Link(ID, address_IP, login, password, model_login);
				}
				else if (model_login == "DLink")
				{
					camera_model = new DLink(ID, address_IP, login, password, model_login);
				}
				else
				{
					perror("Undefined type in camera_model_union! Please define it in code!");
					exit(1);
				}
				camera_model->fill_list_of_presets();
				if (map_of_cameras.find(ID) != map_of_cameras.end())
				{
					string s_perror = "Error!In \"Config_login.txt\" are duplicated camera ID: " +ID;
					cout << s_perror << endl;
					exit(1);
				}
				map_of_cameras[ID]=camera_model;				
			}
		}
		// the option with argc parameters in program call
		else
		{
			int number_of_arguments = 0;
			for (number_of_arguments = 1; number_of_arguments < argc;)
			{
				file_config_login >> ID >> address_IP >> login >> password >> model_login;
				if (strcmp(ID.c_str(), argv[number_of_arguments]) == 0)
				{
					Camera_model *camera_model;
					if (model_login == "TP-Link")
					{
						camera_model = new TP_Link(ID, address_IP, login, password, model_login);
					}
					else if (model_login == "DLink")
					{
						camera_model = new DLink(ID, address_IP, login, password, model_login);
					}
					else
					{
						perror("Undefined type in camera_model! Please define it in code!");
						exit(1);
					}
					camera_model->fill_list_of_presets();
					if (map_of_cameras.find(ID) != map_of_cameras.end())
					{
						string s_perror = "Error!In \"Config_login.txt\" are duplicated camera ID: " + ID;
						cout << s_perror << endl;
						exit(1);
					}
					map_of_cameras[ID] = camera_model;
					number_of_arguments++;
					file_config_login.seekg(0, file_config_login.beg);
				}
			}
		}
	}
	else
	{
		perror("Error with \"Config_login.txt\"");
		exit(1);
	}
	file_config_login.close();
}
int read_config_archiving_intervals(int &time_archiving)
{
	//Config_archiving_interval
	ifstream file_config_archiving_interval;

	int time_interval = 0;

	//Open Config_archiving_interval
	file_config_archiving_interval.open("Config_archiving_interval.txt", ios::in);

	if (file_config_archiving_interval.good() == false)
	{
		perror("Error with \"Config_archiving_interval.txt\"");
		exit(1);
	}
	else 
	{
		string rubbish_line = "";
		string *interval = new string[5];
		string *archiving = new string[6];

		getline(file_config_archiving_interval, rubbish_line); //Omit additional line in Config_archiving_interval

		for (int i = 0; i < 4; i++)
		{
			file_config_archiving_interval >> interval[i];
			if (interval[0] == "a")
			{
				perror("Error in order in the \"file_config_archiving_interval.txt\"");
				exit(1);
			}
		}

		for (int i = 0; i < 5; i++)
		{
			file_config_archiving_interval >> archiving[i];
			if (archiving[0] == "i")
			{
				perror("Error in order in the \"file_config_archiving_interval.txt\"");
				exit(1);
			}
		}

		time_interval = atoi(interval[1].c_str()) + 60 * (atoi(interval[2].c_str())) + 3600 * atoi(interval[3].c_str()); //to seconds
		time_archiving = atoi(archiving[1].c_str()) + 60 * (atoi(archiving[2].c_str())) + 3600 * atoi(archiving[3].c_str()) + 86400 * atoi(archiving[4].c_str());

		delete[] interval;
		delete[] archiving;

		//Close Config_archiving_interval
		file_config_archiving_interval.close();
	}

	return time_interval;
}
