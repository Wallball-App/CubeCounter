cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CubeCounter VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

find_library(GMP_LIBRARY NAMES gmp libgmp REQUIRED)
find_library(GMPXX_LIBRARY NAMES gmpxx libgmpxx REQUIRED)
find_path(GMP_INCLUDE NAMES gmpxx.h REQUIRED)

find_package(OpenGL REQUIRED)

#find_package(gmp REQUIRED)

add_library(glad STATIC "source/glad/glad.c")
target_include_directories(glad PUBLIC include)

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS
    "source/*.cpp"
    "source/*.c"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/MainCLI.cpp")

file(COPY "shaders" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "Fonts" DESTINATION "${CMAKE_BINARY_DIR}")
add_executable(CubeCounter ${SOURCES})

target_compile_definitions(CubeCounter PRIVATE __GMP_WITHIN_GMP)
target_include_directories(CubeCounter PRIVATE ${GMP_INCLUDE})

target_link_libraries(CubeCounter PRIVATE 
    SDL3::SDL3
    glm::glm 
    glad 
    imgui::imgui 
    ${GMPXX_LIBRARY}
    ${GMP_LIBRARY}
    OpenGL::GL
)


add_executable(cubecounter-cli 
    "source/MainCLI.cpp"
    "source/Calculation.cpp"
    "include/Calculation.h")
target_compile_definitions(cubecounter-cli PRIVATE __GMP_WITHIN_GMP)
target_include_directories(cubecounter-cli PRIVATE ${GMP_INCLUDE})
target_include_directories(cubecounter-cli PRIVATE include)
target_link_libraries(cubecounter-cli PRIVATE 
    ${GMPXX_LIBRARY}
    ${GMP_LIBRARY}
)