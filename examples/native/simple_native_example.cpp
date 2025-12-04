/**
 * @file simple_native_example.cpp
 * @brief Demonstrates the native C++ interface of QtMvvm
 * 
 * This example shows how to implement a simple view and presenter
 * using pure C++ interfaces without Qt dependencies in the UI layer.
 */

#include <iostream>
#include <string>
#include <memory>

// These would typically be in your native interface headers
// For demonstration, we define minimal interfaces here

namespace NativeExample {

//! Simple native view interface
class ISimpleView {
public:
    virtual ~ISimpleView() = default;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void setTitle(const std::string &title) = 0;
    virtual void displayMessage(const std::string &message) = 0;
};

//! Console-based implementation of a native view
class ConsoleView : public ISimpleView {
private:
    std::string m_title;
    bool m_visible;

public:
    ConsoleView() : m_visible(false) {}

    void show() override {
        m_visible = true;
        std::cout << "\n=== View Shown ===" << std::endl;
        if (!m_title.empty()) {
            std::cout << "Title: " << m_title << std::endl;
        }
        std::cout << "==================\n" << std::endl;
    }

    void hide() override {
        m_visible = false;
        std::cout << "\n=== View Hidden ===\n" << std::endl;
    }

    void setTitle(const std::string &title) override {
        m_title = title;
    }

    void displayMessage(const std::string &message) override {
        if (m_visible) {
            std::cout << "Message: " << message << std::endl;
        }
    }
};

//! Simple view model (conceptual - would extend QtMvvm::ViewModel in real code)
class SimpleViewModel {
private:
    std::string m_title;
    std::string m_message;

public:
    SimpleViewModel(const std::string &title, const std::string &message)
        : m_title(title), m_message(message) {}

    std::string title() const { return m_title; }
    std::string message() const { return m_message; }
    
    void setMessage(const std::string &message) {
        m_message = message;
    }
};

//! Simple native presenter
class SimplePresenter {
public:
    void present(SimpleViewModel *viewModel, ISimpleView *view) {
        if (!viewModel || !view) {
            return;
        }

        // Initialize view with view model data
        view->setTitle(viewModel->title());
        view->show();
        view->displayMessage(viewModel->message());
    }
};

} // namespace NativeExample

//! Entry point demonstrating the native interface
int main() {
    using namespace NativeExample;

    std::cout << "QtMvvm Native Interface Example" << std::endl;
    std::cout << "================================\n" << std::endl;

    // Create a view model (business logic layer)
    auto viewModel = std::make_unique<SimpleViewModel>(
        "Welcome to Native QtMvvm",
        "This is a pure C++ example without Qt UI dependencies!"
    );

    // Create a native view (UI layer - console in this case)
    auto view = std::make_unique<ConsoleView>();

    // Create a presenter to connect them
    SimplePresenter presenter;

    // Present the view model
    presenter.present(viewModel.get(), view.get());

    // Simulate some interaction
    std::cout << "\n--- Simulating interaction ---\n" << std::endl;
    viewModel->setMessage("Message updated from ViewModel!");
    view->displayMessage(viewModel->message());

    // Hide the view
    view->hide();

    std::cout << "\nKey Benefits of Native Interface:" << std::endl;
    std::cout << "1. No Qt dependencies in the UI layer" << std::endl;
    std::cout << "2. Easy to port to different platforms" << std::endl;
    std::cout << "3. Can use any C++ UI framework" << std::endl;
    std::cout << "4. Pure C++ interfaces are testable" << std::endl;
    std::cout << "5. Smaller binary size without Qt" << std::endl;

    return 0;
}
