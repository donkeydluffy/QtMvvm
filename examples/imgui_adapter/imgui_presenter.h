#ifndef IMGUI_PRESENTER_H
#define IMGUI_PRESENTER_H

#include "nativepresenter.h"
#include "imgui_view_adapter.h"
#include <map>
#include <memory>
#include <string>

namespace QtMvvm {

/**
 * @brief Presenter implementation for ImGui
 * 
 * Manages ImGui views and their lifecycle within the MVVM pattern.
 */
class ImGuiPresenter : public NativePresenterBase {
public:
    ImGuiPresenter();
    virtual ~ImGuiPresenter();
    
    // INativePresenter interface implementation
    void present(INativeViewModel *viewModel,
                const std::map<std::string, std::string> &params,
                INativePresentingView *parent) override;
    
    void showDialog(const NativeMessageConfig &config,
                   INativeMessageResult *result) override;
    
    void registerViewFactory(const std::string &viewModelName,
                            INativeViewFactory *factory) override;
    
    // ImGui-specific methods
    void renderAll();
    void closeAll();
    
private:
    std::map<std::string, INativeViewFactory*> m_viewFactories;
    std::vector<std::shared_ptr<ImGuiViewAdapter>> m_activeViews;
    
    // Find factory for ViewModel
    INativeViewFactory* findFactory(const std::string &viewModelName);
};

/**
 * @brief View factory for ImGui views
 */
class ImGuiViewFactory : public INativeViewFactory {
public:
    ImGuiViewFactory(std::function<ImGuiViewAdapter*()> factoryFunc);
    
    INativeView* createView() override;
    
private:
    std::function<ImGuiViewAdapter*()> m_factoryFunc;
};

/**
 * @brief Helper class to register ImGui views
 */
template<typename TView>
class ImGuiViewRegistrar {
public:
    static void registerView(ImGuiPresenter *presenter, const std::string &viewModelName) {
        auto factory = new ImGuiViewFactory([]() -> ImGuiViewAdapter* {
            return new TView();
        });
        presenter->registerViewFactory(viewModelName, factory);
    }
};

} // namespace QtMvvm

#endif // IMGUI_PRESENTER_H
