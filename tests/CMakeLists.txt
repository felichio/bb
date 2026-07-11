FILE(GLOB SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")
FILE(GLOB MOCK_FILES "${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.hpp" )
list(TRANSFORM SOURCE_FILES REPLACE "[.]cpp$" "")

foreach(test ${SOURCE_FILES})
  message("-- Adding test ${test} ---")
  add_executable(${test} "${test}.cpp")
  target_link_libraries(
    ${test}
    GTest::gtest_main
    GTest::gtest
    GTest::gmock
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    bb
  )
  target_sources(${test}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
      FILES "${MOCK_FILES}"
    )
  add_test(NAME ${test} COMMAND ${test})
endforeach()

