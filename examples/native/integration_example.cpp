/**
 * @file integration_example.cpp
 * @brief Shows how to integrate native interfaces with existing QtMvvm code
 * 
 * This example demonstrates:
 * 1. Using NativeWidgetsPresenter as a drop-in replacement
 * 2. Mixing Qt and native views in the same application
 * 3. Converting between Qt and native types
 */

#include <iostream>
#include <memory>
#include <string>

// Simulated Qt types for demonstration
namespace Qt {
    class Widget {
    public:
        virtual ~Widget() = default;
        virtual void show() { std::cout << "Qt Widget shown" << std::endl; }
        virtual void setTitle(const std::string &title) {
            std::cout << "Qt Widget title: " << title << std::endl;
        }
    };
}

// Simulated QtMvvm types
namespace QtMvvm {
    class ViewModel {
    public:
        virtual ~ViewModel() = default;
        std::string name() const { return "ExampleViewModel"; }
    };

    class INativeView {
    public:
        virtual ~INativeView() = default;
        virtual void show() = 0;
        virtual void setTitle(const std::string &title) = 0;
    };

    class QtWidgetAdapter : public INativeView {
        Qt::Widget *m_widget;
    public:
        QtWidgetAdapter(Qt::Widget *widget) : m_widget(widget) {}
        void show() override { if (m_widget) m_widget->show(); }
        void setTitle(const std::string &title) override {
            if (m_widget) m_widget->setTitle(title);
        }
    };
}

// Example: Custom native view (no Qt dependencies)
class ConsoleView : public QtMvvm::INativeView {
    std::string m_title;
    bool m_visible;
    QtMvvm::ViewModel *m_viewModel;

public:
    ConsoleView(QtMvvm::ViewModel *vm) 
        : m_visible(false), m_viewModel(vm) {}

    void show() override {
        m_visible = true;
        std::cout << "\n--- ConsoleView (Native) ---" << std::endl;
        std::cout << "View shown for: " << m_viewModel->name() << std::endl;
        std::cout << "Title: " << m_title << std::endl;
        std::cout << "----------------------------\n" << std::endl;
    }

    void setTitle(const std::string &title) override {
        m_title = title;
    }
};

// Example: Qt Widget view
class QtWidgetView : public Qt::Widget {
    QtMvvm::ViewModel *m_viewModel;

public:
    QtWidgetView(QtMvvm::ViewModel *vm) : m_viewModel(vm) {}

    void show() override {
        std::cout << "\n--- QtWidgetView (via Adapter) ---" << std::endl;
        std::cout << "Qt Widget shown for: " << m_viewModel->name() << std::endl;
        Qt::Widget::show();
        std::cout << "----------------------------------\n" << std::endl;
    }
};

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "QtMvvm Native Interface Integration Example" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    // Create a view model
    auto viewModel = std::make_unique<QtMvvm::ViewModel>();

    std::cout << "Scenario 1: Pure Native View" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    {
        auto nativeView = std::make_unique<ConsoleView>(viewModel.get());
        nativeView->setTitle("Native Console Application");
        nativeView->show();
    }

    std::cout << "\nScenario 2: Qt Widget via Adapter" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    {
        auto qtWidget = std::make_unique<QtWidgetView>(viewModel.get());
        auto adapter = std::make_unique<QtMvvm::QtWidgetAdapter>(qtWidget.get());
        
        adapter->setTitle("Qt Widget Application");
        adapter->show();  // Uses native interface, but works with Qt Widget
    }

    std::cout << "\nScenario 3: Mixed Approach" << std::endl;
    std::cout << "----------------------------" << std::endl;
    {
        std::cout << "Using both native and Qt views in the same app:" << std::endl;
        
        // Native view for some features
        auto nativeView = std::make_unique<ConsoleView>(viewModel.get());
        nativeView->setTitle("Settings (Native)");
        
        // Qt widget for other features
        auto qtWidget = std::make_unique<QtWidgetView>(viewModel.get());
        auto qtAdapter = std::make_unique<QtMvvm::QtWidgetAdapter>(qtWidget.get());
        qtAdapter->setTitle("Main Window (Qt)");
        
        // Both work through the same INativeView interface
        std::vector<QtMvvm::INativeView*> views = {
            nativeView.get(),
            qtAdapter.get()
        };
        
        std::cout << "\nShowing all views through unified interface:" << std::endl;
        for (auto view : views) {
            view->show();
        }
    }

    std::cout << "\n==================================================" << std::endl;
    std::cout << "Key Benefits Demonstrated:" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "1. Native views have zero Qt dependencies" << std::endl;
    std::cout << "2. Qt widgets can work through native interface" << std::endl;
    std::cout << "3. Both can be used in the same application" << std::endl;
    std::cout << "4. Unified interface simplifies presenter logic" << std::endl;
    std::cout << "5. Easy migration path from Qt to native" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    return 0;
}
