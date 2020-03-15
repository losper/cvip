#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".node")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

find_package(OpenCV 3 REQUIRED core highgui imgcodecs imgproc videoio)
set(PROJECT_NAME cvip)
add_library(${PROJECT_NAME} SHARED
	index.cpp
	)
# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR} ${OpenCV_INCLUDE_DIRS} ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_JS_LIB})

set(binList)
foreach(c ${OpenCV_LIBS})
set(p)
file(GLOB p ${OpenCV_LIB_PATH}/../bin/${c}*)
list(APPEND binList ${p})
endforeach()

foreach(filename ${binList} )
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${filename} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()
