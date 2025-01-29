cmake_minimum_required(VERSION 3.14)
project(cpp-project CXX)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(cpp-project ${SRC_FILES})
target_include_directories(cpp-project PUBLIC "${CMAKE_SOURCE_DIR}/include")
set_target_properties(cpp-project PROPERTIES CXX_STANDARD 23)

# Imgui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/third_party/imgui")
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
target_sources(cpp-project PRIVATE ${IMGUI_SOURCES})
target_include_directories(cpp-project PRIVATE
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(cpp-project PRIVATE glfw OpenGL::GL)