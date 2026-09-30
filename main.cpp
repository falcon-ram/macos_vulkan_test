#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class HelloTriangleApplication {
    public:
        void run() {
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        GLFWwindow* window;
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;

        void initWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        }

        void initVulkan() {
            createInstance();
        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            };
        }

        void cleanup() {
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        void createInstance() {
            constexpr vk::ApplicationInfo appInfo{
                "Hello Triangle",
                VK_MAKE_VERSION(1, 0, 0),
                "No Engine",
                VK_MAKE_VERSION(1, 0, 0),
                vk::ApiVersion14
            };

            vk::InstanceCreateInfo createInfo;
            createInfo.pApplicationInfo = &appInfo;
            
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            auto extensionProperties = context.enumerateInstanceExtensionProperties();
            for (uint32_t i = 0; i < glfwExtensionCount; i++) {
                if (std::ranges::none_of(extensionProperties,
                             [glfwExtension = glfwExtensions[i]](auto const& extensionProperty)
                             { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
                {
                    throw std::runtime_error("Required GLFW extension not supported: " + std::string(glfwExtensions[i]));
                }
            }

            // CRITICAL FOR MACOS/MOLTENVK:
            // 1. Add the Portability Enumeration extension
            glfwExtensions[glfwExtensionCount++] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
            // 2. Add Get Physical Device Properties 2 extension
            glfwExtensions[glfwExtensionCount++] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
            
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            // 3. Set the Portability Flag
            createInfo.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;

            try {
                instance = vk::raii::Instance(context, createInfo);
            } catch (const vk::SystemError& err) {
                std::cerr << "Failed to create Vulkan instance: " << err.what() << std::endl;
                return;
            } catch (const std::exception& err) {
                std::cerr << "Failed to create Vulkan instance: " << err.what() << std::endl;
                return;
            } catch (...) {
                std::cerr << "Failed to create Vulkan instance: Unknown error" << std::endl;
                return;
            }

            std::cout << "Vulkan instance created successfully!" << std::endl;
        }

};

int main() {
    try {
        HelloTriangleApplication app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

