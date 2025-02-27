cmake_minimum_required(VERSION 3.10)

project(VulkanEngine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch GLFW using FetchContent
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)

FetchContent_MakeAvailable(glfw)

# Fetch Vulkan-Headers using FetchContent
FetchContent_Declare(
    vulkan-headers
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG master
)

FetchContent_MakeAvailable(vulkan-headers)

# Include directories for your own headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include     # Add your include directory
)

# Add source files from your src directory
file(GLOB SOURCES
${CMAKE_SOURCE_DIR}/src/*.cpp  # Add all .cpp files in your src directory
)

# Add the executable
add_executable(VulkanEngine ${SOURCES} main.cpp)  # Include your source files along with main.cpp

# Link Vulkan headers
target_include_directories(VulkanEngine PRIVATE ${vulkan-headers_SOURCE_DIR}/include)

# Link GLFW
target_link_libraries(VulkanEngine PRIVATE glfw)

# Link Vulkan runtime (assuming it's installed system-wide)
find_package(Vulkan REQUIRED)
target_link_libraries(VulkanEngine PRIVATE Vulkan::Vulkan)

# Optionally: Run VulkanEngine after building
add_custom_command(TARGET VulkanEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running VulkanEngine..."
    COMMAND $<TARGET_FILE:VulkanEngine>
    COMMENT "Running the VulkanEngine executable"
)
