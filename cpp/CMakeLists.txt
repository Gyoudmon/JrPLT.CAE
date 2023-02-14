# CMakeList.txt : CMake project for MagicalEnergyExpedition, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

project(YouthLanguage)

# Path settings
include_directories(village/vcso/include)
link_directories(village/vcso/lib)

# Add source to this project's executable.
add_executable (Gallery Gallery.cpp)
set_property(TARGET Gallery PROPERTY CXX_STANDARD 17)

######################### BONUS ######################### 
# Sketch Projects
file(GLOB sketches village/sketch/*.cpp)
foreach(sketch ${sketches})
	get_filename_component(t ${sketch} NAME_WE)
	add_executable(${t} ${sketch})
	set_property(TARGET ${t} PROPERTY CXX_STANDARD 17)
endforeach()

# STEMA Projects
# file(GLOB STEMAs village/STEMA/*.cpp)
# foreach(STEMA ${STEMAs})
#	get_filename_component(t ${STEMA} NAME_WE)
#	add_executable(${t} ${STEMA})
#	set_property(TARGET ${t} PROPERTY CXX_STANDARD 17)
# endforeach()
