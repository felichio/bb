# find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(bb)

target_sources(
  bb
  PRIVATE
    main.cpp
)

target_link_libraries(bb PRIVATE Boost::asio Boost::beast)