#ifndef IMGUI_EXAMPLE_VIEWMODEL_H
#define IMGUI_EXAMPLE_VIEWMODEL_H

#include "nativeviewmodel.h"
#include <string>
#include <functional>

namespace QtMvvm {

/**
 * @brief Example ViewModel for ImGui demonstration
 * 
 * Pure C++ ViewModel that works with ImGui views through the adapter pattern.
 */
class ImGuiExampleViewModel : public NativeViewModelBase {
public:
    ImGuiExampleViewModel();
    virtual ~ImGuiExampleViewModel();
    
    // INativeViewModel interface
    void onInit(const std::map<std::string, std::string> &params) override;
    void onResult(int requestCode, const std::string &result) override;
    
    // Properties
    std::string getName() const { return m_name; }
    void setName(const std::string &name);
    
    int getCounter() const { return m_counter; }
    void setCounter(int counter);
    
    bool isActive() const { return m_active; }
    void setActive(bool active);
    
    // Commands
    void incrementCounter();
    void resetCounter();
    void showMessage();
    
    // Property change notification
    void setPropertyChangedCallback(std::function<void(const std::string&)> callback);
    
private:
    std::string m_name;
    int m_counter;
    bool m_active;
    
    std::function<void(const std::string&)> m_propertyChangedCallback;
    
    void notifyPropertyChanged(const std::string &propertyName);
};

} // namespace QtMvvm

#endif // IMGUI_EXAMPLE_VIEWMODEL_H
