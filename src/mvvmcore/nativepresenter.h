#ifndef QTMVVM_NATIVEPRESENTER_H
#define QTMVVM_NATIVEPRESENTER_H

#include <string>
#include <map>
#include <memory>
#include <functional>
#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/nativeview.h"

namespace QtMvvm {

// Forward declarations
class ViewModel;
class MessageResult;

//! Native message configuration structure (Qt-independent)
struct Q_MVVMCORE_EXPORT NativeMessageConfig
{
	std::string type;
	std::string subType;
	std::string title;
	std::string text;
	int buttons = 0;
	std::map<int, std::string> buttonTexts;
	std::string defaultValue;
	std::map<std::string, std::string> viewProperties;

	NativeMessageConfig() = default;
	NativeMessageConfig(const std::string &type);
};

//! Pure C++ presenter interface without Qt dependencies
class Q_MVVMCORE_EXPORT INativePresenter
{
public:
	virtual ~INativePresenter() = default;

	//! Called to present a viewmodel
	//! @param viewModel The view model to present
	//! @param params Parameters for the view model initialization (string-string map)
	//! @param parentViewModel Pointer to parent view model (can be null)
	virtual void present(ViewModel *viewModel, 
	                     const std::map<std::string, std::string> &params, 
	                     ViewModel *parentViewModel = nullptr) = 0;

	//! Called to present a dialog via a native message config
	//! @param config The message configuration
	//! @param result The result handler for the dialog
	virtual void showDialog(const NativeMessageConfig &config, MessageResult *result) = 0;

	//! Registers a view factory for creating views
	//! @param factory The factory to register
	virtual void registerViewFactory(std::shared_ptr<INativeViewFactory> factory) = 0;

	//! Checks if a view can be presented for the given view model type
	//! @param viewModelTypeName The type name of the view model
	//! @return true if a view can be created and presented
	virtual bool canPresent(const std::string &viewModelTypeName) const = 0;
};

//! Base implementation of INativePresenter to reduce boilerplate
class Q_MVVMCORE_EXPORT NativePresenterBase : public INativePresenter
{
public:
	NativePresenterBase();
	virtual ~NativePresenterBase();

	void registerViewFactory(std::shared_ptr<INativeViewFactory> factory) override;
	bool canPresent(const std::string &viewModelTypeName) const override;

protected:
	//! Find a view factory that can create a view for the given view model type
	INativeViewFactory* findViewFactory(const std::string &viewModelTypeName) const;
	
	//! Get all registered view factories
	const std::vector<std::shared_ptr<INativeViewFactory>>& viewFactories() const;

private:
	std::vector<std::shared_ptr<INativeViewFactory>> m_viewFactories;
};

}

#endif // QTMVVM_NATIVEPRESENTER_H
