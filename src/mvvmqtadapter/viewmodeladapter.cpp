#include "viewmodeladapter.h"
#include <QtCore/qstring.h>

namespace QtMvvm {

// ViewModelToNativeAdapter implementation

ViewModelToNativeAdapter::ViewModelToNativeAdapter(ViewModel *qtViewModel)
	: m_qtViewModel(qtViewModel)
	, m_resultConnection()
	, m_instanceConnection()
{
}

ViewModelToNativeAdapter::~ViewModelToNativeAdapter()
{
	if (m_resultConnection) {
		QObject::disconnect(m_resultConnection);
	}
	if (m_instanceConnection) {
		QObject::disconnect(m_instanceConnection);
	}
}

void ViewModelToNativeAdapter::onInit(const std::map<std::string, std::string> &params)
{
	if (m_qtViewModel) {
		auto qtParams = convertParamsToQt(params);
		m_qtViewModel->onInit(qtParams);
	}
}

void ViewModelToNativeAdapter::onResult(uint32_t requestCode, const std::string &result)
{
	if (m_qtViewModel) {
		QVariant qtResult = QString::fromStdString(result);
		m_qtViewModel->onResult(static_cast<quint32>(requestCode), qtResult);
	}
}

void ViewModelToNativeAdapter::setResultCallback(ResultCallback callback)
{
	if (!m_qtViewModel) {
		return;
	}

	// Disconnect previous connection
	if (m_resultConnection) {
		QObject::disconnect(m_resultConnection);
	}

	// Connect to Qt signal
	if (callback) {
		m_resultConnection = QObject::connect(m_qtViewModel.data(), &ViewModel::resultReady,
			[callback](const QVariant &result) {
				callback(result.toString().toStdString());
			});
	}
}

void ViewModelToNativeAdapter::setInstanceInvokedCallback(InstanceInvokedCallback callback)
{
	if (!m_qtViewModel) {
		return;
	}

	// Disconnect previous connection
	if (m_instanceConnection) {
		QObject::disconnect(m_instanceConnection);
	}

	// Connect to Qt signal
	if (callback) {
		m_instanceConnection = QObject::connect(m_qtViewModel.data(), &ViewModel::instanceInvoked,
			[callback](const QVariantHash &params) {
				callback(convertParamsFromQt(params));
			});
	}
}

ViewModel* ViewModelToNativeAdapter::qtViewModel() const
{
	return m_qtViewModel.data();
}

QVariantHash ViewModelToNativeAdapter::convertParamsToQt(const std::map<std::string, std::string> &params)
{
	QVariantHash result;
	for (const auto &pair : params) {
		result[QString::fromStdString(pair.first)] = QString::fromStdString(pair.second);
	}
	return result;
}

std::map<std::string, std::string> ViewModelToNativeAdapter::convertParamsFromQt(const QVariantHash &params)
{
	std::map<std::string, std::string> result;
	for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
		result[it.key().toStdString()] = it.value().toString().toStdString();
	}
	return result;
}

// NativeToViewModelAdapter implementation

NativeToViewModelAdapter::NativeToViewModelAdapter(INativeViewModel *nativeViewModel, QObject *parent)
	: ViewModel(parent)
	, m_nativeViewModel(nativeViewModel)
{
	if (m_nativeViewModel) {
		// Set up result callback
		m_nativeViewModel->setResultCallback([this](const std::string &result) {
			emit resultReady(QString::fromStdString(result));
		});

		// Set up instance invoked callback
		m_nativeViewModel->setInstanceInvokedCallback([this](const std::map<std::string, std::string> &params) {
			emit instanceInvoked(ViewModelToNativeAdapter::convertParamsToQt(params), QPrivateSignal());
		});
	}
}

NativeToViewModelAdapter::~NativeToViewModelAdapter() = default;

void NativeToViewModelAdapter::onInit(const QVariantHash &params)
{
	if (m_nativeViewModel) {
		auto nativeParams = ViewModelToNativeAdapter::convertParamsFromQt(params);
		m_nativeViewModel->onInit(nativeParams);
	}
}

void NativeToViewModelAdapter::onResult(quint32 requestCode, const QVariant &result)
{
	if (m_nativeViewModel) {
		m_nativeViewModel->onResult(static_cast<uint32_t>(requestCode), result.toString().toStdString());
	}
}

INativeViewModel* NativeToViewModelAdapter::nativeViewModel() const
{
	return m_nativeViewModel;
}

}
