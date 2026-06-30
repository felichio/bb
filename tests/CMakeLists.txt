add_executable(
  hello_test
  hello_test.cpp
  ${CMAKE_SOURCE_DIR}/src/core/EventQueue.cpp
)

target_include_directories(hello_test PRIVATE ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(
  hello_test
  GTest::gtest_main
  GTest::gtest
  spdlog::spdlog
)

add_test(NAME some COMMAND hello_test)