#include "imgui_view_adapter.h"
#include <cstring>
#include <iostream>

namespace QtMvvm {

ImGuiViewAdapter::ImGuiViewAdapter(const std::string &viewName)
    : m_viewName(viewName)
    , m_title(viewName)
    , m_visible(false)
    , m_enabled(true)
    , m_parentView(nullptr)
{
}

ImGuiViewAdapter::~ImGuiViewAdapter()
{
}

void ImGuiViewAdapter::show()
{
    m_visible = true;
    std::cout << "[ImGuiViewAdapter] Showing view: " << m_viewName << std::endl;
}

void ImGuiViewAdapter::hide()
{
    m_visible = false;
    std::cout << "[ImGuiViewAdapter] Hiding view: " << m_viewName << std::endl;
}

void ImGuiViewAdapter::close()
{
    m_visible = false;
    std::cout << "[ImGuiViewAdapter] Closing view: " << m_viewName << std::endl;
}

bool ImGuiViewAdapter::isVisible() const
{
    return m_visible;
}

void ImGuiViewAdapter::setTitle(const std::string &title)
{
    m_title = title;
}

std::string ImGuiViewAdapter::getTitle() const
{
    return m_title;
}

void ImGuiViewAdapter::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool ImGuiViewAdapter::isEnabled() const
{
    return m_enabled;
}

void ImGuiViewAdapter::setPresentingView(INativePresentingView *parent)
{
    m_parentView = parent;
}

INativePresentingView* ImGuiViewAdapter::getPresentingView() const
{
    return m_parentView;
}

void ImGuiViewAdapter::render()
{
    // Base implementation - override in derived classes
}

void ImGuiViewAdapter::setPropertyChangedCallback(std::function<void(const std::string&, const std::string&)> callback)
{
    m_propertyChangedCallback = callback;
}

void ImGuiViewAdapter::setCommandCallback(std::function<void(const std::string&)> callback)
{
    m_commandCallback = callback;
}

void ImGuiViewAdapter::renderProperties()
{
    // Helper for rendering property controls
    ImGui::Text("Properties:");
    ImGui::Separator();
}

void ImGuiViewAdapter::renderCommands()
{
    // Helper for rendering command buttons
    ImGui::Separator();
    ImGui::Text("Commands:");
}

// ===== ImGuiExampleView Implementation =====

ImGuiExampleView::ImGuiExampleView()
    : ImGuiViewAdapter("Example View")
    , m_counter(0)
    , m_active(false)
{
    std::memset(m_nameBuffer, 0, sizeof(m_nameBuffer));
    std::strcpy(m_nameBuffer, "");
}

void ImGuiExampleView::render()
{
    if (!isVisible()) {
        return;
    }
    
    ImGui::Begin(getTitle().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (!isEnabled()) {
        ImGui::BeginDisabled();
    }
    
    // Name property
    ImGui::Text("Name:");
    ImGui::SameLine();
    if (ImGui::InputText("##name", m_nameBuffer, sizeof(m_nameBuffer))) {
        // Property changed - notify ViewModel
        if (m_propertyChangedCallback) {
            m_propertyChangedCallback("name", std::string(m_nameBuffer));
        }
    }
    
    // Counter display (read-only from ViewModel)
    ImGui::Text("Counter: %d", m_counter);
    
    // Active checkbox
    if (ImGui::Checkbox("Active", &m_active)) {
        // Property changed - notify ViewModel
        if (m_propertyChangedCallback) {
            m_propertyChangedCallback("active", m_active ? "true" : "false");
        }
    }
    
    ImGui::Separator();
    
    // Command buttons
    if (ImGui::Button("Increment Counter")) {
        if (m_commandCallback) {
            m_commandCallback("increment");
        }
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Reset Counter")) {
        if (m_commandCallback) {
            m_commandCallback("reset");
        }
    }
    
    if (ImGui::Button("Show Message")) {
        if (m_commandCallback) {
            m_commandCallback("showMessage");
        }
    }
    
    if (!isEnabled()) {
        ImGui::EndDisabled();
    }
    
    ImGui::End();
}

void ImGuiExampleView::bindViewModel(std::shared_ptr<INativeViewModel> viewModel)
{
    m_viewModel = viewModel;
    
    if (m_viewModel) {
        // Set up property change callback from UI to ViewModel
        setPropertyChangedCallback([this](const std::string &property, const std::string &value) {
            std::cout << "[ImGuiExampleView] Property changed: " << property << " = " << value << std::endl;
            // In a real implementation, would call ViewModel setter methods
        });
        
        // Set up command callback
        setCommandCallback([this](const std::string &command) {
            std::cout << "[ImGuiExampleView] Command executed: " << command << std::endl;
            // In a real implementation, would call ViewModel command methods
        });
        
        // Initialize UI from ViewModel
        updateFromViewModel();
    }
}

void ImGuiExampleView::updateFromViewModel()
{
    if (!m_viewModel) {
        return;
    }
    
    // In a real implementation, would read properties from ViewModel
    // For example:
    // std::string name = viewModel->getName();
    // std::strncpy(m_nameBuffer, name.c_str(), sizeof(m_nameBuffer) - 1);
    // m_counter = viewModel->getCounter();
    // m_active = viewModel->isActive();
}

void ImGuiExampleView::updateToViewModel()
{
    if (!m_viewModel) {
        return;
    }
    
    // In a real implementation, would write properties to ViewModel
    // For example:
    // viewModel->setName(std::string(m_nameBuffer));
    // viewModel->setActive(m_active);
}

} // namespace QtMvvm
