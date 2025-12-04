#ifndef NATIVEEXAMPLEVIEWMODEL_H
#define NATIVEEXAMPLEVIEWMODEL_H

#include <QtMvvmCore/nativeviewmodel.h>
#include <string>
#include <map>
#include <functional>

// This ViewModel uses the NATIVE C++ interface (no QObject)
class NativeExampleViewModel : public QtMvvm::NativeViewModelBase
{
public:
    explicit NativeExampleViewModel();
    ~NativeExampleViewModel() override = default;

    // Native interface methods (no Qt types)
    void onInit(const std::map<std::string, std::string> &params) override;
    void onResult(int requestCode, const std::string &result) override;

    // Business logic methods
    std::string getName() const { return m_name; }
    void setName(const std::string &name);
    
    int getCounter() const { return m_counter; }
    void setCounter(int counter);
    
    bool isActive() const { return m_active; }
    void setActive(bool active);

    // Property change callbacks
    void setNameChangedCallback(std::function<void(const std::string&)> callback) {
        m_nameChangedCallback = callback;
    }
    
    void setCounterChangedCallback(std::function<void(int)> callback) {
        m_counterChangedCallback = callback;
    }
    
    void setActiveChangedCallback(std::function<void(bool)> callback) {
        m_activeChangedCallback = callback;
    }

    // Commands
    void incrementCounter();
    void resetCounter();
    void showMessage();
    void testProgress();

private:
    std::string m_name;
    int m_counter;
    bool m_active;
    
    // Callbacks for property changes
    std::function<void(const std::string&)> m_nameChangedCallback;
    std::function<void(int)> m_counterChangedCallback;
    std::function<void(bool)> m_activeChangedCallback;
};

#endif // NATIVEEXAMPLEVIEWMODEL_H
