cmake_minimum_required(VERSION 3.7)
project (telegram)

set (CMAKE_TOOLCHAIN_FILE "C:/Users/calmc/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (cpprestsdk_DIR "C:/Users/calmc/vcpkg/installed/x64-windows/share/cpprestsdk")

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
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(td-1.4.0)

add_executable(telegram telegram.cpp
server/server.cpp
server/telegram_client/telegram_client.cpp
server/telegram_client/handlers/authorization_handler.cpp
server/telegram_client/handlers/option_handler.cpp
server/telegram_client/handlers/connectionstate_handler.cpp
server/database_manager/database_manager.cpp
)

find_package(sqlite3 CONFIG REQUIRED)
target_link_libraries(telegram PRIVATE sqlite3)
find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(telegram PRIVATE SQLiteCpp)
find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(telegram PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal)
find_package(OpenSSL REQUIRED)
target_link_libraries(telegram PRIVATE OpenSSL::SSL OpenSSL::Crypto)
find_package(ZLIB REQUIRED)
target_link_libraries(telegram PRIVATE ZLIB::ZLIB)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(telegram PRIVATE spdlog::spdlog)
target_link_libraries(telegram PRIVATE cpprestsdk::cpprest)
target_link_libraries(telegram PRIVATE Td::TdStatic)
