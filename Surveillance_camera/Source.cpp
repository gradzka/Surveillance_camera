#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "time.h"

using namespace std;
using namespace cv;

string get_filename(string IP)
{
	time_t timer = time(0);   // get time now
	struct tm now;
	localtime_s(&now,&timer);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", &now);
	string filename;
	filename.assign(buffer, 19);
	filename = "Screenshots/" + IP + " " + filename + ".PNG";
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
void check_your_IP(string &IP, string &file_IP,string &login, string &password, ifstream &file_config_login)
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

			//Omit two additional lines in Config_file_login
			getline(file_config_login, rubbish_line);
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

	//Omit two additional lines in Config_file_login
	getline(file_config_login, rubbish_line); //Access data to log into the camera:
	getline(file_config_login, rubbish_line); //<address> <login> <password> <model>

	file_config_login >> file_cl_IP; //first IP from Config_login
	
	check_your_IP(IP, file_cl_IP,login, password, file_config_login); //Correct IP address or re-enter data

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
					cout << "Lack of necessary information in the Config_camera!"<<endl<< "Possibility of erroneous data in the Config_login!" << endl;
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
	if (file_config_login.good() == true && file_config_camera.good()==true) 
	{
		bool not_proper = true;
		while (not_proper)
		{
			check_your_data(file_config_login,file_config_camera, login, password, IP, not_proper, video);
		}
		cout << "Wait for video!" << endl;
	}

	//const string videoStreamAddress = "http://" + login + ":" + password + "@"+ IP + "/"+ video + ".mjpg";
	const string videoStreamAddress = "http://78.9.31.142/mjpg/video.mjpg";

	//open the video stream and make sure it's opened
	if (!vcap.open(videoStreamAddress)) 
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	int counter = 0;
	while (true) 
	{
		while (!vcap.read(image))
		{
			cout << "No frame" << endl;

			image = imread("Error.PNG", IMREAD_COLOR); // Read the file

			if (!image.data) // Check for invalid input
			{
				cout << "Could not open or find the image" << endl;
			}
			else
			{
				imshow("Camera window", image);
			}
			waitKey();
			return 1;
		}

		if (counter == 0)
		{
			imwrite(get_filename(IP), image);
		}
		counter++;
		if (counter == 30)
		{
			counter = 0;
		}
		imshow("Camera window", image);
		//Sleep(5000);
		if (waitKey(1) >= 0) 
		{ 
			break; 
		}
	}
}