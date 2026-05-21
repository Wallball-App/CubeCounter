#[[cmake_minimum_required(VERSION 3.10.0)

project(CubeCounter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.8
)
FetchContent_MakeAvailable(imgui)
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS "${imgui_SOURCE_DIR}/*.cpp") 
add_library(imgui STATIC 
    ${IMGUI_SOURCES}
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp" 
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp" 
)

#[[FetchContent_Declare(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.91.0-1
)
FetchContent_MakeAvailable(boost)

add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC include)

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS
    "source/*.cpp"
    ".c"
    ".cpp"
)
add_executable(CubeCounter ${SOURCES})

target_include_directories(CubeCounter PRIVATE "C:/msys64/ucrt64/include")
target_link_directories(CubeCounter PRIVATE "C:/msys64/ucrt64/lib")
target_link_libraries(CubeCounter PRIVATE 
    SDL3::SDL3
    glm::glm 
    glad 
    imgui 
    gmpxx 
    gmp 
    opengl32
)
target_include_directories(CubeCounter PRIVATE include)
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}" "${imgui_SOURCE_DIR}/backends")
target_link_libraries(imgui PUBLIC SDL3::SDL3)

include(CTest)


enable_testing()]]
cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "" FORCE)
set(VCPKG_HOST_TRIPLET "x64-mingw-dynamic" CACHE STRING "" FORCE)

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