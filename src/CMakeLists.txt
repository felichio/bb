# find_package(Boost REQUIRED COMPONENTS filesystem)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(bb)

target_sources(
  bb
  PRIVATE
    main.cpp
)

add_subdirectory(util)

# Boost
option(B2 "Enable b2 build for boost" OFF)
if(B2)
  message("Using b2 for boost")
  list(JOIN BOOST_INCLUDE_LIBRARIES "," BOOST_INCLUDE_LIBRARIES_CSV)
  # exlcude here pure header only libraries in order to remove the from --with-libraries option
  list(REMOVE_ITEM BOOST_INCLUDE_LIBRARIES_CSV "")

  # bootstrap and b2
  execute_process(
    COMMAND ./bootstrap.sh --prefix=dist --with-libraries=${BOOST_INCLUDE_LIBRARIES_CSV}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/boost
  )

  execute_process(
    COMMAND ./b2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/boost
  )

  execute_process(
    COMMAND ./b2 install
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/boost
  )

  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/external/boost/dist/lib/cmake/Boost-1.89.0)
  find_package(Boost REQUIRED)
else()
  message("Using CMAKE for boost")
endif()

list(TRANSFORM BOOST_INCLUDE_LIBRARIES PREPEND "Boost::" OUTPUT_VARIABLE BOOST_LIBS)
target_link_libraries(bb PRIVATE "${BOOST_LIBS}" nlohmann_json::nlohmann_json spdlog::spdlog)
