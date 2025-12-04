#include <QApplication>
#include <QDebug>
#include <iostream>

// Include Native interface headers
#include <QtMvvmCore/nativeviewmodel.h>
#include <QtMvvmCore/viewmodeladapter.h>
#include <QtMvvmCore/nativepresenter.h>
#include <QtMvvmCore/presenteradapter.h>

// Include Qt integration headers
#include <QtMvvmWidgets/WidgetsPresenter>
#include <QtMvvmCore/ServiceRegistry>

#include "nativeexampleviewmodel.h"
#include "nativeexampleview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Native C++ ViewModel Integration Test" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    std::cout << "[Main] Initializing QtMvvm with Native C++ interfaces..." << std::endl;
    
    // Register the Qt Widget View (uses Q_INVOKABLE constructor)
    QtMvvm::WidgetsPresenter::registerView<NativeExampleView>();
    std::cout << "[Main] Registered NativeExampleView" << std::endl;
    
    // Create native ViewModel (pure C++)
    auto nativeViewModel = new NativeExampleViewModel();
    std::cout << "[Main] Created NativeExampleViewModel (pure C++ - no QObject)" << std::endl;
    
    // Wrap native ViewModel with adapter to make it work with Qt
    auto adapter = new QtMvvm::NativeToViewModelAdapter(nativeViewModel);
    std::cout << "[Main] Created adapter to bridge Native ViewModel to Qt" << std::endl;
    
    // Initialize the ViewModel with some parameters
    std::map<std::string, std::string> initParams;
    initParams["initialCounter"] = "10";
    initParams["name"] = "Test Native Integration";
    nativeViewModel->onInit(initParams);
    
    // Show the view using Qt presenter
    // The adapter makes the native ViewModel compatible with Qt's WidgetsPresenter
    try {
        QtMvvm::WidgetsPresenter::present(adapter);
        std::cout << "[Main] Successfully presented view with native ViewModel!" << std::endl;
        std::cout << "\n[Main] Application running..." << std::endl;
        std::cout << "[Main] Interact with the UI to test native interface integration" << std::endl;
        std::cout << "[Main] All property changes and commands go through pure C++ interface\n" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[Main] Error presenting view: " << e.what() << std::endl;
        return 1;
    }
    
    int result = app.exec();
    
    std::cout << "\n[Main] Application exiting..." << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    return result;
}
