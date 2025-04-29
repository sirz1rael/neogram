cmake_minimum_required(VERSION 3.22)
project(Neogram)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Ищем зависимости GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Добавляем include-директории
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        # Если нужно добавить другие пути, можно указать здесь
)

# Линкуем зависимости
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        PkgConfig::GTKMM  # Используем импортированный таргет
)

# Для старых версий CMake (если PkgConfig::GTKMM не работает):
# target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
# target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
# target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})
# target_compile_options(${PROJECT_NAME} PRIVATE ${GTKMM_CFLAGS_OTHER})