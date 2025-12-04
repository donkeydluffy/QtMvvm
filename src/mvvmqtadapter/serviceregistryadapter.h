#ifndef QTMVVM_SERVICEREGISTRYADAPTER_H
#define QTMVVM_SERVICEREGISTRYADAPTER_H

#include <QtCore/qobject.h>
#include <QtCore/qhash.h>

#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/serviceregistry.h"
#include "QtMvvmCore/nativeserviceregistry.h"

namespace QtMvvm {

//! Adapter to make Qt ServiceRegistry work as INativeServiceRegistry
class Q_MVVMCORE_EXPORT ServiceRegistryToNativeAdapter : public INativeServiceRegistry
{
public:
	//! Constructor taking Qt ServiceRegistry
	explicit ServiceRegistryToNativeAdapter(ServiceRegistry *qtRegistry);
	~ServiceRegistryToNativeAdapter() override;

	// INativeServiceRegistry interface implementation
	bool isRegistered(const std::string &serviceId) const override;
	
	void registerService(
		const std::string &serviceId,
		std::function<void*()> factory,
		std::function<void(void*)> deleter,
		DestructionScope scope = DestroyOnAppDestroy,
		bool weak = false
	) override;
	
	void registerServiceInstance(
		const std::string &serviceId,
		void *instance,
		std::function<void(void*)> deleter,
		DestructionScope scope = DestroyOnAppDestroy,
		bool weak = false
	) override;
	
	void* getService(const std::string &serviceId) override;
	void unregisterService(const std::string &serviceId) override;
	void clear() override;

	//! Get the underlying Qt ServiceRegistry
	ServiceRegistry* qtServiceRegistry() const;

private:
	ServiceRegistry *m_qtRegistry;
	QHash<QString, void*> m_nativeServices; // Track native-registered services
	QHash<QString, std::function<void(void*)>> m_deleters;
};

//! Adapter to make INativeServiceRegistry work as Qt ServiceRegistry
class Q_MVVMCORE_EXPORT NativeToServiceRegistryAdapter
{
public:
	//! Constructor taking native ServiceRegistry
	explicit NativeToServiceRegistryAdapter(INativeServiceRegistry *nativeRegistry);
	~NativeToServiceRegistryAdapter();

	//! Register a Qt service through native interface
	template<typename TInterface, typename TService>
	void registerInterface(
		typename INativeServiceRegistry::DestructionScope scope = INativeServiceRegistry::DestroyOnAppDestroy,
		bool weak = false
	);

	//! Register a Qt object through native interface
	template<typename TService>
	void registerObject(
		typename INativeServiceRegistry::DestructionScope scope = INativeServiceRegistry::DestroyOnAppDestroy,
		bool weak = false
	);

	//! Get a Qt service through native interface
	template<typename TInterface>
	TInterface* service();

	//! Get the underlying native ServiceRegistry
	INativeServiceRegistry* nativeServiceRegistry() const;

private:
	INativeServiceRegistry *m_nativeRegistry;
	QHash<QString, QObject*> m_qtServices; // Track Qt-registered services
};

// Template implementations

template<typename TInterface, typename TService>
void NativeToServiceRegistryAdapter::registerInterface(
	typename INativeServiceRegistry::DestructionScope scope,
	bool weak)
{
	std::string serviceId = typeid(TInterface).name();
	
	m_nativeRegistry->registerService(
		serviceId,
		[]() -> void* { return new TService(); },
		[](void *ptr) { delete static_cast<TService*>(ptr); },
		scope,
		weak
	);
}

template<typename TService>
void NativeToServiceRegistryAdapter::registerObject(
	typename INativeServiceRegistry::DestructionScope scope,
	bool weak)
{
	std::string serviceId = typeid(TService).name();
	
	m_nativeRegistry->registerService(
		serviceId,
		[]() -> void* { return new TService(); },
		[](void *ptr) { delete static_cast<TService*>(ptr); },
		scope,
		weak
	);
}

template<typename TInterface>
TInterface* NativeToServiceRegistryAdapter::service()
{
	std::string serviceId = typeid(TInterface).name();
	void *ptr = m_nativeRegistry->getService(serviceId);
	return static_cast<TInterface*>(ptr);
}

}

#endif // QTMVVM_SERVICEREGISTRYADAPTER_H
