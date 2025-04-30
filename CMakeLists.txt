cmake_minimum_required(VERSION 3.22)
project(Neogram)

set(DCMAKE_CXX_STANDARD 17)
set(DCMAKE_CXX_STANDARD_REQUIRED ON)
set(DCMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++ -lc++abi")
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(Td 1.8.47 REQUIRED COMPONENTS 
    tdjson 
    tdcore
    tdclient
    tdactor
    tdutils
)

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
        Td::tdjson
        Td::tdcore
        Td::tdutils
        Td::tdactor
        Td::tdclient
        pthread
        dl
        ssl
        crypto
        z
)