/**
 * @file message_example.cpp
 * @brief Demonstrates native message and progress control interfaces
 * 
 * This example shows how to use INativeMessageResult and INativeProgressControl
 * without Qt dependencies for dialogs and progress tracking.
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

// Simulated native interfaces for demonstration
namespace QtMvvm {

// Simple MessageResult implementation
class ConsoleMessageResult {
private:
    std::string m_result;
    bool m_hasResult;
    bool m_autoDelete;
    std::function<void(int)> m_completionCallback;

public:
    ConsoleMessageResult() 
        : m_result()
        , m_hasResult(false)
        , m_autoDelete(true) 
    {}

    bool hasResult() const { return m_hasResult; }
    
    std::string result() const { return m_result; }
    
    void setResult(const std::string &result) {
        m_result = result;
        m_hasResult = true;
    }
    
    bool autoDelete() const { return m_autoDelete; }
    
    void setAutoDelete(bool autoDelete) { m_autoDelete = autoDelete; }
    
    void complete(int button) {
        std::cout << "Message completed with button: " << button << std::endl;
        if (m_completionCallback) {
            m_completionCallback(button);
        }
    }
    
    void complete(int button, const std::string &result) {
        setResult(result);
        complete(button);
    }
    
    void discardMessage() {
        std::cout << "Message discarded" << std::endl;
    }
    
    void setCompletionCallback(std::function<void(int)> callback) {
        m_completionCallback = callback;
    }
};

// Simple ProgressControl implementation
class ConsoleProgressControl {
private:
    bool m_autoDelete;
    bool m_indeterminate;
    int m_minimum;
    int m_maximum;
    int m_progress;
    std::string m_label;
    std::function<void(int)> m_cancelCallback;
    std::function<void()> m_closeCallback;

public:
    ConsoleProgressControl()
        : m_autoDelete(true)
        , m_indeterminate(false)
        , m_minimum(0)
        , m_maximum(100)
        , m_progress(0)
        , m_label("Processing...")
    {}

    bool autoDelete() const { return m_autoDelete; }
    void setAutoDelete(bool autoDelete) { m_autoDelete = autoDelete; }
    
    bool isIndeterminate() const { return m_indeterminate; }
    void setIndeterminate(bool indeterminate) { 
        m_indeterminate = indeterminate;
        if (indeterminate) {
            std::cout << "Progress: [Indeterminate]" << std::endl;
        }
    }
    
    int minimum() const { return m_minimum; }
    void setMinimum(int minimum) { m_minimum = minimum; }
    
    int maximum() const { return m_maximum; }
    void setMaximum(int maximum) { m_maximum = maximum; }
    
    int progress() const { return m_progress; }
    
    void setProgress(int progress) {
        m_progress = progress;
        displayProgress();
    }
    
    void setProgress(double progressPercent) {
        int range = m_maximum - m_minimum;
        m_progress = m_minimum + static_cast<int>(progressPercent * range);
        displayProgress();
    }
    
    void close() {
        std::cout << "\nProgress dialog closed" << std::endl;
        if (m_closeCallback) {
            m_closeCallback();
        }
    }
    
    void updateLabel(const std::string &text) {
        m_label = text;
        std::cout << "\nLabel updated: " << m_label << std::endl;
        displayProgress();
    }
    
    void setCancelCallback(std::function<void(int)> callback) {
        m_cancelCallback = callback;
    }
    
    void setCloseCallback(std::function<void()> callback) {
        m_closeCallback = callback;
    }
    
    void simulateCancel(int button) {
        std::cout << "\nUser canceled with button: " << button << std::endl;
        if (m_cancelCallback) {
            m_cancelCallback(button);
        }
    }

private:
    void displayProgress() {
        if (m_indeterminate) {
            return;
        }
        
        int percentage = 0;
        if (m_maximum > m_minimum) {
            percentage = ((m_progress - m_minimum) * 100) / (m_maximum - m_minimum);
        }
        
        std::cout << "\r" << m_label << " [";
        int barWidth = 30;
        int pos = (barWidth * percentage) / 100;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << percentage << "%" << std::flush;
    }
};

} // namespace QtMvvm

int main() {
    using namespace QtMvvm;

    std::cout << "==================================================" << std::endl;
    std::cout << "Native Message and Progress Example" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    // Example 1: Message Result
    std::cout << "Example 1: Message Result" << std::endl;
    std::cout << "-------------------------" << std::endl;
    
    auto messageResult = std::make_unique<ConsoleMessageResult>();
    
    messageResult->setCompletionCallback([](int button) {
        std::cout << "Completion callback invoked for button: " << button << std::endl;
    });
    
    // Simulate user input
    std::cout << "Simulating user entering text: 'Hello Native!'" << std::endl;
    messageResult->setResult("Hello Native!");
    
    // Simulate completion with OK button (button code 1)
    messageResult->complete(1);
    
    std::cout << "Result: " << messageResult->result() << std::endl;
    std::cout << "Has result: " << (messageResult->hasResult() ? "Yes" : "No") << std::endl;

    // Example 2: Progress Control
    std::cout << "\n\nExample 2: Progress Control" << std::endl;
    std::cout << "----------------------------" << std::endl;
    
    auto progressControl = std::make_unique<ConsoleProgressControl>();
    
    progressControl->setCloseCallback([]() {
        std::cout << "\nProgress close callback invoked" << std::endl;
    });
    
    progressControl->setCancelCallback([](int button) {
        std::cout << "Progress cancel callback invoked with button: " << button << std::endl;
    });
    
    std::cout << "Starting progress..." << std::endl;
    progressControl->updateLabel("Loading data...");
    
    // Simulate progress updates
    for (int i = 0; i <= 100; i += 20) {
        progressControl->setProgress(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << std::endl;
    progressControl->updateLabel("Processing complete!");
    progressControl->close();

    // Example 3: Indeterminate Progress
    std::cout << "\n\nExample 3: Indeterminate Progress" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    auto indeterminateProgress = std::make_unique<ConsoleProgressControl>();
    indeterminateProgress->setIndeterminate(true);
    indeterminateProgress->updateLabel("Connecting to server...");
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::cout << "Switching to determinate..." << std::endl;
    indeterminateProgress->setIndeterminate(false);
    indeterminateProgress->updateLabel("Downloading...");
    
    for (int i = 0; i <= 100; i += 10) {
        indeterminateProgress->setProgress(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << std::endl;
    indeterminateProgress->close();

    // Example 4: Progress with Cancel
    std::cout << "\n\nExample 4: Progress with Cancel" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    
    auto cancelableProgress = std::make_unique<ConsoleProgressControl>();
    cancelableProgress->updateLabel("Long operation...");
    
    // Simulate user canceling at 60%
    for (int i = 0; i <= 60; i += 20) {
        cancelableProgress->setProgress(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << std::endl;
    std::cout << "User canceled the operation!" << std::endl;
    cancelableProgress->simulateCancel(2); // Cancel button code
    cancelableProgress->close();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "Key Benefits Demonstrated:" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "1. Message results without Qt dependencies" << std::endl;
    std::cout << "2. Progress tracking with pure C++ callbacks" << std::endl;
    std::cout << "3. Indeterminate and determinate progress modes" << std::endl;
    std::cout << "4. Cancel and close notifications via callbacks" << std::endl;
    std::cout << "5. No QObject or Qt signals required" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    return 0;
}
