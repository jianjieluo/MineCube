cmake_minimum_required (VERSION 3.8)
project(MineCube CXX)

set(CMAKE_CXX_STANDARD 11)

# OS config
if (WIN32)
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
endif()
if(NOT WIN32)
    find_package(OpenGL REQUIRED)
endif()

# config glfw
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory(3rd_party/glfw)
include_directories(3rd_party/glfw/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

# config glad
set(GLAD_SRC 3rd_party/glad/src/glad.c)
if (NOT WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(3rd_party/glad/include)

# config nlohmann::json
include_directories(3rd_party/nlohmann)

# config glm
include_directories(3rd_party/glm)

# config imgui
include_directories(3rd_party/imgui)
include_directories(3rd_party/imgui/examples/opengl3_example)
aux_source_directory(3rd_party/imgui IMGUI_SRC)

# include our headers
include_directories(include)

# get MineCube src files
aux_source_directory(./src MINECUBE_SRC)

set(SOURCE_FILES ${MINECUBE_SRC} ${IMGUI_SRC} ${GLAD_SRC})

# Compile
add_executable(MineCube ${SOURCE_FILES})

# Link
target_link_libraries(MineCube ${LIBS})