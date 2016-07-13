#include <iostream>
#include <fstream>
#include <map>
#include "time.h"

#include "Camera.h"
#include "DLink.h"
#include "TP_Link.h"

using namespace std;

#define time_for_changing_position 5000

#ifdef _WIN32
void sleep(unsigned milliseconds)
{
	Sleep(milliseconds);
}
#else
void sleep(unsigned milliseconds)
{
	usleep(milliseconds * 1000); //takes microseconds
}
#endif


unsigned int Camera::max_number_of_presets = 0;
void fill_map_of_cameras(map <string, Camera *> &map_of_cameras, unsigned int argc, char *argv[]);
unsigned int read_config_archiving_intervals(unsigned int &time_archiving); //returns time that 

int main(unsigned int argc, char *argv[])
{
	map <string, Camera *> map_of_cameras; //map of pointers to Camera_model objects
	fill_map_of_cameras(map_of_cameras, argc, argv);

	map<string, Camera *>::iterator iterator;
	for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
	{
		iterator->second->create_folder();
	}

	//Read from "Config_archiving_interval.txt"
	unsigned int time_archiving = 0;
	unsigned int time_interval = 0;

	time_interval = read_config_archiving_intervals(time_archiving);
	//cout << "T_A: " << time_archiving << " T_I: " << time_interval << endl;

	//main loop
	while (true)
	{
		cout << "Surveillance cameras have started partol!" << endl;
		for (unsigned int preset_number = 0; preset_number < Camera::get_max_number_of_presets(); preset_number++)
		{
			for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
			{
				if (preset_number < (iterator->second->get_number_of_presets()))
				{
					iterator->second->set_position(preset_number);
				}
			}

			sleep(time_for_changing_position);

			for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
			{
				if (preset_number < (iterator->second->get_number_of_presets()))
				{
					iterator->second->get_frame();
					//cout << iterator->second->return_address_IP() << "\t" << preset_number << endl;
				}
			}

		}
		cout << "Patrol has already ended!" << endl;

		for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
		{
			iterator->second->delete_screenshots(time_archiving);
		}

		if (argc >= 1) //the option with argc parameters in program call, after one patrol program will be ended
		{
			break; 
		}

		sleep(time_interval * 1000);
	}

	//deleting Camera objects
	for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
	{
		delete iterator->second;
	}

	//Clear
	map_of_cameras.clear();

	cout << "Program has succesfully ended!" << endl;
	return 0;
}

void fill_map_of_cameras(map <string, Camera *> &map_of_cameras, unsigned int argc, char *argv[])
{
	ifstream file_config_login;
	string rubbish_line = "";
	string ID = "";
	string address_IP = "";
	string login="";
	string password="";
	string model="";

	file_config_login.open("Config_login.txt", ios::in);
	
	if (file_config_login.is_open())
	{
		getline(file_config_login, rubbish_line); //omit additional line in Config_login.txt
		
		// the option with no parameters in program call
		if (argc <= 1)
		{
			while (!file_config_login.eof())
			{
				file_config_login >> ID >> address_IP >> login >> password >> model;

				if (map_of_cameras.find(ID) != map_of_cameras.end())
				{
					string s_perror = "Error! Probably in \"Config_login.txt\" are duplicated camera ID: " + ID + " or you typed duplicated parameters!";
					cout << s_perror << endl;
					exit(1);
				}

				Camera *camera = new Camera(ID, address_IP, login, password, model);
				map_of_cameras[ID] = camera;

			}
		}

		// the option with argc parameters in program call
		else
		{
			unsigned int number_of_arguments = 0;

			for (number_of_arguments = 1; number_of_arguments < argc;)
			{
				file_config_login >> ID >> address_IP >> login >> password >> model;
				if (strcmp(ID.c_str(), argv[number_of_arguments]) == 0)
				{
					if (map_of_cameras.find(ID) != map_of_cameras.end())
					{
						string s_perror = "Error!In \"Config_login.txt\" are duplicated camera ID: " + ID;
						cout << s_perror << endl;
						exit(1);
					}

					Camera *camera = new Camera(ID, address_IP, login, password, model);
					map_of_cameras[ID] = camera;

					number_of_arguments++;

					file_config_login.seekg(0, file_config_login.beg);
				}

				else if (file_config_login.eof())
				{
					cout << "The ID: " << argv[number_of_arguments] << " isn't in \"Config_login.txt\"" << endl;
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

	if (map_of_cameras.empty() == true)
	{
		cout << "Error! The <map_of_cameras> is empty! Problably \"Config_login\" is not filled" << endl;
		exit(1);
	}

	file_config_login.close();
}
unsigned int read_config_archiving_intervals(unsigned int &time_archiving)
{
	//Config_archiving_interval
	ifstream file_config_archiving_interval;

	unsigned int time_interval = 0;

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

		for (unsigned int i = 0; i < 4; i++)
		{
			file_config_archiving_interval >> interval[i];

			if (interval[0] == "a")
			{
				perror("Error in order in the \"file_config_archiving_interval.txt\"");
				exit(1);
			}
		}

		for (unsigned int i = 0; i < 5; i++)
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
