cmake_minimum_required(VERSION 3.10)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
 if(DEFINED ENV{VITASDK})
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain")
 else()
  message(FATAL_ERROR "Define VITASDK")
 endif()
endif()
project(vitastream)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "PS Vita Stream")
set(VITA_TITLEID "VSTR00002")
set(VITA_VERSION "02.01")
add_executable(${PROJECT_NAME} src/main.c src/capture.c src/transport.c)
target_link_libraries(${PROJECT_NAME}
 SceDisplay_stub
 SceKernelThreadMgr_stub
 SceKernelModulemgr_stub
 SceSysmodule_stub
 SceNet_stub
 SceNetCtl_stub
)
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self VERSION ${VITA_VERSION} NAME ${VITA_APP_NAME})
