#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
include_directories(${PROJECT_SOURCE_DIR}/output/include)
LINK_DIRECTORIES (${PROJECT_SOURCE_DIR}/output/lib)

if(PASSOA_COMPILER_MSVC)
#ADD_EXECUTABLE(opencvtest
#${PROJECT_SOURCE_DIR}/test/cpp/opencvtest.cpp
#)
#target_link_libraries(opencvtest
#	debug opencv_core320d
#	debug opencv_imgproc320d
#	debug opencv_highgui320d
#	debug opencv_imgcodecs320d
#	optimized opencv_core320
#	optimized opencv_imgproc320
#	optimized opencv_highgui320
#	optimized opencv_imgcodecs320
#)
else()

endif()
