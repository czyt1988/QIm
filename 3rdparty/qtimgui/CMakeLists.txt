# minimum require
cmake_minimum_required(VERSION 3.25.0)

# global solution
project(qtimgui VERSION 1.0.0 LANGUAGES CXX)

# goto subs
add_subdirectory(examples)
add_subdirectory(modules)
add_subdirectory(src)
