#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "time.h"
using namespace std;
using namespace cv;

//Sleep, creating folder for Windows and Linux
#ifdef _WIN32 //If this is Winodows system

#include "io.h"
#pragma warning (disable:4996)

void create_folder(string IP)
{
	string folder_name = "md Screenshots\\" + IP;
	system(folder_name.c_str());
}
void delete_screenshots(string *archiving, string IP)
{
	//time after which the screen is removed
	int time_from_archiving = 0; //in seconds
	time_from_archiving = atoi(archiving[1].c_str()) + 60 * (atoi(archiving[2].c_str())) + 3600 * atoi(archiving[3].c_str()) + 86400 * atoi(archiving[4].c_str());

	_finddata_t danePliku;
	long uchwyt;
	string name_of_the_screenshot = "";
	name_of_the_screenshot = "Screenshots/" + IP + "/" + "*.PNG";

	if ((uchwyt = _findfirst(name_of_the_screenshot.c_str(), &danePliku)) == -1)
	{
		cout << name_of_the_screenshot.c_str() << endl;
		perror("File not found!\n");
		exit(1);
	}

	do
	{
		if ((time(0)-danePliku.time_write) > time_from_archiving)
		{
			//Remove file
			name_of_the_screenshot = "Screenshots/" + IP +"/"+ danePliku.name;
			if (remove(name_of_the_screenshot.c_str()) != 0)
			{
				cout << name_of_the_screenshot.c_str() << endl;
				perror("Error deleting file\n");
				exit(1);
			}
		}
		//cout << danePliku.name << "\t" << danePliku.time_write << endl;
	} while (_findnext(uchwyt, &danePliku) == 0);

	_findclose(uchwyt);
}
#else //for Linux

#include <dirent.h>
#include <sys/stat.h>

void create_folder(string IP)
{
	system("mkdir Screenshots");
	string folder_name = "mkdir Screenshots/" + IP;
	system(folder_name.c_str());
}
void delete_screenshots(string *archiving, string IP)
{
	//time after which the screen is removed
	int time_from_archiving = 0; //in seconds
	time_from_archiving = atoi(archiving[1].c_str()) + 60 * (atoi(archiving[2].c_str())) + 3600 * atoi(archiving[3].c_str()) + 86400 * atoi(archiving[4].c_str());

	DIR *dpdf;
	struct dirent *epdf;
	char folderName[40] = "./Screenshots/";
	strcat(folderName, IP.c_str());
	strcat(folderName, "/");
	char path[60];

	dpdf = opendir(folderName);
	if (dpdf != NULL)
	{
		struct stat st;
		while (epdf = readdir(dpdf))
		{
			if (strstr(epdf->d_name, ".PNG") != NULL)
			{
				strcpy(path, folderName);
				strcat(path, epdf->d_name);

				if (stat(path, &st))
				{
					cout << epdf->d_name << endl;
					perror("Error deleting file\n");
					exit(1);
				}
				else 
				{
					if ((time(0) - (long long)st.st_mtim.tv_sec) > time_from_archiving) 
					{
						remove(path);
						//cout<<time(0)-(long long)st.st_mtim.tv_sec<<endl;
					}
				}
			}
		}
		closedir(dpdf);
	}
}
#endif

string get_filename(string IP)
{
	char cur_time[20];
	time_t timer = time(0);   // get time now
	struct tm *now = localtime(&timer);
	strftime(cur_time, sizeof(cur_time), "%d-%m-%Y %H-%M-%S", now);

	string filename;
	filename.assign(cur_time, 19);
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

			if (file_config_login.good()==true)
			{
				//Omit additional line in Config_file_login
				getline(file_config_login, rubbish_line);

				your_data(login, password, IP);
			}
			else
			{
				perror("Error with \"Config_login.txt\"\n");
				exit(1);
			}
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
	if (file_config_camera.good())
	{
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
						perror("Lack of necessary information in the Config_camera!\nPossibility of erroneous data in the Config_login!\n");
						exit(1);
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
				if (file_config_login.good()==true)
				{
					your_data(login, password, IP);
					check_your_IP(IP, file_cl_IP, login, password, file_config_login);
				}
				else
				{
					perror("Error with \"Config_login.txt\"\n");
					exit(1);
				}
			}
		}
		file_config_login.close();
		file_config_camera.close();
	}
	else
	{
		perror("Error with \"Config_camera.txt\"\n");
		exit(1);
	}

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
				perror("Error in order in the \"file_config_archiving_interval.txt\"\n");
				exit(1);
			}
		}

		for (int i = 0; i < 5; i++)
		{
			file_config_archiving_interval >> archiving[i];
			if (archiving[0] == "i")
			{
				perror("Error in order in the \"file_config_archiving_interval.txt\"\n");
				exit(1);
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


	//if files was opened correctly
	if (file_config_login.good() == true && file_config_camera.good() == true)
	{
		//Enter your data
		your_data(login, password, IP);

		bool not_proper = true;
		while (not_proper)
		{
			check_your_data(file_config_login, file_config_camera, login, password, IP, not_proper, video);
		}
		cout << "Please wait!" << endl;
	}
	else
	{
		perror("Error with \"Config_login.txt\" or \"Config_camera.txt\"\n");
		exit(1);
	}
	const string videoStreamAddress = "http://" + login + ":" + password + "@" + IP + "/" + video + ".mjpg";
	//const string videoStreamAddress = "http://78.9.31.142/mjpg/video.mjpg"; //helpful in home

	//open the video stream and make sure it's opened
	if (!vcap.open(videoStreamAddress))
	{
		perror("Error opening video stream or file\n");
		exit(1);
	}
	create_folder(IP);
	cout << "Screenshots are saving in the \"Screenshots/" + IP + "\"!" << endl;

	//Config_archiving_interval
	ifstream file_config_archiving_interval;

	string *interval = new string[5];

	string *archiving = new string[6];

	string rubbish_line = "";

	//Open Config_archiving_interval
	file_config_archiving_interval.open("Config_archiving_interval.txt", ios::in);

	if (file_config_archiving_interval.good() == false)
	{
		perror("Error with \"Config_archiving_interval.txt\"\n");
		exit(1);
	}

	int time_for_sleep_now = 0;
	time_for_sleep_now = time_sleep_now(interval, archiving, file_config_archiving_interval);

	time_t programstart, timepassed;

	//read initial frame
	while (!vcap.read(image))
	{
		perror("No frame\n");
		exit(1);
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
				perror("No frame!\n");
				exit(1);
			}
			timepassed = time(0) - programstart;
		}
		delete_screenshots(archiving,IP);
		
		if (waitKey(1) >= 0)
		{
			break;
		}
	}

	delete[]interval;
	delete[]archiving;
}