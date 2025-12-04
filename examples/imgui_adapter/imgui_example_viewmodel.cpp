#include "imgui_example_viewmodel.h"
#include <iostream>

namespace QtMvvm {

ImGuiExampleViewModel::ImGuiExampleViewModel()
    : m_name("ImGui User")
    , m_counter(0)
    , m_active(false)
{
    std::cout << "[ImGuiExampleViewModel] Created" << std::endl;
}

ImGuiExampleViewModel::~ImGuiExampleViewModel()
{
    std::cout << "[ImGuiExampleViewModel] Destroyed" << std::endl;
}

void ImGuiExampleViewModel::onInit(const std::map<std::string, std::string> &params)
{
    std::cout << "[ImGuiExampleViewModel] onInit called with " << params.size() << " parameters" << std::endl;
    
    // Initialize from parameters
    auto nameIt = params.find("name");
    if (nameIt != params.end()) {
        setName(nameIt->second);
    }
    
    auto counterIt = params.find("counter");
    if (counterIt != params.end()) {
        try {
            setCounter(std::stoi(counterIt->second));
        } catch (...) {
            std::cerr << "[ImGuiExampleViewModel] Warning: Invalid counter value" << std::endl;
        }
    }
}

void ImGuiExampleViewModel::onResult(int requestCode, const std::string &result)
{
    std::cout << "[ImGuiExampleViewModel] onResult called: requestCode=" << requestCode 
              << ", result=" << result << std::endl;
}

void ImGuiExampleViewModel::setName(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        notifyPropertyChanged("name");
        std::cout << "[ImGuiExampleViewModel] Name changed to: " << m_name << std::endl;
    }
}

void ImGuiExampleViewModel::setCounter(int counter)
{
    if (m_counter != counter) {
        m_counter = counter;
        notifyPropertyChanged("counter");
        std::cout << "[ImGuiExampleViewModel] Counter changed to: " << m_counter << std::endl;
    }
}

void ImGuiExampleViewModel::setActive(bool active)
{
    if (m_active != active) {
        m_active = active;
        notifyPropertyChanged("active");
        std::cout << "[ImGuiExampleViewModel] Active changed to: " << (m_active ? "true" : "false") << std::endl;
    }
}

void ImGuiExampleViewModel::incrementCounter()
{
    std::cout << "[ImGuiExampleViewModel] Increment counter command" << std::endl;
    setCounter(m_counter + 1);
}

void ImGuiExampleViewModel::resetCounter()
{
    std::cout << "[ImGuiExampleViewModel] Reset counter command" << std::endl;
    setCounter(0);
}

void ImGuiExampleViewModel::showMessage()
{
    std::cout << "[ImGuiExampleViewModel] Show message command" << std::endl;
    
    // Emit result to show message
    emitResult("Message from ViewModel: Counter is " + std::to_string(m_counter));
}

void ImGuiExampleViewModel::setPropertyChangedCallback(std::function<void(const std::string&)> callback)
{
    m_propertyChangedCallback = callback;
}

void ImGuiExampleViewModel::notifyPropertyChanged(const std::string &propertyName)
{
    if (m_propertyChangedCallback) {
        m_propertyChangedCallback(propertyName);
    }
}

} // namespace QtMvvm
