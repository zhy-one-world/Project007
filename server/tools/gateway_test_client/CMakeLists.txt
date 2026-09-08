cmake_minimum_required(VERSION 3.29.3)

project(gateway_test_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

# Deploy next to shipping DLLs so System32\zlib1.dll cannot shadow libcurl deps.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_COMPONENT_DIR}/../../../server_shipping)

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_definitions(-DSPDLOG_USE_STD_FORMAT)
add_definitions(-D_WIN32_WINNT=0x0601)

set(SERVER_BASE_PUBLIC_DIR "${PROJECT_COMPONENT_DIR}/../../SeverBase/include/public")
set(PROJECT_DEP_DIR "${PROJECT_COMPONENT_DIR}/../../dep")

add_executable(gateway_test_client
	gateway_load_client.hpp
	gateway_load_client.cpp
	main.cpp
)
target_link_options(gateway_test_client PRIVATE "/FORCE:MULTIPLE")

target_include_directories(gateway_test_client PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_COMPONENT_DIR}
	${PROJECT_COMPONENT_DIR}/def
	${PROJECT_COMPONENT_DIR}/def/logic
	${PROJECT_COMPONENT_DIR}/def/protocol
	${PROJECT_COMPONENT_DIR}/def/utility
	${PROJECT_COMPONENT_DIR}/def/protocol/internet
	${SERVER_BASE_PUBLIC_DIR}
	${SERVER_BASE_PUBLIC_DIR}/common
	${PROJECT_DEP_DIR}/vcpkg/installed/x64-windows/include
	${PROJECT_DEP_DIR}/vcpkg/installed/x64-windows/include/engine
	${PROJECT_DEP_DIR}/google/protocolbuff/src
)

target_link_directories(gateway_test_client PRIVATE
	${PROJECT_DEP_DIR}/vcpkg/installed/x64-windows/lib
	${PROJECT_DEP_DIR}/google/protocolbuff/vsprojects/Bin/Release
)

target_link_libraries(gateway_test_client PRIVATE
	hld_def
	server_common
	server_net
	event
	libcurl
	libssl
	libcrypto
	zlib
	lua
	libprotobuf
	abseil_dll
	jsoncpp
	spdlog
	fmt
	TracyClient
	boost_thread-vc143-mt-x64-1_88
	boost_serialization-vc143-mt-x64-1_88
)

set_target_properties(gateway_test_client PROPERTIES FOLDER "tools")
