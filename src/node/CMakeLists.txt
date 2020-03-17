#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".node")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(OpenCV 4 REQUIRED core highgui imgcodecs imgproc videoio)
set(PROJECT_NAME cvip)
add_library(${PROJECT_NAME} SHARED
	index.cpp
	cvip.cpp
	)
# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
message("test:" ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR} ${OpenCV_INCLUDE_DIRS} ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_JS_LIB})

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_LIB_PATH}/../bin/opencv_world420d.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else() 
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_LIB_PATH}/../bin/opencv_world420.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
# set(binList)
# foreach(c ${OpenCV_LIB_COMPONENTS})
# set(p)
# file(GLOB p ${OpenCV_LIB_PATH}/../bin/${c}*)
# message("test2:" ${p})
# list(APPEND binList ${p})
# endforeach()

# foreach(filename ${binList} )
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
#                    COMMAND ${CMAKE_COMMAND} -E copy ${filename} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# endforeach()
