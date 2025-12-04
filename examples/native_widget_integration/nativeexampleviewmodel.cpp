#include "nativeexampleviewmodel.h"
#include <iostream>

NativeExampleViewModel::NativeExampleViewModel()
    : m_name("Native ViewModel")
    , m_counter(0)
    , m_active(true)
{
}

void NativeExampleViewModel::onInit(const std::map<std::string, std::string> &params)
{
    // Initialize from parameters (pure C++ map)
    auto it = params.find("initialCounter");
    if (it != params.end()) {
        try {
            m_counter = std::stoi(it->second);
        } catch (...) {
            m_counter = 0;
        }
    }
    
    it = params.find("name");
    if (it != params.end()) {
        m_name = it->second;
    }
    
    std::cout << "[Native] ViewModel initialized with counter=" << m_counter 
              << ", name=" << m_name << std::endl;
}

void NativeExampleViewModel::onResult(int requestCode, const std::string &result)
{
    std::cout << "[Native] Received result: code=" << requestCode 
              << ", result=" << result << std::endl;
    
    // Handle results from dialogs, etc.
    if (requestCode == 1) {
        setName(result);
    }
}

void NativeExampleViewModel::setName(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        std::cout << "[Native] Name changed to: " << m_name << std::endl;
        
        if (m_nameChangedCallback) {
            m_nameChangedCallback(m_name);
        }
    }
}

void NativeExampleViewModel::setCounter(int counter)
{
    if (m_counter != counter) {
        m_counter = counter;
        std::cout << "[Native] Counter changed to: " << m_counter << std::endl;
        
        if (m_counterChangedCallback) {
            m_counterChangedCallback(m_counter);
        }
    }
}

void NativeExampleViewModel::setActive(bool active)
{
    if (m_active != active) {
        m_active = active;
        std::cout << "[Native] Active changed to: " << (m_active ? "true" : "false") << std::endl;
        
        if (m_activeChangedCallback) {
            m_activeChangedCallback(m_active);
        }
    }
}

void NativeExampleViewModel::incrementCounter()
{
    setCounter(m_counter + 1);
}

void NativeExampleViewModel::resetCounter()
{
    setCounter(0);
}

void NativeExampleViewModel::showMessage()
{
    std::cout << "[Native] Current state: name=" << m_name 
              << ", counter=" << m_counter 
              << ", active=" << (m_active ? "true" : "false") << std::endl;
    
    // Emit a result to demonstrate callback mechanism
    emitResult("Message shown: " + m_name + ", counter=" + std::to_string(m_counter));
}

void NativeExampleViewModel::testProgress()
{
    std::cout << "[Native] Testing progress..." << std::endl;
    // In a real app, this would trigger a long-running operation
}
