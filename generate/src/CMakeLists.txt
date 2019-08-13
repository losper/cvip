#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(OpenCV 3 REQUIRED core highgui imgcodecs imgproc videoio)

add_library(cvip SHARED
	${PROJECT_SOURCE_DIR}/src/cvip/cv.cpp
	${PROJECT_SOURCE_DIR}/src/index.cpp
	${PROJECT_SOURCE_DIR}/src/binding.cpp
	${PROJECT_SOURCE_DIR}/src/cvip/cvip.cpp
	${PROJECT_SOURCE_DIR}/src/camera/camera.cpp
	${PROJECT_SOURCE_DIR}/src/export.def
	${PROJECT_SOURCE_DIR}/src/plugin.h
	)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(cvip ${OpenCV_LIBS})
install(TARGETS cvip RUNTIME DESTINATION node_modules/cvip)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/stage/ DESTINATION node_modules/cvip)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/test/ DESTINATION test)
set(binList)
foreach(c ${OpenCV_LIBS})
set(p)
file(GLOB p ${OpenCV_LIB_PATH}/../bin/${c}*)
list(APPEND binList ${p})
endforeach()
install(FILES ${binList} DESTINATION node_modules/cvip)