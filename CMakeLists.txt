cmake_minimum_required(VERSION 3.16)
project(WAM01_VCU_Panel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Dependencies
# =========================
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# =========================
# Paths
# =========================
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rd_party)

# =========================
# ImGui library
# =========================
set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# =========================
# App sources
# =========================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(app ${SRC_FILES})

# =========================
# Includes (IMPORTANT: single source of truth)
# =========================
target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${THIRD_PARTY_DIR}
)

# =========================
# Linking
# =========================
target_link_libraries(app PRIVATE
    imgui
    glfw
    OpenGL::GL
)

# =========================
# Resources
# =========================
file(COPY src/resources DESTINATION ${CMAKE_BINARY_DIR})

# =========================
# Linux extras
# =========================
if(UNIX AND NOT APPLE)
    target_link_libraries(app PRIVATE dl pthread)
endif()

# =========================
# Compile DB for clangd
# =========================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
