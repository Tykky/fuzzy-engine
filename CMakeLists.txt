project(fuzzy-engine)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

foreach(FILE ${SOURCE_FILES})
    cmake_path(GET FILE FILENAME NAME)
    add_executable(${NAME} ${FILE})
endforeach()