

target_sources(bb
    PRIVATE
        EnvReader.cpp
    PUBLIC FILE_SET HEADERS
        BASE_DIRS "${CMAKE_SOURCE_DIR}/src"
        FILES EnvReader.hpp
)