/**
 * @file serviceregistry_example.cpp
 * @brief Demonstrates native service registry and dependency injection
 * 
 * This example shows how to use INativeServiceRegistry for dependency injection
 * without Qt dependencies.
 */

#include <iostream>
#include <memory>
#include <string>

// Simulated native interfaces for demonstration
namespace QtMvvm {

// Simple logger service interface
class ILogger {
public:
	virtual ~ILogger() = default;
	virtual void log(const std::string &message) = 0;
	virtual void setLevel(int level) = 0;
};

// Console logger implementation
class ConsoleLogger : public ILogger {
private:
	int m_level;
	std::string m_name;

public:
	ConsoleLogger(const std::string &name = "Console")
		: m_level(0), m_name(name)
	{
		std::cout << "[" << m_name << " Logger] Created" << std::endl;
	}

	~ConsoleLogger() override {
		std::cout << "[" << m_name << " Logger] Destroyed" << std::endl;
	}

	void log(const std::string &message) override {
		std::cout << "[" << m_name << "] " << message << std::endl;
	}

	void setLevel(int level) override {
		m_level = level;
		std::cout << "[" << m_name << " Logger] Level set to " << level << std::endl;
	}
};

// Data service interface
class IDataService {
public:
	virtual ~IDataService() = default;
	virtual void loadData() = 0;
	virtual void saveData(const std::string &data) = 0;
};

// Data service implementation with logger dependency
class DataService : public IDataService {
private:
	ILogger *m_logger;
	std::string m_data;

public:
	explicit DataService(ILogger *logger)
		: m_logger(logger), m_data()
	{
		if (m_logger) {
			m_logger->log("DataService created");
		}
	}

	~DataService() override {
		if (m_logger) {
			m_logger->log("DataService destroyed");
		}
	}

	void loadData() override {
		m_data = "Sample data loaded";
		if (m_logger) {
			m_logger->log("Data loaded: " + m_data);
		}
	}

	void saveData(const std::string &data) override {
		m_data = data;
		if (m_logger) {
			m_logger->log("Data saved: " + m_data);
		}
	}
};

// Simple service registry implementation for demonstration
class SimpleServiceRegistry {
private:
	std::map<std::string, void*> m_services;
	std::map<std::string, std::function<void(void*)>> m_deleters;
	std::map<std::string, std::function<void*()>> m_factories;

public:
	~SimpleServiceRegistry() {
		clear();
	}

	template<typename TService>
	void registerService(const std::string &serviceId) {
		m_factories[serviceId] = []() -> void* {
			return new TService();
		};
		m_deleters[serviceId] = [](void *ptr) {
			delete static_cast<TService*>(ptr);
		};
	}

	template<typename TService>
	void registerService(const std::string &serviceId, std::function<TService*()> factory) {
		m_factories[serviceId] = [factory]() -> void* {
			return factory();
		};
		m_deleters[serviceId] = [](void *ptr) {
			delete static_cast<TService*>(ptr);
		};
	}

	template<typename TService>
	TService* getService(const std::string &serviceId) {
		// Check if already instantiated
		auto it = m_services.find(serviceId);
		if (it != m_services.end()) {
			return static_cast<TService*>(it->second);
		}

		// Lazy instantiation
		auto factoryIt = m_factories.find(serviceId);
		if (factoryIt != m_factories.end()) {
			void *instance = factoryIt->second();
			m_services[serviceId] = instance;
			return static_cast<TService*>(instance);
		}

		return nullptr;
	}

	bool isRegistered(const std::string &serviceId) const {
		return m_factories.find(serviceId) != m_factories.end() ||
		       m_services.find(serviceId) != m_services.end();
	}

	void clear() {
		// Delete in reverse order
		for (auto it = m_services.rbegin(); it != m_services.rend(); ++it) {
			auto deleterIt = m_deleters.find(it->first);
			if (deleterIt != m_deleters.end()) {
				deleterIt->second(it->second);
			}
		}
		m_services.clear();
		m_factories.clear();
		m_deleters.clear();
	}
};

} // namespace QtMvvm

int main() {
	using namespace QtMvvm;

	std::cout << "==================================================" << std::endl;
	std::cout << "Native Service Registry Example" << std::endl;
	std::cout << "==================================================\n" << std::endl;

	auto registry = std::make_unique<SimpleServiceRegistry>();

	std::cout << "Step 1: Register services" << std::endl;
	std::cout << "-------------------------" << std::endl;
	
	// Register logger service
	registry->registerService<ConsoleLogger>("ILogger");
	std::cout << "Registered: ILogger -> ConsoleLogger" << std::endl;
	
	// Register data service with dependency injection
	registry->registerService<DataService>("IDataService", [&registry]() {
		// Inject logger dependency
		ILogger *logger = registry->getService<ILogger>("ILogger");
		return new DataService(logger);
	});
	std::cout << "Registered: IDataService -> DataService (with ILogger dependency)" << std::endl;

	std::cout << "\nStep 2: Check if services are registered" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "ILogger registered: " << (registry->isRegistered("ILogger") ? "Yes" : "No") << std::endl;
	std::cout << "IDataService registered: " << (registry->isRegistered("IDataService") ? "Yes" : "No") << std::endl;

	std::cout << "\nStep 3: Get service (lazy instantiation)" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	ILogger *logger = registry->getService<ILogger>("ILogger");
	std::cout << "Got ILogger service" << std::endl;
	
	logger->setLevel(1);
	logger->log("Service registry initialized");

	std::cout << "\nStep 4: Get service with dependency" << std::endl;
	std::cout << "------------------------------------" << std::endl;
	IDataService *dataService = registry->getService<IDataService>("IDataService");
	std::cout << "Got IDataService service (with injected ILogger)" << std::endl;

	std::cout << "\nStep 5: Use the services" << std::endl;
	std::cout << "------------------------" << std::endl;
	dataService->loadData();
	dataService->saveData("New user data");

	std::cout << "\nStep 6: Reuse singleton services" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	ILogger *logger2 = registry->getService<ILogger>("ILogger");
	std::cout << "Same logger instance: " << (logger == logger2 ? "Yes" : "No") << std::endl;
	logger2->log("Singleton service reused");

	std::cout << "\nStep 7: Clean up (registry destructor)" << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	registry->clear();
	std::cout << "Services destroyed in reverse order" << std::endl;

	std::cout << "\n==================================================" << std::endl;
	std::cout << "Key Benefits Demonstrated:" << std::endl;
	std::cout << "==================================================" << std::endl;
	std::cout << "1. Dependency injection without Qt meta-object" << std::endl;
	std::cout << "2. Lazy instantiation of services" << std::endl;
	std::cout << "3. Singleton service management" << std::endl;
	std::cout << "4. Automatic dependency resolution" << std::endl;
	std::cout << "5. No QObject or Qt types required" << std::endl;
	std::cout << "==================================================\n" << std::endl;

	return 0;
}
