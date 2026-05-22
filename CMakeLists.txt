cmake_minimum_required(VERSION 3.10.0)

option(BUILD_GUI "Build the GUI version of the application" OFF)
option(OPEN_CMD "Open the command line interface with GUI" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(CubeCounter VERSION 0.1.0 LANGUAGES C CXX)

find_library(GMP_LIBRARY NAMES gmp libgmp REQUIRED)
find_library(GMPXX_LIBRARY NAMES gmpxx libgmpxx REQUIRED)
find_path(GMP_INCLUDE NAMES gmpxx.h REQUIRED)

if(BUILD_GUI) 
    find_package(SDL3 CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)

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
    if(OPEN_CMD)
        add_executable(CubeCounter ${SOURCES})
    else()
        add_executable(CubeCounter WIN32 MACOSX_BUNDLE ${SOURCES})
    endif()


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
endif()

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