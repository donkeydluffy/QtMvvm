#include "presenteradapter.h"
#include <QtCore/qstring.h>

namespace QtMvvm {

// PresenterAdapter implementation

PresenterAdapter::PresenterAdapter(INativePresenter *nativePresenter)
	: m_nativePresenter(nativePresenter)
{
}

PresenterAdapter::~PresenterAdapter() = default;

void PresenterAdapter::present(ViewModel *viewModel, const QVariantHash &params, QPointer<ViewModel> parent)
{
	if (m_nativePresenter) {
		auto nativeParams = convertParamsToNative(params);
		m_nativePresenter->present(viewModel, nativeParams, parent.data());
	}
}

void PresenterAdapter::showDialog(const MessageConfig &config, MessageResult *result)
{
	if (m_nativePresenter) {
		auto nativeConfig = convertMessageConfigToNative(config);
		m_nativePresenter->showDialog(nativeConfig, result);
	}
}

INativePresenter* PresenterAdapter::nativePresenter() const
{
	return m_nativePresenter;
}

std::map<std::string, std::string> PresenterAdapter::convertParamsToNative(const QVariantHash &params)
{
	std::map<std::string, std::string> result;
	for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
		result[it.key().toStdString()] = it.value().toString().toStdString();
	}
	return result;
}

QVariantHash PresenterAdapter::convertParamsFromNative(const std::map<std::string, std::string> &params)
{
	QVariantHash result;
	for (const auto &pair : params) {
		result[QString::fromStdString(pair.first)] = QString::fromStdString(pair.second);
	}
	return result;
}

NativeMessageConfig PresenterAdapter::convertMessageConfigToNative(const MessageConfig &config)
{
	NativeMessageConfig nativeConfig;
	nativeConfig.type = config.type().toStdString();
	nativeConfig.subType = config.subType().toStdString();
	nativeConfig.title = config.title().toStdString();
	nativeConfig.text = config.text().toStdString();
	nativeConfig.buttons = static_cast<int>(config.buttons());
	
	// Convert button texts
	auto buttonTexts = config.buttonTexts();
	for (auto it = buttonTexts.constBegin(); it != buttonTexts.constEnd(); ++it) {
		nativeConfig.buttonTexts[static_cast<int>(it.key())] = it.value().toStdString();
	}
	
	// Convert view properties
	auto viewProps = config.viewProperties();
	for (auto it = viewProps.constBegin(); it != viewProps.constEnd(); ++it) {
		nativeConfig.viewProperties[it.key().toStdString()] = it.value().toString().toStdString();
	}
	
	if (config.defaultValue().isValid()) {
		nativeConfig.defaultValue = config.defaultValue().toString().toStdString();
	}
	
	return nativeConfig;
}

MessageConfig PresenterAdapter::convertMessageConfigFromNative(const NativeMessageConfig &config)
{
	MessageConfig qtConfig;
	qtConfig.setType(QByteArray::fromStdString(config.type));
	qtConfig.setSubType(QByteArray::fromStdString(config.subType));
	qtConfig.setTitle(QString::fromStdString(config.title));
	qtConfig.setText(QString::fromStdString(config.text));
	qtConfig.setButtons(static_cast<MessageConfig::StandardButtons>(config.buttons));
	
	// Convert button texts
	QHash<MessageConfig::StandardButton, QString> buttonTexts;
	for (const auto &pair : config.buttonTexts) {
		buttonTexts[static_cast<MessageConfig::StandardButton>(pair.first)] = QString::fromStdString(pair.second);
	}
	qtConfig.setButtonTexts(buttonTexts);
	
	// Convert view properties
	QVariantMap viewProps;
	for (const auto &pair : config.viewProperties) {
		viewProps[QString::fromStdString(pair.first)] = QString::fromStdString(pair.second);
	}
	qtConfig.setViewProperties(viewProps);
	
	if (!config.defaultValue.empty()) {
		qtConfig.setDefaultValue(QString::fromStdString(config.defaultValue));
	}
	
	return qtConfig;
}

// NativePresenterAdapter implementation

NativePresenterAdapter::NativePresenterAdapter(IPresenter *qtPresenter)
	: m_qtPresenter(qtPresenter)
	, m_viewFactories()
{
}

NativePresenterAdapter::~NativePresenterAdapter() = default;

void NativePresenterAdapter::present(ViewModel *viewModel, 
                                     const std::map<std::string, std::string> &params, 
                                     ViewModel *parentViewModel)
{
	if (m_qtPresenter) {
		auto qtParams = PresenterAdapter::convertParamsFromNative(params);
		QPointer<ViewModel> parent(parentViewModel);
		m_qtPresenter->present(viewModel, qtParams, parent);
	}
}

void NativePresenterAdapter::showDialog(const NativeMessageConfig &config, MessageResult *result)
{
	if (m_qtPresenter) {
		auto qtConfig = PresenterAdapter::convertMessageConfigFromNative(config);
		m_qtPresenter->showDialog(qtConfig, result);
	}
}

void NativePresenterAdapter::registerViewFactory(std::shared_ptr<INativeViewFactory> factory)
{
	if (factory) {
		m_viewFactories.push_back(factory);
	}
}

bool NativePresenterAdapter::canPresent(const std::string &viewModelTypeName) const
{
	// Check if any registered factory can create a view for this view model
	for (const auto &factory : m_viewFactories) {
		if (factory && factory->canCreateView(viewModelTypeName)) {
			return true;
		}
	}
	return false;
}

IPresenter* NativePresenterAdapter::qtPresenter() const
{
	return m_qtPresenter;
}

}
