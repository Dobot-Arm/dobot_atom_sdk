cmake_minimum_required(VERSION 3.16)
project(atom_sdk)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#配置CYCLONEDDS_URI环境变量为config/cyclonedds.xml
add_compile_definitions(_SET_CYCLONEDDS_URI)

# 头文件搜索路径
list(APPEND SDK_INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/idl
    ${CMAKE_SOURCE_DIR}/thirdparty/eigen3
    ${CMAKE_SOURCE_DIR}/thirdparty/yaml
    ${CMAKE_SOURCE_DIR}/thirdparty/yaml/include
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/include
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/rpc/include
)

find_package(Threads REQUIRED)
# 库文件路径
list(APPEND SDK_LIBS_DIR
    ${CMAKE_SOURCE_DIR}/lib/libclient_rpc.a
    ${CMAKE_SOURCE_DIR}/lib/libcommon.a
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/lib/libddsc.so
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/lib/libcycloneddsidl.so
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/lib/libdds_security_ac.so
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/lib/libdds_security_auth.so
    ${CMAKE_SOURCE_DIR}/thirdparty/DDS/lib/libdds_security_crypto.so
    ${CMAKE_SOURCE_DIR}/thirdparty/yaml/lib/libyaml-cpp.a
    Threads::Threads
)

# 设置所有可执行文件输出到build根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(example)
