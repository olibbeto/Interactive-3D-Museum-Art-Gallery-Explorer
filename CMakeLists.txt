cmake_minimum_required(VERSION 3.14)
project(Interactive3DMuseum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# We need OpenGL
find_package(OpenGL REQUIRED)

# Fetch FreeGLUT directly so you don't have to install it manually!
include(FetchContent)
set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # Use static to avoid DLL issues
set(FREEGLUT_REPLACE_VALID_TARGET ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    freeglut
    GIT_REPOSITORY https://github.com/FreeGLUTProject/freeglut.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(freeglut)

# Project sources
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)
# Include FreeGLUT directory dynamically
target_include_directories(${PROJECT_NAME} PRIVATE ${freeglut_SOURCE_DIR}/include)

# Link OpenGL and FreeGLUT
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} freeglut_static)
