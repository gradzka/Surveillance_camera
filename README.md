# Surveillance_camera

##Overview
The main aim of the project is archiving data captured from surveillances cameras mounted in laboratory classes.

##Description
First step in this project was to capture screenshots from IP cameras. If you log in successfully (data in "Config_login.txt" is proper), they are saved in "\Screenshots\IP" after time fixed in "file_config_archiving_interval.txt" and also deleted after time fixed in the same file. Program communicates with IP camera by using http request and wget. There is a possibility to change the position of IP cameras by using proper presets. Cameras change they position sequentially. What is more, you can run the program with parameters: "Surveillance_camera ID_1 ID_2 ..." . If you don't specify any parameters, you will run the program with all of cameras included in "Config_login.txt". The option with typed parameters will start controlling all positions specified for cameras only one time. Option with no parameters will be running indefinitely. The program is compatible with Windows and Linux.

##Tools
- Microsoft Visual Studio 2013
- wget

##How to run
Warning: You must have config files properly filled!

###Windows
- download the latest wget version
- make sure if the name of application is "wget.exe"; if not, change name into "wget.exe"
- make sure if the path to the "wget.exe" doesn't contain any special characters, e.g.: 'ą', 'ä'
- set "WGET_DIR" environment variable with value as path to the "wget.exe", e.g. in cmd: setx -m WGET_DIR "C:GnuWin32\bin"
- run "Surveillance_camera.exe"

###Linux
- download the latest wget version: sudo apt-get install wget
- run "Surveillance_camera"

##How to compile

###Windows
- you can use Microsoft Visual Studio to open the project and compiled it
- other way is using the g++ compiler; in cmd type: g++ -std=c++11 -o Surveillance_camera.exe Source.cpp Camera.cpp Camera_model.cpp Preset.cpp DLink.cpp TP_Link.cpp

###Linux
- using the terminal, go to the directory, where file CMakeLists.txt is
- type cmake .
- type make

##Attributions
- http://gnuwin32.sourceforge.net/packages/wget.htm
- http://www.linux.net.pl/~wkotwica/doc/wget/wget_3.html
- http://www.cplusplus.com/forum/beginner/46782/#msg256950
- http://stackoverflow.com/questions/6605282/how-can-i-check-if-a-string-has-special-characters-in-c-effectively/6605305#6605305
- http://stackoverflow.com/questions/1658386/sleep-function-in-c/1658429#1658429
- http://cboard.cprogramming.com/c-programming/76434-time-filename.html#post541515
- http://www.cplusplus.com/reference/cstdlib/getenv/

##License
MIT

##Credits
* Monika Grądzka
* Robert Kazimierczak

The project was conducted during the student internship held by the Institute of Control and Information Engineering, Poznan University of Technology.

Supervisor: Eng. Dariusz Urbański
