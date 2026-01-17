# specify the needed boost libs inside BOOST_INCLUDE_LIBRARIES
set(BOOST_INCLUDE_LIBRARIES beast asio)

add_subdirectory(external/boost EXCLUDE_FROM_ALL)