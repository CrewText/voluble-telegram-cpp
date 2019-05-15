cmake_minimum_required(VERSION 3.7)
project (telegram)

set (CMAKE_TOOLCHAIN_FILE "C:/Users/calmc/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (cpprestsdk_DIR "C:/Users/calmc/vcpkg/installed/x64-windows/share/cpprestsdk")
set (SQLITECPP_ENABLE_ASSERT_HANDLER true)

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
${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp/include
)

add_subdirectory(td-1.4.0)
add_subdirectory(thirdparty/SQLiteCpp)

add_executable(telegram telegram.cpp
server/server.cpp
server/telegram_client/telegram_client.cpp
server/telegram_client/handlers/authorization_handler.cpp
server/telegram_client/handlers/option_handler.cpp
server/telegram_client/handlers/connectionstate_handler.cpp
server/database_manager/database_manager.cpp
)

find_package(cpprestsdk CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(telegram PRIVATE
SQLiteCpp
sqlite3
cpprestsdk::cpprest
cpprestsdk::cpprestsdk_zlib_internal
cpprestsdk::cpprestsdk_boost_internal
cpprestsdk::cpprestsdk_openssl_internal
OpenSSL::SSL
OpenSSL::Crypto
ZLIB::ZLIB
spdlog::spdlog
cpprestsdk::cpprest
Td::TdStatic
)
