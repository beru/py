cmake_minimum_required(VERSION 3.14)

project(py)

include_directories(./)

add_executable(py
	main.cpp
	map.h
	py.h
	py.cpp
	)

set_target_properties(py PROPERTIES
	CXX_STANDARD 17
)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT py)
endif()

