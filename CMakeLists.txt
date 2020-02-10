cmake_minimum_required(VERSION 3.14)
project(CQPqjs VERSION 0.1.0) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须


# QuickJS C 库
file(STRINGS "extern/QuickJS/VERSION" QJS_VER)
set(QJS_SRC 
    extern/QuickJS/quickjs.c 
    extern/QuickJS/libbf.c 
    extern/QuickJS/libunicode.c 
    extern/QuickJS/libregexp.c 
    extern/QuickJS/cutils.c 
    extern/QuickJS/quickjs-libc.c)
set(QJS_DEF CONFIG_VERSION="${QJS_VER}" _GNU_SOURCE CONFIG_BIGNUM)
include_directories(extern/QuickJS)
add_library(quickjs STATIC ${QJS_SRC})
target_compile_definitions(quickjs PRIVATE ${QJS_DEF})

# cqcppsdk
include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须
include_directories(extern/cqcppsdk/include) # 设置 SDK 的 include 目录为包含目录, 必须

# 插件源码配置
include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改
file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)

cq_set_app_id(${APP_ID}) # 设置 app id, 必须

set(LIB_NAME "app") # 酷Q要求开发阶段的应用名为 app.dll

set(CQCPPSDK_DEV_MODE ON)
cq_add_app(${LIB_NAME} ${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
target_link_libraries(${LIB_NAME} quickjs)
