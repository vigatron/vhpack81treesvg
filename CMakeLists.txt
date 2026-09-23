cmake_minimum_required(VERSION 3.16)

project(vhpack81treesvg VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(
	src/mods
	src/mods/colors
	src/mods/strtools
	src/mods/layerarea
	src/mods/tree
	src/mods/svg

	src/calc
	src/draw
	src/parser
	src/global
	src/generate
	src
)

file( GLOB_RECURSE source_list_mods 
	src/*.cpp  src/*.hpp
	src/calc/*
	src/draw/*
	src/parser/*
	src/global/*
	src/generate/*
	src/*

	src/mods/tree/*
	src/mods/colors/*
	src/mods/strtools/*
	src/mods/layerarea/*
	src/mods/svg/*
	src/mods/*.cpp
    src/mods/*.hpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${source_list_mods} )
