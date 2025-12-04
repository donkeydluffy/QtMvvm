#ifndef QTMVVM_NATIVESERVICEREGISTRY_H
#define QTMVVM_NATIVESERVICEREGISTRY_H

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <typeinfo>

#include "QtMvvmCore/qtmvvmcore_global.h"

namespace QtMvvm {

//! Pure C++ service registry interface without Qt dependencies
class Q_MVVMCORE_EXPORT INativeServiceRegistry
{
public:
	virtual ~INativeServiceRegistry() = default;

	//! Scope to indicate when a service should be deleted
	enum DestructionScope {
		DestroyOnAppQuit = 1,
		DestroyOnAppDestroy = 2,
		DestroyOnRegistryDestroy = 3,
		DestroyNever = 127
	};

	//! Check if a service with given ID is registered
	virtual bool isRegistered(const std::string &serviceId) const = 0;

	//! Register a service factory function
	virtual void registerService(
		const std::string &serviceId,
		std::function<void*()> factory,
		std::function<void(void*)> deleter,
		DestructionScope scope = DestroyOnAppDestroy,
		bool weak = false
	) = 0;

	//! Register an existing service instance
	virtual void registerServiceInstance(
		const std::string &serviceId,
		void *instance,
		std::function<void(void*)> deleter,
		DestructionScope scope = DestroyOnAppDestroy,
		bool weak = false
	) = 0;

	//! Get a service by ID
	virtual void* getService(const std::string &serviceId) = 0;

	//! Unregister a service
	virtual void unregisterService(const std::string &serviceId) = 0;

	//! Clear all services
	virtual void clear() = 0;
};

//! Base implementation of INativeServiceRegistry
class Q_MVVMCORE_EXPORT NativeServiceRegistryBase : public INativeServiceRegistry
{
public:
	NativeServiceRegistryBase();
	~NativeServiceRegistryBase() override;

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

protected:
	//! Destroy services based on scope
	void destroyServicesInScope(DestructionScope scope);

private:
	struct ServiceInfo {
		void *instance;
		std::function<void*()> factory;
		std::function<void(void*)> deleter;
		DestructionScope scope;
		bool weak;
		bool isFactoryCreated;
	};

	std::map<std::string, ServiceInfo> m_services;
};

//! Template-based typed service registry
template<typename TRegistry = NativeServiceRegistryBase>
class TypedServiceRegistry : public TRegistry
{
public:
	//! Register a service with automatic type deduction
	template<typename TService>
	void registerService(
		const std::string &serviceId,
		typename INativeServiceRegistry::DestructionScope scope = INativeServiceRegistry::DestroyOnAppDestroy,
		bool weak = false
	) {
		TRegistry::registerService(
			serviceId,
			[]() -> void* { return new TService(); },
			[](void *ptr) { delete static_cast<TService*>(ptr); },
			scope,
			weak
		);
	}

	//! Register a service with factory function
	template<typename TService>
	void registerService(
		const std::string &serviceId,
		std::function<TService*()> factory,
		typename INativeServiceRegistry::DestructionScope scope = INativeServiceRegistry::DestroyOnAppDestroy,
		bool weak = false
	) {
		TRegistry::registerService(
			serviceId,
			[factory]() -> void* { return factory(); },
			[](void *ptr) { delete static_cast<TService*>(ptr); },
			scope,
			weak
		);
	}

	//! Register an existing service instance
	template<typename TService>
	void registerServiceInstance(
		const std::string &serviceId,
		TService *instance,
		typename INativeServiceRegistry::DestructionScope scope = INativeServiceRegistry::DestroyOnAppDestroy,
		bool weak = false
	) {
		TRegistry::registerServiceInstance(
			serviceId,
			instance,
			[](void *ptr) { delete static_cast<TService*>(ptr); },
			scope,
			weak
		);
	}

	//! Get a service with automatic type casting
	template<typename TService>
	TService* getService(const std::string &serviceId) {
		void *ptr = TRegistry::getService(serviceId);
		return static_cast<TService*>(ptr);
	}
};

//! Native injection helper - stores service ID for later injection
class Q_MVVMCORE_EXPORT NativeInjectable
{
public:
	explicit NativeInjectable(const std::string &serviceId)
		: m_serviceId(serviceId) {}

	std::string serviceId() const { return m_serviceId; }

private:
	std::string m_serviceId;
};

//! Exception for service registry errors
class Q_MVVMCORE_EXPORT NativeServiceException : public std::runtime_error
{
public:
	explicit NativeServiceException(const std::string &message)
		: std::runtime_error(message) {}
};

//! Exception when service already exists
class Q_MVVMCORE_EXPORT NativeServiceExistsException : public NativeServiceException
{
public:
	explicit NativeServiceExistsException(const std::string &serviceId)
		: NativeServiceException("Service already registered: " + serviceId) {}
};

//! Exception when service construction fails
class Q_MVVMCORE_EXPORT NativeServiceConstructionException : public NativeServiceException
{
public:
	explicit NativeServiceConstructionException(const std::string &message)
		: NativeServiceException("Service construction failed: " + message) {}
};

//! Exception when service dependency is missing
class Q_MVVMCORE_EXPORT NativeServiceDependencyException : public NativeServiceConstructionException
{
public:
	explicit NativeServiceDependencyException(const std::string &serviceId)
		: NativeServiceConstructionException("Missing dependency: " + serviceId) {}
};

}

#endif // QTMVVM_NATIVESERVICEREGISTRY_H
