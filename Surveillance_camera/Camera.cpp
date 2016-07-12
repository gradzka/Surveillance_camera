#include "Camera.h"

Camera::Camera(string ID, string address_IP, string login, string password, string model, string access_name_for_stream)
{
	this->ID = ID;
	this->address_IP = address_IP;
	this->login = login;
	this->password = password;
	this->model = model;
	this->access_name_for_stream = access_name_for_stream;
}


Camera::~Camera()
{

}

#ifdef _WIN32 //If this is Winodows system
void Camera::create_folder()
{
	string folder_name = "md Screenshots\\" + address_IP;
	system(folder_name.c_str());
}
#else //for Linux

void Camera::create_folder()
{
	system("mkdir Screenshots");
	string folder_name = "mkdir Screenshots/" + address_IP;
	system(folder_name.c_str());
}
#endif

string Camera::get_filename()
{
	char cur_time[20];
	time_t timer = time(0);
	struct tm *now = localtime(&timer);
	strftime(cur_time, sizeof(cur_time), "%d-%m-%Y %H-%M-%S", now);

	string filename;
	filename.assign(cur_time, 19);
	filename = "\"Screenshots/" + address_IP + "/" + filename + ".jpg\"";
	//cout << filename << endl;

	return filename;
}