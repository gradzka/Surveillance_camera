#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <thread>

#include "Camera.h"
#include "DLink.h"
#include "TP_Link.h"

using namespace std;

#define time_for_changing_position 5000

#ifdef _WIN32
string Camera_model::quiet_wget = " 2> NUL";
#else
string Camera_model::quiet_wget = " 2>/dev/null";
#endif

unsigned int Camera::max_number_of_presets = 0;

void fill_map_of_cameras(map <string, Camera *> &map_of_cameras, int argc, char *argv[]);
unsigned int read_config_archiving_intervals(unsigned int &time_archiving); //returns time that 
bool has_non_digit(const char *str);

int main(int argc, char *argv[])
{
	//Read from "Config_archiving_interval.txt"
	unsigned int time_archiving = 0;
	unsigned int time_interval = 0;

	try
	{
#ifdef _WIN32
		Camera_model::find_path_to_wget();
#endif
		time_interval = read_config_archiving_intervals(time_archiving);
		//cout << "T_A: " << time_archiving << " T_I: " << time_interval << endl;
	}
	catch (const char *perror)
	{
		cout << perror;
		cout << "Program has ended!" << endl;
		exit(1);
	}
	catch (string perror)
	{
		cout << perror;
		cout << "Program has ended!" << endl;
		exit(1);
	}
	catch (...)
	{
		cout << "Undefined error!\n";
		cout << "Program has ended!" << endl;
		exit(1);
	}

		map <string, Camera *> map_of_cameras; //map of pointers to Camera_model objects
		map<string, Camera *>::iterator iterator;
	try
	{
		fill_map_of_cameras(map_of_cameras, argc, argv);

		for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
		{
			iterator->second->create_folder();
		}

		std::chrono::milliseconds duration_for_changing_position(time_for_changing_position);
		std::chrono::milliseconds duration_for_time_interval(time_interval * 1000);

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

				std::this_thread::sleep_for(duration_for_changing_position);

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

			if (argc > 1) //the option with argc parameters in program call, after one patrol program will be ended
			{
				break;
			}

			std::this_thread::sleep_for(duration_for_time_interval);
		}

	}
	catch (const char *perror)
	{
		cout << perror;
	}
	catch (string perror)
	{
		cout << perror;
	}
	catch (...)
	{
		cout << "Undefined error!\n";
	}
	
	//deleting Camera objects
	for (iterator = map_of_cameras.begin(); iterator != map_of_cameras.end(); iterator++)
	{
		delete iterator->second;
	}

	//Clear
	map_of_cameras.clear();

	cout << "Program has ended!" << endl;
	return 0;
}

void fill_map_of_cameras(map <string, Camera *> &map_of_cameras, int argc, char *argv[])
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
					string perror = "Error! Probably in \"Config_login.txt\" are duplicated camera ID: " + ID + " or you typed duplicated parameters!\n";
					throw perror;
				}

				Camera *camera = new Camera(ID, address_IP, login, password, model);
				map_of_cameras[ID] = camera;

			}
		}

		// the option with argc parameters in program call
		else
		{
			int number_of_arguments = 0;

			for (number_of_arguments = 1; number_of_arguments < argc;)
			{
				file_config_login >> ID >> address_IP >> login >> password >> model;
				if (strcmp(ID.c_str(), argv[number_of_arguments]) == 0)
				{
					if (map_of_cameras.find(ID) != map_of_cameras.end())
					{
						string perror = "Error!In \"Config_login.txt\" are duplicated camera ID: " + ID + "\n";
						throw perror;
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
		throw "Error with \"Config_login.txt\"\n";
	}

	if (map_of_cameras.empty() == true)
	{
		throw "Error! The <map_of_cameras> is empty! Problably \"Config_login\" is not filled\n";
	}

	file_config_login.close();
}
bool has_non_digit(const char *str)
{
	if (strspn(str, "0123456789") == strlen(str)) return true;
	else
	{
		return false;
	}
	//if there is a digit or digits that "0123456789" doesn't include
	//strspn return the index of first appearance of incorrect digit

	//if everything is ok strspn return the length of str
	//so str[length(str)] is \0
}
void fill_interval_and_archiving_tables(ifstream &file_config_archiving_interval, string *interval_or_archiving)
{
	for (unsigned int i = 0; i <= 3; i++)
	{
		file_config_archiving_interval >> interval_or_archiving[i];
		if (has_non_digit(interval_or_archiving[i].c_str()) == false)
		{
			throw "Error in order in the \"file_config_archiving_interval.txt\". Incorrect digit found!\n";
		}
	}
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
		throw "Error with \"Config_archiving_interval.txt\"\n";
	}
	else 
	{
		string rubbish_line = "";
		string *interval = new string[4];
		string *archiving = new string[4];

		getline(file_config_archiving_interval, rubbish_line); //Omit additional line in Config_archiving_interval

		string i_or_a = "";

		file_config_archiving_interval >> i_or_a;
		if (i_or_a == "a")
		{
			fill_interval_and_archiving_tables(file_config_archiving_interval, archiving);
			file_config_archiving_interval >> i_or_a;
			if (i_or_a != "i") 
			{ 
				throw "Error in proper letter in \"file_config_archiving_interval.txt\".\n"; 
			}
			fill_interval_and_archiving_tables(file_config_archiving_interval, interval);
		}
		else if (i_or_a == "i")
		{
			fill_interval_and_archiving_tables(file_config_archiving_interval, interval);
			file_config_archiving_interval >> i_or_a;
			if (i_or_a != "a") 
			{ 
				throw "Error in proper letter in \"file_config_archiving_interval.txt\".\n"; 
			}
			fill_interval_and_archiving_tables(file_config_archiving_interval, archiving);
		}
		else
		{
			throw "Error in proper letter in \"file_config_archiving_interval.txt\".\n";
		}

		time_interval = atoi(interval[0].c_str()) + 60 * (atoi(interval[1].c_str())) + 3600 * atoi(interval[2].c_str()) + 86400 * atoi(interval[3].c_str()); //to seconds
		time_archiving = atoi(archiving[0].c_str()) + 60 * (atoi(archiving[1].c_str())) + 3600 * atoi(archiving[2].c_str()) + 86400 * atoi(archiving[3].c_str());

		delete[] interval;
		delete[] archiving;

		//Close Config_archiving_interval
		file_config_archiving_interval.close();
	}

	return time_interval;
}
