# Surveillance_camera

##Overview
The main aim of the project is archiving data captured from surveillances cameras mounted in laboratory classes.

##Description
First step in this project was to capture screenshots from IP cameras. If you log in successfully (data in "Config_login" is proper), they are saved in "\Screenshots\IP" after time fixed in "file_config_archiving_interval.txt" and also deleted after time fixed in the same file. There is a possibility to change the position of IP cameras by using proper presets. Cameras change they position sequentially. What is more, you can run the program with parameters: "Surveillance_camera ID_1 ID_2 ..." . If you don't specify any parameters, you will run the program with all of cameras included in "Config_login". The option with typed parameters will start controlling all positions specified for cameras only one time. Option with no parameters will be running indefinitely. The program is compatible with Windows and Linux.

##Tools
- Microsoft Visual Studio 2013
- wget

##How to run


##Attributions
- http://gnuwin32.sourceforge.net/packages/wget.htm
- http://www.cplusplus.com/forum/beginner/46782/#msg256950

##License
MIT

##Credits
* Monika Grądzka
* Robert Kazimierczak
