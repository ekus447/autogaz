cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(AutoGaz)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "AutoGaz")
set(SHORT_NAME AutoGaz)
set(VITA_TITLEID  "ATGZ447")
set(VITA_VERSION  "01.31")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
	../common
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${VITA_APP_NAME}
  src/main.cpp
)

target_link_libraries(${VITA_APP_NAME}
  SceLibKernel_stub
  SceDisplay_stub
  SceTouch_stub
  SceAudio_stub
  -lvita2d 
  -lSceDisplay_stub 
  -lSceGxm_stub
  -lSceSysmodule_stub
  -lSceCtrl_stub
  -lScePgf_stub
  -lScePvf_stub
  -lSceCommonDialog_stub
  -lfreetype
  -lpng
  -ljpeg
  -lz
  -lm
  -lc
  soloud
  pthread
  m
)

vita_create_self(${SHORT_NAME}.self ${SHORT_NAME})
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  
  # resources
  FILE src/resources/before.png resources/before.png
  FILE src/resources/after.png resources/after.png
  FILE src/resources/polat.png resources/polat.png
  FILE src/resources/agm.ogg resources/agm.ogg

  # Livearea files
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
