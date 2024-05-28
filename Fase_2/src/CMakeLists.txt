cmake_minimum_required(VERSION 3.9)

project(fase2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(engine ./engine/engine.cpp)
add_executable(generator ./generator/generator.cpp)

add_library(
    utils_lib
    ./utils/point.cpp
    ./utils/point.hpp
    ./utils/figure.cpp
    ./utils/figure.hpp
)

add_library(
    tinyXML2_lib
    ./tinyXML2/tinyxml2.cpp
    ./tinyXML2/tinyxml2.h
)

add_library(
    figures_lib
    ./generator/sphere.cpp
    ./generator/sphere.hpp
	./generator/ring.cpp
    ./generator/ring.hpp
	./generator/box.cpp
	./generator/box.hpp
	./generator/plane.cpp
	./generator/plane.hpp
	./generator/cone.cpp
	./generator/cone.hpp
)

add_library(
	config_lib
	./engine/config.cpp
	./engine/config.hpp	
	./engine/group.hpp
	./engine/group.cpp
	./engine/transform.hpp
	./engine/transform.cpp
)

target_link_libraries(generator utils_lib figures_lib)
target_link_libraries(engine utils_lib figures_lib tinyXML2_lib config_lib)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	
	include_directories(${TOOLKITS_FOLDER}/glut )
	#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
	#									  ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(generator ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	
	
	#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
	
	target_link_libraries(generator.exe ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	target_link_libraries(engine.exe ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)