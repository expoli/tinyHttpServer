# CMakeList.txt: firstWebServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (firstWebServer main.cpp config.h file/file.cpp file/file.h http/http.cpp http/http.h utils/utils.h utils/utils.cpp)

if( WIN32)
    target_link_libraries(firstWebServer wsock32 ws2_32)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET firstWebServer PROPERTY CXX_STANDARD 11)
endif()

# TODO: 如有需要，请添加测试并安装目标。
