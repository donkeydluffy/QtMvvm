#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui_view_adapter.h"
#include "imgui_presenter.h"
#include "imgui_example_viewmodel.h"

using namespace QtMvvm;

/**
 * @brief Complete ImGui MVVM application demonstrating mvvmcore integration
 * 
 * This example shows:
 * - Pure C++ ViewModel (no Qt)
 * - ImGui view adapter implementing INativeView
 * - ImGui presenter managing view lifecycle
 * - Property binding and command execution
 * - Result callbacks
 */

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char**)
{
    std::cout << "=== ImGui MVVM Example ===" << std::endl;
    std::cout << "Demonstrating mvvmcore with ImGui immediate-mode GUI" << std::endl;
    std::cout << std::endl;
    
    // Setup GLFW window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }
    
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui MVVM Example - mvvmcore Integration", NULL, NULL);
    if (window == NULL) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    // Create MVVM components
    auto presenter = std::make_shared<ImGuiPresenter>();
    auto viewModel = std::make_shared<ImGuiExampleViewModel>();
    auto view = std::make_shared<ImGuiExampleView>();
    
    // Register view factory
    ImGuiViewRegistrar<ImGuiExampleView>::registerView(presenter.get(), "ExampleViewModel");
    
    // Bind view to ViewModel
    view->bindViewModel(viewModel);
    
    // Show the view
    view->show();
    
    // Initialize ViewModel
    std::map<std::string, std::string> params;
    params["name"] = "ImGui User";
    params["counter"] = "0";
    viewModel->onInit(params);
    
    std::cout << "Application initialized. Starting render loop..." << std::endl;
    std::cout << std::endl;
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll events
        glfwPollEvents();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Render our view
        view->render();
        
        // Show demo window
        static bool show_demo = true;
        if (show_demo) {
            ImGui::ShowDemoWindow(&show_demo);
        }
        
        // Show info window
        ImGui::Begin("Info");
        ImGui::Text("mvvmcore ImGui Integration");
        ImGui::Separator();
        ImGui::Text("This demonstrates a pure C++ ViewModel");
        ImGui::Text("driving an ImGui immediate-mode GUI.");
        ImGui::Separator();
        ImGui::Text("Architecture:");
        ImGui::BulletText("mvvmcore: Pure C++ MVVM framework");
        ImGui::BulletText("ImGuiViewAdapter: INativeView implementation");
        ImGui::BulletText("ImGuiPresenter: View lifecycle manager");
        ImGui::BulletText("ImGuiExampleViewModel: Pure C++ ViewModel");
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                   1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }
    
    std::cout << std::endl;
    std::cout << "Application shutting down..." << std::endl;
    
    // Cleanup
    view->close();
    presenter->closeAll();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "=== ImGui MVVM Example Complete ===" << std::endl;
    
    return 0;
}
