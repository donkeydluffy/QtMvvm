#include "bindingadapter.h"
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

namespace QtMvvm {

// QtPropertyAdapter implementation

QtPropertyAdapter::QtPropertyAdapter(QObject *object, const char *propertyName)
	: m_object(object)
	, m_property()
	, m_callback()
	, m_connection()
{
	if (m_object) {
		int propertyIndex = m_object->metaObject()->indexOfProperty(propertyName);
		if (propertyIndex >= 0) {
			m_property = m_object->metaObject()->property(propertyIndex);
		}
	}
}

QtPropertyAdapter::QtPropertyAdapter(QObject *object, const QMetaProperty &property)
	: m_object(object)
	, m_property(property)
	, m_callback()
	, m_connection()
{
}

QtPropertyAdapter::~QtPropertyAdapter()
{
	unregisterChangeCallbacks();
}

std::string QtPropertyAdapter::name() const
{
	return m_property.isValid() ? m_property.name() : std::string();
}

std::string QtPropertyAdapter::getValue() const
{
	if (!m_object || !m_property.isValid()) {
		return std::string();
	}

	QVariant value = m_property.read(m_object.data());
	return value.toString().toStdString();
}

void QtPropertyAdapter::setValue(const std::string &value)
{
	if (!m_object || !m_property.isValid()) {
		return;
	}

	QString qvalue = QString::fromStdString(value);
	QVariant variant = QVariant::fromValue(qvalue);
	
	// Try to convert to the property's actual type
	if (variant.canConvert(m_property.userType())) {
		variant.convert(m_property.userType());
	}
	
	m_property.write(m_object.data(), variant);
}

void QtPropertyAdapter::registerChangeCallback(ChangeCallback callback)
{
	m_callback = callback;
	
	if (!m_object || !m_property.isValid() || !m_property.hasNotifySignal()) {
		return;
	}

	// Disconnect previous connection
	if (m_connection) {
		QObject::disconnect(m_connection);
	}

	// Connect to property's notify signal
	if (m_callback) {
		QMetaMethod notifySignal = m_property.notifySignal();
		
		// Create a generic slot that reads the property and calls the callback
		m_connection = QObject::connect(m_object.data(), notifySignal,
			m_object.data(), [this]() {
				if (m_callback) {
					m_callback(getValue());
				}
			}, Qt::DirectConnection);
	}
}

void QtPropertyAdapter::unregisterChangeCallbacks()
{
	if (m_connection) {
		QObject::disconnect(m_connection);
		m_connection = QMetaObject::Connection();
	}
	m_callback = nullptr;
}

QObject* QtPropertyAdapter::object() const
{
	return m_object.data();
}

QMetaProperty QtPropertyAdapter::property() const
{
	return m_property;
}

// NativeBindingAdapter implementation

Binding NativeBindingAdapter::createQtBinding(std::unique_ptr<INativeBinding> nativeBinding)
{
	// Note: This is a simplified implementation
	// A full implementation would need to store the unique_ptr and manage its lifetime
	// For now, we just check validity
	if (!nativeBinding || !nativeBinding->isValid()) {
		return Binding();
	}
	
	// Would need a way to wrap the native binding in a Qt Binding handle
	// This is a placeholder implementation
	return Binding();
}

std::unique_ptr<INativeBinding> NativeBindingAdapter::createNativeBinding(
	QObject *viewModel, const char *viewModelProperty,
	QObject *view, const char *viewProperty,
	INativeBinding::Direction direction)
{
	if (!viewModel || !view || !viewModelProperty || !viewProperty) {
		return nullptr;
	}

	// Create property adapters
	auto viewModelProp = std::make_unique<QtPropertyAdapter>(viewModel, viewModelProperty);
	auto viewProp = std::make_unique<QtPropertyAdapter>(view, viewProperty);

	// Create factory and binding
	NativeBindingFactory factory;
	auto binding = factory.createBinding(viewModelProp.get(), viewProp.get(), direction);

	// Note: In a full implementation, we'd need to manage the lifetime of the property adapters
	// For now, this is a demonstration of the concept
	
	return binding;
}

INativeBinding::Direction NativeBindingAdapter::convertDirection(Binding::BindingDirection qtDirection)
{
	int result = 0;
	
	if (qtDirection & Binding::SingleInit) {
		result |= INativeBinding::SingleInit;
	}
	if (qtDirection & Binding::OneWayToView) {
		result |= INativeBinding::OneWayToView;
	}
	if (qtDirection & Binding::OneWayToViewModel) {
		result |= INativeBinding::OneWayToViewModel;
	}
	
	return static_cast<INativeBinding::Direction>(result);
}

Binding::BindingDirection NativeBindingAdapter::convertDirection(INativeBinding::Direction nativeDirection)
{
	Binding::BindingDirection result = static_cast<Binding::BindingDirection>(0);
	
	if (nativeDirection & INativeBinding::SingleInit) {
		result |= Binding::SingleInit;
	}
	if (nativeDirection & INativeBinding::OneWayToView) {
		result |= Binding::OneWayToView;
	}
	if (nativeDirection & INativeBinding::OneWayToViewModel) {
		result |= Binding::OneWayToViewModel;
	}
	
	return result;
}

}
