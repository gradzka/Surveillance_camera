cmake_minimum_required(VERSION 2.8)
project( Surveillance_camera )
file(GLOB Surveillance_camera_SRC
    "*.h"
    "*.cpp"
)
add_executable( Surveillance_camera ${Surveillance_camera_SRC} )
