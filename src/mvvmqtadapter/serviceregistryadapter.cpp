#include "serviceregistryadapter.h"
#include <QtCore/qstring.h>
#include <QtCore/qbytearray.h>

namespace QtMvvm {

// ServiceRegistryToNativeAdapter implementation

ServiceRegistryToNativeAdapter::ServiceRegistryToNativeAdapter(ServiceRegistry *qtRegistry)
	: m_qtRegistry(qtRegistry)
	, m_nativeServices()
	, m_deleters()
{
}

ServiceRegistryToNativeAdapter::~ServiceRegistryToNativeAdapter()
{
	// Clean up native-registered services
	for (auto it = m_nativeServices.begin(); it != m_nativeServices.end(); ++it) {
		auto deleterIt = m_deleters.find(it.key());
		if (deleterIt != m_deleters.end()) {
			deleterIt.value()(it.value());
		}
	}
}

bool ServiceRegistryToNativeAdapter::isRegistered(const std::string &serviceId) const
{
	QString qServiceId = QString::fromStdString(serviceId);
	
	// Check if registered as native service
	if (m_nativeServices.contains(qServiceId)) {
		return true;
	}
	
	// Check if registered in Qt registry
	if (m_qtRegistry) {
		return m_qtRegistry->isRegistered(qServiceId.toUtf8());
	}
	
	return false;
}

void ServiceRegistryToNativeAdapter::registerService(
	const std::string &serviceId,
	std::function<void*()> factory,
	std::function<void(void*)> deleter,
	DestructionScope scope,
	bool weak)
{
	QString qServiceId = QString::fromStdString(serviceId);
	
	if (!weak && isRegistered(serviceId)) {
		throw NativeServiceExistsException(serviceId);
	}
	
	if (isRegistered(serviceId)) {
		return; // Weak registration
	}
	
	// Create instance eagerly for simplicity
	void *instance = factory();
	m_nativeServices[qServiceId] = instance;
	m_deleters[qServiceId] = deleter;
}

void ServiceRegistryToNativeAdapter::registerServiceInstance(
	const std::string &serviceId,
	void *instance,
	std::function<void(void*)> deleter,
	DestructionScope scope,
	bool weak)
{
	QString qServiceId = QString::fromStdString(serviceId);
	
	if (!weak && isRegistered(serviceId)) {
		throw NativeServiceExistsException(serviceId);
	}
	
	if (isRegistered(serviceId)) {
		return; // Weak registration
	}
	
	m_nativeServices[qServiceId] = instance;
	m_deleters[qServiceId] = deleter;
}

void* ServiceRegistryToNativeAdapter::getService(const std::string &serviceId)
{
	QString qServiceId = QString::fromStdString(serviceId);
	
	// Check native services first
	auto it = m_nativeServices.find(qServiceId);
	if (it != m_nativeServices.end()) {
		return it.value();
	}
	
	// Try Qt registry
	if (m_qtRegistry) {
		QObject *obj = m_qtRegistry->serviceObj(qServiceId.toUtf8());
		if (obj) {
			return obj;
		}
	}
	
	throw NativeServiceDependencyException(serviceId);
}

void ServiceRegistryToNativeAdapter::unregisterService(const std::string &serviceId)
{
	QString qServiceId = QString::fromStdString(serviceId);
	
	auto it = m_nativeServices.find(qServiceId);
	if (it != m_nativeServices.end()) {
		auto deleterIt = m_deleters.find(qServiceId);
		if (deleterIt != m_deleters.end()) {
			deleterIt.value()(it.value());
			m_deleters.erase(deleterIt);
		}
		m_nativeServices.erase(it);
	}
}

void ServiceRegistryToNativeAdapter::clear()
{
	// Clean up all native services
	for (auto it = m_nativeServices.begin(); it != m_nativeServices.end(); ++it) {
		auto deleterIt = m_deleters.find(it.key());
		if (deleterIt != m_deleters.end()) {
			deleterIt.value()(it.value());
		}
	}
	m_nativeServices.clear();
	m_deleters.clear();
}

ServiceRegistry* ServiceRegistryToNativeAdapter::qtServiceRegistry() const
{
	return m_qtRegistry;
}

// NativeToServiceRegistryAdapter implementation

NativeToServiceRegistryAdapter::NativeToServiceRegistryAdapter(INativeServiceRegistry *nativeRegistry)
	: m_nativeRegistry(nativeRegistry)
	, m_qtServices()
{
}

NativeToServiceRegistryAdapter::~NativeToServiceRegistryAdapter()
{
	// Native registry owns the services
	m_qtServices.clear();
}

INativeServiceRegistry* NativeToServiceRegistryAdapter::nativeServiceRegistry() const
{
	return m_nativeRegistry;
}

}
