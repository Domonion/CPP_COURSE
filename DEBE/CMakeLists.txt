cmake_minimum_required(VERSION 3.10)
project(DEBE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wshadow -pedantic -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -fvisibility=hidden")

add_executable(DEBE
        addresschecker.cpp
	addresschecker.h
	main.cpp
	mainwindow.cpp
	mainwindow.h
	mymainwindow.cpp
	mymainwindow.h
	window.cpp
	window.h
	xmlwriter.cpp
	xmlwriter.h
	)

qt5_use_modules(DEBE Widgets)
target_link_libraries(DEBE Qt5::Network)
