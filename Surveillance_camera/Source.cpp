#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "time.h"

using namespace std;
using namespace cv;

//Sleep, creating folder for Windows and Linux
#ifdef _WIN32 //If this is Winodows system
#pragma warning (disable:4996)
void create_folder(string IP)
{
	string folder_name = "md Screenshots\\" + IP;
	system(folder_name.c_str());
}
#else //for Linux
void create_folder(string IP)
{
	string folder_name = "mkdir Screenshots/" + IP;
	system(folder_name.c_str());
}
#endif

char *current_time()
{
	char cur_time[20];
	time_t timer = time(0);   // get time now
	struct tm *now = localtime(&timer);
	strftime(cur_time, sizeof(cur_time), "%d-%m-%Y %H-%M-%S", now);

	return cur_time;
}
string get_filename(string IP)
{
	char buffer[20];
	strcpy(buffer, current_time());
	string filename;
	filename.assign(buffer, 19);
	filename = "Screenshots/" + IP + "/" + filename + ".PNG";
	return filename;
}
void your_data(string &login, string &password, string &IP)
{
	cout << "Enter camera IP:" << endl;
	cin >> IP;
	cout << "Enter your login:" << endl;
	cin >> login;
	cout << "Enter your password:" << endl;
	cin >> password;
}
void check_your_IP(string &IP, string &file_IP, string &login, string &password, ifstream &file_config_login)
{
	string rubbish_line;
	while (!(IP == file_IP)) //if IP! = file_IP then skip the line and get next file_IP
	{
		if (!file_config_login.eof()) //If there isn't the end of the file
		{
			getline(file_config_login, rubbish_line);
			file_config_login >> file_IP;
		}
		else
		{
			cout << "WARNING! You typed wrong IP adress!" << endl;

			file_config_login.close();
			file_config_login.open("Config_login.txt", ios::in);

			//Omit additional line in Config_file_login
			getline(file_config_login, rubbish_line);

			your_data(login, password, IP);
		}
	}
}
void check_your_data(ifstream &file_config_login, ifstream &file_config_camera, string &login, string &password, string &IP, bool &not_proper, string &video)
{
	//Data from Config_login
	string file_cl_IP = "";
	string file_cl_login = "";
	string file_cl_password = "";
	string file_cl_model = "";

	string rubbish_line = ""; //includes data from Config_login to skip

	//Omit additional line in Config_file_login
	getline(file_config_login, rubbish_line); //<address> <login> <password> <model>

	file_config_login >> file_cl_IP; //first IP from Config_login

	check_your_IP(IP, file_cl_IP, login, password, file_config_login); //Correct IP address or re-enter data

	//Download the remaining data from Config_login: login, password and model of the camera
	file_config_login >> file_cl_login;
	file_config_login >> file_cl_password;
	file_config_login >> file_cl_model;

	//Second Config_camera
	string access_name_for_stream = "";
	string file_cc_model;

	if (!file_config_camera.is_open())
	{
		file_config_camera.open("Config_camera.txt", ios::in);
	}

	getline(file_config_camera, rubbish_line); //skip first rubbish_line
	file_config_camera >> file_cc_model; //includes model of the camera
	file_config_camera >> access_name_for_stream;

	bool wrong_data = true; //check if you enter appropriate data; only used in while

	while (wrong_data == true)
	{
		if (login == file_cl_login && password == file_cl_password) //data you taped is the same in Config_login
		{
			not_proper = false;
			while (!(file_cc_model == file_cl_model)) //model in Config_login file is included in Config_camera file
			{
				if (!file_config_camera.eof())
				{
					file_config_camera >> file_cc_model;
					file_config_camera >> access_name_for_stream;
				}
				else
				{
					cout << "Lack of necessary information in the Config_camera!" << endl << "Possibility of erroneous data in the Config_login!" << endl;
					exit(0);
				}
			}
			video = access_name_for_stream;
			wrong_data = false;
		}

		else
		{
			cout << "WARNING! You typed wrong data!" << endl;

			file_config_login.close();
			file_config_login.open("Config_login.txt", ios::in);

			your_data(login, password, IP);
			check_your_IP(IP, file_cl_IP, login, password, file_config_login);
		}
	}
	file_config_login.close();
	file_config_camera.close();
}
//interval
int time_sleep_now(string *interval, string *archiving, ifstream &file_config_archiving_interval)
{
	int time_for_sleep_now = 0;
	string rubbish_line = "";

	if (file_config_archiving_interval.good() == true)
	{
		getline(file_config_archiving_interval, rubbish_line); //Omit additional line in Config_archiving_interval

		for (int i = 0; i < 4; i++)
		{
			file_config_archiving_interval >> interval[i];
			if (interval[0] == "a")
			{
				perror("Error in order in the \"file_config_archiving_interval\"");
				exit(0);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			file_config_archiving_interval >> archiving[i];
			if (archiving[0] == "i")
			{
				perror("Error in order in the \"file_config_archiving_interval\"");
				exit(0);
			}
		}
	}
	time_for_sleep_now = atoi(interval[1].c_str()) + 60 * (atoi(interval[2].c_str())) + 3600 * atoi(interval[3].c_str()); //to seconds

	//Close Config_archiving_interval
	file_config_archiving_interval.close();

	return time_for_sleep_now;
}
int main(int, char**)
{
	VideoCapture vcap;
	Mat image;

	//Login data
	string login = "";
	string password = "";
	string IP = "";
	string video = "";

	//Files
	ifstream file_config_login;
	ifstream file_config_camera;

	//Open Config_login && Config_camera files
	file_config_login.open("Config_login.txt", ios::in);
	file_config_camera.open("Config_camera.txt", ios::in);

	//Enter your data
	your_data(login, password, IP);

	//if files was opened correctly
	if (file_config_login.good() == true && file_config_camera.good() == true)
	{
		bool not_proper = true;
		while (not_proper)
		{
			check_your_data(file_config_login, file_config_camera, login, password, IP, not_proper, video);
		}
		cout << "Please wait!" << endl;
	}

	const string videoStreamAddress = "http://" + login + ":" + password + "@" + IP + "/" + video + ".mjpg";
	//const string videoStreamAddress = "http://78.9.31.142/mjpg/video.mjpg"; //helpful in home

	//open the video stream and make sure it's opened
	if (!vcap.open(videoStreamAddress))
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	create_folder(IP);
	cout << "Screenshots are saving in the \"Screenshots/" + IP + "\"!" << endl;

	//Config_archiving_interval
	ifstream file_config_archiving_interval;

	string *interval = new string[5];

	string *archiving = new string[5];

	string rubbish_line = "";

	//Open Config_archiving_interval
	file_config_archiving_interval.open("Config_archiving_interval.txt", ios::in);

	int time_for_sleep_now = 0;
	time_for_sleep_now = time_sleep_now(interval, archiving, file_config_archiving_interval);

	delete[]interval;
	delete[]archiving;

	time_t programstart, timepassed;

	//read initial frame
	while (!vcap.read(image))
	{
		cout << "No frame" << endl;
		return 1;
	}

	while (true)
	{
		imwrite(get_filename(IP), image); //save screenshot in the "Screenshots/..."
		//imshow("Camera window", image); //live stream

		programstart = time(0);
		timepassed = 0;

		while (timepassed < time_for_sleep_now)
		{
			while (!vcap.read(image))
			{
				cout << "No frame" << endl;
				return 1;
			}
			timepassed = time(0) - programstart;
		}
		/*
		//Remove file
		if (remove("Screenshots/<IP> 06-07-2016 09-05-57.PNG") != 0)
		{
			perror("Error deleting file");
		}
		else
		{
			puts("File successfully deleted");
		}
		*/
		if (waitKey(1) >= 0)
		{
			break;
		}
	}
}