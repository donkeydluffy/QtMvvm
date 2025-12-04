#include "nativeserviceregistry.h"

namespace QtMvvm {

// NativeServiceRegistryBase implementation

NativeServiceRegistryBase::NativeServiceRegistryBase()
	: m_services()
{
}

NativeServiceRegistryBase::~NativeServiceRegistryBase()
{
	clear();
}

bool NativeServiceRegistryBase::isRegistered(const std::string &serviceId) const
{
	return m_services.find(serviceId) != m_services.end();
}

void NativeServiceRegistryBase::registerService(
	const std::string &serviceId,
	std::function<void*()> factory,
	std::function<void(void*)> deleter,
	DestructionScope scope,
	bool weak)
{
	if (!weak && isRegistered(serviceId)) {
		throw NativeServiceExistsException(serviceId);
	}

	if (isRegistered(serviceId)) {
		return; // Weak registration, already exists
	}

	ServiceInfo info;
	info.instance = nullptr;
	info.factory = factory;
	info.deleter = deleter;
	info.scope = scope;
	info.weak = weak;
	info.isFactoryCreated = false;

	m_services[serviceId] = info;
}

void NativeServiceRegistryBase::registerServiceInstance(
	const std::string &serviceId,
	void *instance,
	std::function<void(void*)> deleter,
	DestructionScope scope,
	bool weak)
{
	if (!weak && isRegistered(serviceId)) {
		throw NativeServiceExistsException(serviceId);
	}

	if (isRegistered(serviceId)) {
		return; // Weak registration, already exists
	}

	ServiceInfo info;
	info.instance = instance;
	info.factory = nullptr;
	info.deleter = deleter;
	info.scope = scope;
	info.weak = weak;
	info.isFactoryCreated = false;

	m_services[serviceId] = info;
}

void* NativeServiceRegistryBase::getService(const std::string &serviceId)
{
	auto it = m_services.find(serviceId);
	if (it == m_services.end()) {
		throw NativeServiceDependencyException(serviceId);
	}

	ServiceInfo &info = it->second;

	// Lazy initialization
	if (info.instance == nullptr && info.factory) {
		try {
			info.instance = info.factory();
			info.isFactoryCreated = true;
		} catch (const std::exception &e) {
			throw NativeServiceConstructionException(
				std::string("Failed to construct service '") + serviceId + "': " + e.what()
			);
		} catch (...) {
			throw NativeServiceConstructionException(
				std::string("Failed to construct service '") + serviceId + "': unknown error"
			);
		}
	}

	return info.instance;
}

void NativeServiceRegistryBase::unregisterService(const std::string &serviceId)
{
	auto it = m_services.find(serviceId);
	if (it == m_services.end()) {
		return;
	}

	ServiceInfo &info = it->second;
	if (info.instance && info.deleter && info.isFactoryCreated) {
		info.deleter(info.instance);
	}

	m_services.erase(it);
}

void NativeServiceRegistryBase::clear()
{
	// Destroy in reverse order of registration
	for (auto it = m_services.rbegin(); it != m_services.rend(); ++it) {
		ServiceInfo &info = it->second;
		if (info.instance && info.deleter && info.isFactoryCreated) {
			info.deleter(info.instance);
		}
	}
	m_services.clear();
}

void NativeServiceRegistryBase::destroyServicesInScope(DestructionScope scope)
{
	std::vector<std::string> toRemove;

	for (auto &pair : m_services) {
		if (pair.second.scope == scope) {
			ServiceInfo &info = pair.second;
			if (info.instance && info.deleter && info.isFactoryCreated) {
				info.deleter(info.instance);
				info.instance = nullptr;
			}
			toRemove.push_back(pair.first);
		}
	}

	for (const auto &serviceId : toRemove) {
		m_services.erase(serviceId);
	}
}

}
