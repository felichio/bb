cmake_minimum_required(VERSION 3.30)

project(BB)

# Boost
include(cmake/boost.cmake)
# nlohmann_json
include(cmake/nlohmann_json.cmake)
# spdlog
include(cmake/spdlog.cmake)

# .env copy
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/.env DESTINATION ${CMAKE_BINARY_DIR}/config)

add_subdirectory(src)

if(ENABLE_TESTS)
  message("ENABLE TESTS OK")
  include(cmake/googletest.cmake)
  enable_testing()
  add_subdirectory(tests)
endif()
