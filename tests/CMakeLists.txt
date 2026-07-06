FILE(GLOB SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
list(TRANSFORM SOURCE_FILES REPLACE "[.]cpp$" "")

foreach(test ${SOURCE_FILES})
  message("-- Adding test ${test} ---")
  add_executable(${test} "${test}.cpp")
  target_link_libraries(
    ${test}
    GTest::gtest_main
    GTest::gtest
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    bb
  )
  add_test(NAME ${test} COMMAND ${test})
endforeach()

