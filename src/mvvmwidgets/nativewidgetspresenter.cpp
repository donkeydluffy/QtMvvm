#include "nativewidgetspresenter.h"
#include <QtMvvmCore/coreapp.h>
#include <QtMvvmCore/serviceregistry.h>

namespace QtMvvm {

NativeWidgetsPresenter::NativeWidgetsPresenter(QObject *parent)
	: QObject(parent)
	, NativePresenterBase()
	, m_widgetsPresenter(new WidgetsPresenter(this))
	, m_adapter(new PresenterAdapter(this))
{
}

NativeWidgetsPresenter::~NativeWidgetsPresenter() = default;

void NativeWidgetsPresenter::registerAsPresenter()
{
	// Register as both INativePresenter and IPresenter for compatibility
	ServiceRegistry::instance()->registerInterface<IPresenter, NativeWidgetsPresenter>();
}

void NativeWidgetsPresenter::present(ViewModel *viewModel, 
                                     const std::map<std::string, std::string> &params, 
                                     ViewModel *parentViewModel)
{
	// Convert native params to Qt format
	auto qtParams = PresenterAdapter::convertParamsFromNative(params);
	
	// Use the underlying Qt widgets presenter
	QPointer<ViewModel> parent(parentViewModel);
	m_widgetsPresenter->present(viewModel, qtParams, parent);
}

void NativeWidgetsPresenter::showDialog(const NativeMessageConfig &config, MessageResult *result)
{
	// Convert native message config to Qt format
	auto qtConfig = PresenterAdapter::convertMessageConfigFromNative(config);
	
	// Use the underlying Qt widgets presenter
	m_widgetsPresenter->showDialog(qtConfig, result);
}

WidgetsPresenter* NativeWidgetsPresenter::widgetsPresenter() const
{
	return m_widgetsPresenter;
}

}
