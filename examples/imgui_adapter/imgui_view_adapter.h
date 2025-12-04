#ifndef IMGUI_VIEW_ADAPTER_H
#define IMGUI_VIEW_ADAPTER_H

#include "nativeview.h"
#include "nativeviewmodel.h"
#include <imgui.h>
#include <memory>
#include <string>
#include <functional>

namespace QtMvvm {

/**
 * @brief Adapter that implements INativeView for ImGui
 * 
 * This adapter bridges the pure C++ mvvmcore INativeView interface
 * with ImGui's immediate-mode rendering system.
 */
class ImGuiViewAdapter : public INativeView {
public:
    ImGuiViewAdapter(const std::string &viewName);
    virtual ~ImGuiViewAdapter();
    
    // INativeView interface implementation
    void show() override;
    void hide() override;
    void close() override;
    bool isVisible() const override;
    
    void setTitle(const std::string &title) override;
    std::string getTitle() const override;
    
    void setEnabled(bool enabled) override;
    bool isEnabled() const override;
    
    void setPresentingView(INativePresentingView *parent) override;
    INativePresentingView* getPresentingView() const override;
    
    // ImGui-specific rendering method
    virtual void render();
    
    // Property change callback registration
    void setPropertyChangedCallback(std::function<void(const std::string&, const std::string&)> callback);
    
    // Command execution callback
    void setCommandCallback(std::function<void(const std::string&)> callback);
    
protected:
    std::string m_viewName;
    std::string m_title;
    bool m_visible;
    bool m_enabled;
    INativePresentingView *m_parentView;
    
    std::function<void(const std::string&, const std::string&)> m_propertyChangedCallback;
    std::function<void(const std::string&)> m_commandCallback;
    
    // Helper methods for ImGui rendering
    void renderProperties();
    void renderCommands();
};

/**
 * @brief Example ImGui view with ViewModel integration
 */
class ImGuiExampleView : public ImGuiViewAdapter {
public:
    ImGuiExampleView();
    
    void render() override;
    
    // Bind to a ViewModel
    void bindViewModel(std::shared_ptr<INativeViewModel> viewModel);
    
private:
    std::shared_ptr<INativeViewModel> m_viewModel;
    
    // UI state
    char m_nameBuffer[256];
    int m_counter;
    bool m_active;
    
    // Update UI from ViewModel
    void updateFromViewModel();
    
    // Update ViewModel from UI
    void updateToViewModel();
};

} // namespace QtMvvm

#endif // IMGUI_VIEW_ADAPTER_H
