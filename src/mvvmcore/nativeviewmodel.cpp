#include "nativeviewmodel.h"

namespace QtMvvm {

// NativeViewModelBase implementation

NativeViewModelBase::NativeViewModelBase()
	: m_resultCallback()
	, m_instanceInvokedCallback()
{
}

NativeViewModelBase::~NativeViewModelBase() = default;

void NativeViewModelBase::onInit(const std::map<std::string, std::string> &params)
{
	// Default implementation does nothing
	// Subclasses can override to handle initialization
	(void)params;
}

void NativeViewModelBase::onResult(uint32_t requestCode, const std::string &result)
{
	// Default implementation does nothing
	// Subclasses can override to handle results
	(void)requestCode;
	(void)result;
}

void NativeViewModelBase::setResultCallback(ResultCallback callback)
{
	m_resultCallback = callback;
}

void NativeViewModelBase::setInstanceInvokedCallback(InstanceInvokedCallback callback)
{
	m_instanceInvokedCallback = callback;
}

void NativeViewModelBase::emitResult(const std::string &result)
{
	if (m_resultCallback) {
		m_resultCallback(result);
	}
}

void NativeViewModelBase::emitInstanceInvoked(const std::map<std::string, std::string> &params)
{
	if (m_instanceInvokedCallback) {
		m_instanceInvokedCallback(params);
	}
}

void NativeViewModelBase::showViewModel(const std::string &viewModelName,
                                        const std::map<std::string, std::string> &params)
{
	// Note: This would need to interact with the presenter to actually show the viewmodel
	// For now, this is a placeholder that subclasses or a native CoreApp equivalent would implement
	(void)viewModelName;
	(void)params;
}

void NativeViewModelBase::showViewModelForResult(uint32_t requestCode,
                                                 const std::string &viewModelName,
                                                 const std::map<std::string, std::string> &params)
{
	// Note: This would need to interact with the presenter to actually show the viewmodel
	// For now, this is a placeholder that subclasses or a native CoreApp equivalent would implement
	(void)requestCode;
	(void)viewModelName;
	(void)params;
}

}
