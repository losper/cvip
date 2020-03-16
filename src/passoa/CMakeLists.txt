#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(OpenCV 3 REQUIRED core highgui imgcodecs imgproc videoio)

add_library(cvip SHARED
	cv.cpp
	index.cpp
	binding.cpp
	cvip.cpp
	camera.cpp
	export.def
	plugin.h
	)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(cvip ${OpenCV_LIBS})

set(binList)
foreach(c ${OpenCV_LIBS})
set(p)
file(GLOB p ${OpenCV_LIB_PATH}/../bin/${c}*)
list(APPEND binList ${p})
endforeach()

foreach(filename ${binList} )
add_custom_command(TARGET cvip POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${filename} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()
