#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(OpenCV 3 REQUIRED)

add_library(cvip SHARED
	${PROJECT_SOURCE_DIR}/binding/cvip/cv.cpp
	${PROJECT_SOURCE_DIR}/binding/cvip/index.cpp
	${PROJECT_SOURCE_DIR}/binding/cvip/cvip.cpp
	${PROJECT_SOURCE_DIR}/src/cvip/cvip.cpp
	${PROJECT_SOURCE_DIR}/binding/export.def
	${PROJECT_SOURCE_DIR}/binding/plugin.h
	)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(cvip ${OpenCV_LIBS})
install(TARGETS cvip RUNTIME DESTINATION node_modules/cvip)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/cvip/dist/ DESTINATION node_modules/cvip)
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/test/ DESTINATION test)
