cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)

project (hello-world-native)


# define NPI_VERSION
add_definitions(-DNAPI_VERSION=7)

set (CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_JS_INC})
file(GLOB SOURCE_FILES "src/hello_world.cc")

option(OTIO_CXX_INSTALL "" OFF)
option(OTIO_DEPENDENCIES_INSTALL "" OFF)
option(OTIO_INSTALL_PYTHON_MODULES "" OFF)
option(OTIO_INSTALL_COMMANDLINE_TOOLS "" OFF)
option(OTIO_INSTALL_CONTRIB "" OFF)
option(OTIO_SHARED_LIBS "" OFF)
add_subdirectory("deps/OpenTimelineIO")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_directories(${PROJECT_NAME} PRIVATE "deps/OpenTimelineIO")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} opentimelineio)

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()