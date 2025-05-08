cmake_minimum_required(VERSION 3.22)
project(Neogram)

set(DCMAKE_CXX_STANDARD 17)
set(DCMAKE_CXX_STANDARD_REQUIRED ON)
set(DCMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++ -lc++abi")
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

find_package(Td 1.8.48 REQUIRED)

find_package(CURL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)


add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        PkgConfig::GTKMM 
    PRIVATE
        Td::TdStatic
        pthread
        dl
        ssl
        crypto
        z
    PRIVATE
        ${CURL_LIBRARIES}
)
