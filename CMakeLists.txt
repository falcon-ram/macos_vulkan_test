cmake_minimum_required(VERSION 3.20)
project(VulkanMacTutorial)

set(CMAKE_CXX_STANDARD 20) # Vulkan tutorials typically target C++20

# Set the path to your Vulkan SDK install directory if not found automatically
set(VULKAN_SDK "$ENV{HOME}/VulkanSDK/1.4.357.1/macOS") # Update with your exact version

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(VulkanApp main.cpp)

target_include_directories(VulkanApp PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    /opt/homebrew/include
)

target_link_libraries(VulkanApp PRIVATE 
    Vulkan::Vulkan 
    glfw
)
