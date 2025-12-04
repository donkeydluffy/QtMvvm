#include "imgui_presenter.h"
#include <iostream>
#include <algorithm>

namespace QtMvvm {

ImGuiPresenter::ImGuiPresenter()
{
    std::cout << "[ImGuiPresenter] Created" << std::endl;
}

ImGuiPresenter::~ImGuiPresenter()
{
    closeAll();
    
    // Clean up factories
    for (auto &pair : m_viewFactories) {
        delete pair.second;
    }
    m_viewFactories.clear();
    
    std::cout << "[ImGuiPresenter] Destroyed" << std::endl;
}

void ImGuiPresenter::present(INativeViewModel *viewModel,
                             const std::map<std::string, std::string> &params,
                             INativePresentingView *parent)
{
    if (!viewModel) {
        std::cerr << "[ImGuiPresenter] Error: Null ViewModel" << std::endl;
        return;
    }
    
    // Get ViewModel type name (in real implementation, would use RTTI or type registry)
    std::string viewModelName = "ExampleViewModel"; // Placeholder
    
    // Find factory for this ViewModel type
    INativeViewFactory *factory = findFactory(viewModelName);
    if (!factory) {
        std::cerr << "[ImGuiPresenter] Error: No factory registered for " << viewModelName << std::endl;
        return;
    }
    
    // Create view
    INativeView *view = factory->createView();
    if (!view) {
        std::cerr << "[ImGuiPresenter] Error: Factory returned null view" << std::endl;
        return;
    }
    
    // Cast to ImGui view
    ImGuiViewAdapter *imguiView = dynamic_cast<ImGuiViewAdapter*>(view);
    if (!imguiView) {
        std::cerr << "[ImGuiPresenter] Error: View is not an ImGuiViewAdapter" << std::endl;
        delete view;
        return;
    }
    
    // Set parent
    if (parent) {
        view->setPresentingView(parent);
    }
    
    // Initialize ViewModel
    viewModel->onInit(params);
    
    // Show view
    view->show();
    
    // Add to active views
    m_activeViews.push_back(std::shared_ptr<ImGuiViewAdapter>(imguiView));
    
    std::cout << "[ImGuiPresenter] Presented view for " << viewModelName << std::endl;
}

void ImGuiPresenter::showDialog(const NativeMessageConfig &config,
                               INativeMessageResult *result)
{
    std::cout << "[ImGuiPresenter] Showing dialog: " << config.title << std::endl;
    std::cout << "  Message: " << config.text << std::endl;
    std::cout << "  Type: " << static_cast<int>(config.type) << std::endl;
    
    // In a real implementation, would create an ImGui popup dialog
    // For now, just auto-complete
    if (result) {
        result->complete(0, ""); // Button index 0 (OK/Yes)
    }
}

void ImGuiPresenter::registerViewFactory(const std::string &viewModelName,
                                        INativeViewFactory *factory)
{
    if (m_viewFactories.find(viewModelName) != m_viewFactories.end()) {
        std::cerr << "[ImGuiPresenter] Warning: Overwriting factory for " << viewModelName << std::endl;
        delete m_viewFactories[viewModelName];
    }
    
    m_viewFactories[viewModelName] = factory;
    std::cout << "[ImGuiPresenter] Registered factory for " << viewModelName << std::endl;
}

void ImGuiPresenter::renderAll()
{
    // Render all active views
    for (auto &view : m_activeViews) {
        if (view && view->isVisible()) {
            view->render();
        }
    }
    
    // Remove closed views
    m_activeViews.erase(
        std::remove_if(m_activeViews.begin(), m_activeViews.end(),
                      [](const std::shared_ptr<ImGuiViewAdapter> &view) {
                          return !view || !view->isVisible();
                      }),
        m_activeViews.end()
    );
}

void ImGuiPresenter::closeAll()
{
    for (auto &view : m_activeViews) {
        if (view) {
            view->close();
        }
    }
    m_activeViews.clear();
}

INativeViewFactory* ImGuiPresenter::findFactory(const std::string &viewModelName)
{
    auto it = m_viewFactories.find(viewModelName);
    if (it != m_viewFactories.end()) {
        return it->second;
    }
    return nullptr;
}

// ===== ImGuiViewFactory Implementation =====

ImGuiViewFactory::ImGuiViewFactory(std::function<ImGuiViewAdapter*()> factoryFunc)
    : m_factoryFunc(factoryFunc)
{
}

INativeView* ImGuiViewFactory::createView()
{
    if (m_factoryFunc) {
        return m_factoryFunc();
    }
    return nullptr;
}

} // namespace QtMvvm
