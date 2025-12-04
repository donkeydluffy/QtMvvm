/**
 * @file viewmodel_binding_example.cpp
 * @brief Demonstrates native ViewModel and Binding interfaces
 * 
 * This example shows how to use INativeViewModel and INativeBinding
 * without Qt dependencies in the business logic layer.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Simulated native interfaces for demonstration
namespace QtMvvm {

// Simple property implementation for demonstration
class SimpleProperty {
private:
    std::string m_name;
    std::string m_value;
    std::vector<std::function<void(const std::string&)>> m_callbacks;

public:
    SimpleProperty(const std::string &name, const std::string &initialValue = "")
        : m_name(name), m_value(initialValue) {}

    std::string name() const { return m_name; }
    
    std::string getValue() const { return m_value; }
    
    void setValue(const std::string &value) {
        if (m_value != value) {
            m_value = value;
            // Notify all callbacks
            for (const auto &callback : m_callbacks) {
                callback(m_value);
            }
        }
    }
    
    void registerChangeCallback(std::function<void(const std::string&)> callback) {
        m_callbacks.push_back(callback);
    }
};

// Simple ViewModel implementation
class UserViewModel {
private:
    SimpleProperty m_name;
    SimpleProperty m_email;
    std::function<void(const std::string&)> m_resultCallback;

public:
    UserViewModel()
        : m_name("name", "John Doe")
        , m_email("email", "john@example.com")
    {}

    SimpleProperty& nameProperty() { return m_name; }
    SimpleProperty& emailProperty() { return m_email; }

    void onInit(const std::map<std::string, std::string> &params) {
        std::cout << "UserViewModel::onInit called" << std::endl;
        for (const auto &param : params) {
            std::cout << "  " << param.first << " = " << param.second << std::endl;
        }
    }

    void setResultCallback(std::function<void(const std::string&)> callback) {
        m_resultCallback = callback;
    }

    void saveUser() {
        std::cout << "\nSaving user:" << std::endl;
        std::cout << "  Name: " << m_name.getValue() << std::endl;
        std::cout << "  Email: " << m_email.getValue() << std::endl;
        
        if (m_resultCallback) {
            m_resultCallback("User saved successfully");
        }
    }
};

// Simple View implementation
class UserView {
private:
    SimpleProperty m_nameInput;
    SimpleProperty m_emailInput;

public:
    UserView()
        : m_nameInput("nameInput")
        , m_emailInput("emailInput")
    {}

    SimpleProperty& nameInputProperty() { return m_nameInput; }
    SimpleProperty& emailInputProperty() { return m_emailInput; }

    void show() {
        std::cout << "\nView displayed:" << std::endl;
        std::cout << "  Name input: " << m_nameInput.getValue() << std::endl;
        std::cout << "  Email input: " << m_emailInput.getValue() << std::endl;
    }

    void simulateUserInput(const std::string &name, const std::string &email) {
        std::cout << "\nUser editing fields..." << std::endl;
        m_nameInput.setValue(name);
        m_emailInput.setValue(email);
    }
};

// Simple Binding implementation
class SimpleBinding {
private:
    SimpleProperty *m_source;
    SimpleProperty *m_target;
    bool m_twoWay;
    bool m_updating;

public:
    SimpleBinding(SimpleProperty *source, SimpleProperty *target, bool twoWay = true)
        : m_source(source), m_target(target), m_twoWay(twoWay), m_updating(false)
    {
        // Initialize target with source value
        m_target->setValue(m_source->getValue());

        // Set up source to target propagation
        m_source->registerChangeCallback([this](const std::string &newValue) {
            if (!m_updating) {
                m_updating = true;
                m_target->setValue(newValue);
                std::cout << "  Binding: " << m_source->name() 
                         << " -> " << m_target->name() 
                         << " (value: " << newValue << ")" << std::endl;
                m_updating = false;
            }
        });

        // Set up target to source propagation (if two-way)
        if (m_twoWay) {
            m_target->registerChangeCallback([this](const std::string &newValue) {
                if (!m_updating) {
                    m_updating = true;
                    m_source->setValue(newValue);
                    std::cout << "  Binding: " << m_target->name() 
                             << " -> " << m_source->name() 
                             << " (value: " << newValue << ")" << std::endl;
                    m_updating = false;
                }
            });
        }
    }
};

} // namespace QtMvvm

int main() {
    using namespace QtMvvm;

    std::cout << "==================================================" << std::endl;
    std::cout << "Native ViewModel and Binding Example" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    // Create ViewModel and View
    auto viewModel = std::make_unique<UserViewModel>();
    auto view = std::make_unique<UserView>();

    std::cout << "Step 1: Initialize ViewModel" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::map<std::string, std::string> initParams = {
        {"mode", "edit"},
        {"userId", "123"}
    };
    viewModel->onInit(initParams);

    std::cout << "\nStep 2: Set up data bindings" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Creating two-way bindings between ViewModel and View\n" << std::endl;
    
    // Create two-way bindings
    auto nameBinding = std::make_unique<SimpleBinding>(
        &viewModel->nameProperty(),
        &view->nameInputProperty(),
        true  // two-way
    );

    auto emailBinding = std::make_unique<SimpleBinding>(
        &viewModel->emailProperty(),
        &view->emailInputProperty(),
        true  // two-way
    );

    std::cout << "\nStep 3: Display view" << std::endl;
    std::cout << "----------------------------" << std::endl;
    view->show();

    std::cout << "\nStep 4: Simulate user editing in view" << std::endl;
    std::cout << "----------------------------" << std::endl;
    view->simulateUserInput("Jane Smith", "jane@example.com");

    std::cout << "\nStep 5: Check ViewModel updated via binding" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "ViewModel name: " << viewModel->nameProperty().getValue() << std::endl;
    std::cout << "ViewModel email: " << viewModel->emailProperty().getValue() << std::endl;

    std::cout << "\nStep 6: Update ViewModel programmatically" << std::endl;
    std::cout << "----------------------------" << std::endl;
    viewModel->nameProperty().setValue("Bob Johnson");
    viewModel->emailProperty().setValue("bob@example.com");

    std::cout << "\nStep 7: Check View updated via binding" << std::endl;
    std::cout << "----------------------------" << std::endl;
    view->show();

    std::cout << "\nStep 8: Save and get result" << std::endl;
    std::cout << "----------------------------" << std::endl;
    viewModel->setResultCallback([](const std::string &result) {
        std::cout << "Result received: " << result << std::endl;
    });
    viewModel->saveUser();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "Key Benefits Demonstrated:" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "1. ViewModel independent of Qt (pure C++)" << std::endl;
    std::cout << "2. Two-way data binding with callbacks" << std::endl;
    std::cout << "3. View and ViewModel communicate through bindings" << std::endl;
    std::cout << "4. Result notification via std::function callbacks" << std::endl;
    std::cout << "5. No Qt signals/slots or QObject required" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    return 0;
}
