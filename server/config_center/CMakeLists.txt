cmake_minimum_required(VERSION 3.29.3)
project(config_center)
option(GROUP_BY_EXPLORER ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_PARALLEL_LEVEL 32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_COMPONENT_DIR}/../../../server_shipping)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /utf-8")

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_definitions(-D_WIN32_WINNT=0x0601)

file(GLOB_RECURSE HEADER_FILES *.h *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/proto_codec\\.cpp$")

set(AllFile ${HEADER_FILES} ${SOURCE_FILES})

foreach(fileItem ${AllFile})
	get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
	string(REPLACE "/" "\\" GROUP "${GROUP}")
	source_group("${GROUP}" FILES "${fileItem}")
endforeach()

add_executable(config_center ${AllFile})

option(USE_SOLUTION_FOLDERS ON)
set_target_properties(config_center PROPERTIES FOLDER "src")

target_include_directories(config_center PUBLIC
	${PROJECT_COMPONENT_DIR}
	${PROJECT_COMPONENT_DIR}/config_center
	${PROJECT_COMPONENT_DIR}/def
	${PROJECT_COMPONENT_DIR}/def/log
	${PROJECT_COMPONENT_DIR}/def/logic
	${PROJECT_COMPONENT_DIR}/def/utility
	${PROJECT_COMPONENT_DIR}/def/protocol
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include
)

target_link_directories(config_center PUBLIC
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/lib
)

target_link_libraries(config_center PUBLIC
	hld_def
	server_common
	server_net
	event
	event_core
	event_extra
	event_openssl
	libssl
	libcrypto
	libcurl
	zlib
	lua
	jsoncpp
	spdlog
	fmt
	TracyClient
	redis++
	hiredis
	ws2_32
	boost_thread-vc143-mt-x64-1_88
	boost_serialization-vc143-mt-x64-1_88
)
