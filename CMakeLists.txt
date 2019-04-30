cmake_minimum_required (VERSION 2.6)
project (VolubleTelegramClient)

set (VolubleTelegramClient_VERSION_MAJOR 1)
set (VolubleTelegramClient_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/VolubleTelegramClientConfig.h.in"
  "${PROJECT_BINARY_DIR}/VolubleTelegramClientConfig.h"
  )

  # add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(telegram telegram.cpp)