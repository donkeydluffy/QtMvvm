#ifndef ANDROID_VIEW_ADAPTER_H
#define ANDROID_VIEW_ADAPTER_H

#include "nativeview.h"
#include "nativeviewmodel.h"
#include <jni.h>
#include <memory>
#include <string>
#include <functional>

namespace QtMvvm {

/**
 * @brief Adapter that implements INativeView for Android
 * 
 * This adapter bridges the pure C++ mvvmcore INativeView interface
 * with Android's Java Activity/View system through JNI.
 */
class AndroidViewAdapter : public INativeView {
public:
    AndroidViewAdapter(JNIEnv *env, jobject activity);
    virtual ~AndroidViewAdapter();
    
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
    
    // JNI-specific methods
    void setJNIEnvironment(JNIEnv *env);
    JNIEnv* getJNIEnvironment() const;
    
    jobject getActivity() const;
    
    // Property change notification to Java
    void notifyPropertyChanged(const std::string &propertyName, const std::string &value);
    
    // Update UI element from C++
    void updateTextView(const std::string &viewId, const std::string &text);
    void updateButton(const std::string &viewId, bool enabled);
    void updateCheckBox(const std::string &viewId, bool checked);
    
protected:
    JNIEnv *m_env;
    jobject m_activity;
    jclass m_activityClass;
    
    std::string m_title;
    bool m_visible;
    bool m_enabled;
    INativePresentingView *m_parentView;
    
    // JNI method IDs (cached for performance)
    jmethodID m_setTitleMethod;
    jmethodID m_showMethod;
    jmethodID m_hideMethod;
    jmethodID m_updateTextViewMethod;
    jmethodID m_updateButtonMethod;
    jmethodID m_updateCheckBoxMethod;
    
    // Initialize JNI method IDs
    void initializeJNIMethods();
    
    // Helper to call Java methods safely
    void callVoidMethod(jmethodID methodID, ...);
    std::string callStringMethod(jmethodID methodID, ...);
};

/**
 * @brief Example Android view with ViewModel integration
 */
class AndroidExampleView : public AndroidViewAdapter {
public:
    AndroidExampleView(JNIEnv *env, jobject activity);
    
    // Bind to a ViewModel
    void bindViewModel(std::shared_ptr<INativeViewModel> viewModel);
    
    // Handle UI events from Java
    void onNameChanged(const std::string &name);
    void onActiveChanged(bool active);
    void onIncrementClicked();
    void onResetClicked();
    void onShowMessageClicked();
    
private:
    std::shared_ptr<INativeViewModel> m_viewModel;
    
    // Update UI from ViewModel
    void updateUIFromViewModel();
};

} // namespace QtMvvm

#endif // ANDROID_VIEW_ADAPTER_H
