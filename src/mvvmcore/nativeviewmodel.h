#ifndef QTMVVM_NATIVEVIEWMODEL_H
#define QTMVVM_NATIVEVIEWMODEL_H

#include <string>
#include <map>
#include <functional>
#include <cstdint>
#include "QtMvvmCore/qtmvvmcore_global.h"

namespace QtMvvm {

//! Pure C++ interface for a view model without Qt dependencies
class Q_MVVMCORE_EXPORT INativeViewModel
{
public:
	virtual ~INativeViewModel() = default;

	//! Called by the presenter to initialize the viewmodel
	virtual void onInit(const std::map<std::string, std::string> &params) = 0;
	
	//! Called by the presenter when a result of a showed viewmodel is ready
	virtual void onResult(uint32_t requestCode, const std::string &result) = 0;

	//! Callback type for result notification
	using ResultCallback = std::function<void(const std::string&)>;
	
	//! Set the callback to be invoked when the viewmodel's result is ready
	virtual void setResultCallback(ResultCallback callback) = 0;
	
	//! Callback type for instance invocation (for singleton viewmodels)
	using InstanceInvokedCallback = std::function<void(const std::map<std::string, std::string>&)>;
	
	//! Set the callback to be invoked when singleton viewmodel is shown again
	virtual void setInstanceInvokedCallback(InstanceInvokedCallback callback) = 0;
};

//! Base implementation of INativeViewModel to reduce boilerplate
class Q_MVVMCORE_EXPORT NativeViewModelBase : public INativeViewModel
{
public:
	NativeViewModelBase();
	virtual ~NativeViewModelBase();

	// INativeViewModel interface implementation
	void onInit(const std::map<std::string, std::string> &params) override;
	void onResult(uint32_t requestCode, const std::string &result) override;
	void setResultCallback(ResultCallback callback) override;
	void setInstanceInvokedCallback(InstanceInvokedCallback callback) override;

protected:
	//! Emit result to the callback
	void emitResult(const std::string &result);
	
	//! Emit instance invoked to the callback
	void emitInstanceInvoked(const std::map<std::string, std::string> &params);
	
	//! Show another viewmodel as a child of this one
	void showViewModel(const std::string &viewModelName, 
	                   const std::map<std::string, std::string> &params = {});
	
	//! Show another viewmodel and expect its result
	void showViewModelForResult(uint32_t requestCode,
	                           const std::string &viewModelName,
	                           const std::map<std::string, std::string> &params = {});

private:
	ResultCallback m_resultCallback;
	InstanceInvokedCallback m_instanceInvokedCallback;
};

}

#endif // QTMVVM_NATIVEVIEWMODEL_H
