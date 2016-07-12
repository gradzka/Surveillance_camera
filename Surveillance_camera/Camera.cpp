#include "Camera.h"

Camera::Camera(string ID, string address_IP, string login, string password, string model)
{
	this->ID = ID;
	this->address_IP = address_IP;
	this->login = login;
	this->password = password;
	this->model = return_camera_model_pointer(model, ID, address_IP, login, password);
}

Camera::~Camera()
{

}

#ifdef _WIN32 //If this is Winodows system
void Camera::create_folder()
{
	string folder_name = "md Screenshots\\" + address_IP + " 2> NUL";
	system(folder_name.c_str());
	cout << "Screenshots are saving in the \"Screenshots/" + address_IP + "\"!" << endl;
}

void Camera::delete_screenshots(int time_archiving)
{
	_finddata_t danePliku;
	long uchwyt;
	string name_of_the_screenshot = "";
	name_of_the_screenshot = "Screenshots/" + address_IP + "/" + "*.jpg";

	if ((uchwyt = _findfirst(name_of_the_screenshot.c_str(), &danePliku)) == -1)
	{
		cout << name_of_the_screenshot.c_str() << endl;
		perror("File not found!\n");
		exit(1);
	}

	do
	{
		if ((time(0) - danePliku.time_write) > time_archiving)
		{
			//Remove file
			name_of_the_screenshot = "Screenshots/" + address_IP + "/" + danePliku.name;
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

void Camera::create_folder()
{
	system("mkdir Screenshots 2>>/dev/null");
	string folder_name = "mkdir Screenshots/" + address_IP + " 2>>/dev/null";
	system(folder_name.c_str());
	cout << "Screenshots are saving in the \"Screenshots/" + address_IP + "\"!" << endl;
}

void Camera::delete_screenshots(int time_archiving)
{
	DIR *directory;
	struct dirent *file_screenshot;
	char folderName[40] = "./Screenshots/";
	strcat(folderName, address_IP.c_str());
	strcat(folderName, "/");
	char path[60];

	directory = opendir(folderName);
	if (directory != NULL)
	{
		struct stat st;
		while (file_screenshot = readdir(directory))
		{
			if (strstr(file_screenshot->d_name, ".jpg") != NULL)
			{
				strcpy(path, folderName);
				strcat(path, file_screenshot->d_name);

				if (stat(path, &st))
				{
					cout << file_screenshot->d_name << endl;
					perror("Error deleting file\n");
					exit(1);
				}
				else
				{
					if ((time(0) - (long long)st.st_mtim.tv_sec) > time_archiving)
					{
						remove(path);
						//cout<<time(0)-(long long)st.st_mtim.tv_sec<<endl;
					}
				}
			}
		}
		closedir(directory);
	}
}
#endif

Camera_model* Camera::return_camera_model_pointer(string model_login, string ID, string address_IP, string login, string password)
{
	Camera_model *camera_model;

	if (model_login == "TP-Link")
	{
		camera_model = new TP_Link();
	}
	else if (model_login == "DLink")
	{
		camera_model = new DLink();
	}
	else
	{
		perror("Undefined type in camera_model! Please define it in code!");
		exit(1);
	}

	camera_model->fill_list_of_presets(login, password, address_IP, list_of_presets);
	return camera_model;
}