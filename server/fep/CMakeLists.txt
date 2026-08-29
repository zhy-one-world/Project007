cmake_minimum_required(VERSION 3.29.3)     # 指定 CMake 版本号
project(fep)                            # 项目名称为 fep
option(GROUP_BY_EXPLORER ON)            # 启用保留文件结构和资源管理器一样
set(CMAKE_CXX_STANDARD 20)              # 设置C++版本 20
set(CMAKE_BUILD_PARALLEL_LEVEL 32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_COMPONENT_DIR}/../../../server_shipping)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_definitions(-DSPDLOG_USE_STD_FORMAT)
#add_definitions(-DTRACY_ENABLE)
add_definitions(-D_WIN32_WINNT=0x0601)

# 递归查找文件夹下的 .h .hpp. ini 文件保存到 HEADER_FILES
file(GLOB_RECURSE HEADER_FILES *.h *.hpp) 

# 递归查找文件夹下的 *.cpp *.c 文件保存到 SOURCE_FILES
file(GLOB_RECURSE SOURCE_FILES *.cpp)

# 递归查找文件夹下的 *.hlsl 文件保存到 SHADER_FILES(如果没有 shader 可以删除)
#file(GLOB_RECURSE SHADER_FILES *.hlsl)

# 将 HEDADER_FILES 和 SOURCE_FILES 和 SHADER_FILES 保存到 AllFile 变量
set(AllFile ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

# 对 AllFile 变量里面的所有文件分类(保留资源管理器的目录结构)
foreach(fileItem ${AllFile})       
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${fileItem}" DIRECTORY)
    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    # Group into "Source Files" and "Header Files"
    set(GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${fileItem}")
endforeach()

# 将 AllFile 里面的所有文件编译为 fep 可执行文件
add_executable(fep ${AllFile})

# 启用VS项目文件夹分类
option(USE_SOLUTION_FOLDERS  ON)
# 设置 fep 为 src 文件夹下
set_target_properties(fep PROPERTIES FOLDER "src")

# 设置 hlsl 为 shader 资源文件
#set_property(SOURCE ${SHADER_FILES} PROPERTY VS_TOOL_OVERRIDE "shader") 

# 头文件目录
target_include_directories(fep PUBLIC 
    ${PROJECT_COMPONENT_DIR}
    ${PROJECT_COMPONENT_DIR}/fep
    ${PROJECT_COMPONENT_DIR}/def
    ${PROJECT_COMPONENT_DIR}/def/log
    ${PROJECT_COMPONENT_DIR}/def/logic
    ${PROJECT_COMPONENT_DIR}/def/utility
    ${PROJECT_COMPONENT_DIR}/def/protocol
    ${PROJECT_COMPONENT_DIR}/def/protocol/internal
    ${PROJECT_COMPONENT_DIR}/def/protocol/internet
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include/engine
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include/engine/common
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include/engine/net
	${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/include/engine/lua
	${PROJECT_COMPONENT_DIR}/../../dep/google/protocolbuff/src
)

# 链接库目录(如果有才需要)
target_link_directories(fep PUBLIC
    ${PROJECT_COMPONENT_DIR}/../../dep/vcpkg/installed/x64-windows/lib
    ${PROJECT_COMPONENT_DIR}/../../dep/google/protocolbuff/vsprojects/Bin/Release
)

# 链接库
target_link_libraries(fep PUBLIC
	hld_def
	release_common
	release_lua
	release_net
	event
	libcurl
	zlib
	lua
	libprotobuf
	jsoncpp
	spdlog
	fmt
	TracyClient
	boost_thread-vc143-mt-x64-1_85
	boost_serialization-vc143-mt-x64-1_85
)