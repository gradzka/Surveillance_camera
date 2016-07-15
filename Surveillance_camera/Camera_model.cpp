#include "Camera_model.h"

Camera_model::Camera_model()
{
}

Camera_model::~Camera_model()
{
}

string Camera_model::get_filename(string address_IP)
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

bool Camera_model::is_file_empty(ifstream &file)
{
	int length = 0;
	file.seekg(0, ios::end);
	// odczytaj pozycje w pliku -> rozmiar pliku
	length = file.tellg();
	// wroc na poczatek pliku!
	file.seekg(0, ios::beg);
	if (length == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

#ifdef _WIN32

void Camera_model::find_path_to_wget()
{
	cout << "Please wait, program will finish searching \"WGET_DIR\" environment variable soon\n";
	
	char *value_of_variable = NULL;
	value_of_variable = getenv("WGET_DIR");
	if (value_of_variable != NULL)
	{
		string str_value_of_variable(value_of_variable);
		str_value_of_variable += "\\";
		path_to_wget = str_value_of_variable;
		cout << "Program found \"WGET_DIR\" environment variable!\n";
	}
	else
	{
		throw "Error! Can't find \"WGET_DIR\" environment variable!\n";
	}
}

#endif

string Camera_model::path_to_wget = "";