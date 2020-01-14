cmake_minimum_required(VERSION 3.7)
project (telegram CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#set (CMAKE_TOOLCHAIN_FILE "C:/Users/calmc/vcpkg/scripts/buildsystems/vcpkg.cmake")

set (VolubleTelegramClient_VERSION_MAJOR 0)
set (VolubleTelegramClient_VERSION_MINOR 1)
set (VolubleTelegramClient_VERSION_PATCH 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/VolubleTelegramClientConfig.h.in"
  "${PROJECT_BINARY_DIR}/VolubleTelegramClientConfig.h"
  )

  # add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}"
third_party/sqlite_orm/include
third_party/spdlog/include
)

add_subdirectory(td-1.4.0)

add_executable(telegram telegram.cpp
server/server.cpp
server/telegram_client/telegram_client.cpp
server/telegram_client/handlers/authorization_handler.cpp
server/telegram_client/handlers/option_handler.cpp
server/telegram_client/handlers/connectionstate_handler.cpp
#server/telegram_client/UserManager.cpp
server/database_manager/database_manager.cpp
server/database_manager/User.cpp
logger.cpp

)

#find_package(cpprestsdk CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
#find_package(spdlog CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
#find_library(sqlite3 REQUIRED)
find_package(sqlite3 REQUIRED CONFIG)

#FindSQLite3()
if(SQLite3_FOUND)
  message(STATUS "Found Sqlite3:")
  message(STATUS "-- Libs: ${SQLite3_LIBRARIES"})
  message(STATUS "-- Includes: ${SQLite3_INCLUDE_DIRS"})
  include_directories(${SQLite3_INCLUDE_DIRS})
  target_link_libraries(SQLite3_LIBRARIES)
endif()

target_link_libraries(telegram PRIVATE
#cpprestsdk::cpprest
#cpprestsdk::cpprestsdk_zlib_internal
#cpprestsdk::cpprestsdk_boost_internal
#cpprestsdk::cpprestsdk_openssl_internal
OpenSSL::SSL
OpenSSL::Crypto
ZLIB::ZLIB
#spdlog::spdlog
Td::TdStatic
sqlite3
)
#target_link_libraries(telegram LINK_PUBLIC sqlite3)
