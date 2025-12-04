#include "nativebinding.h"

namespace QtMvvm {

// NativeBindingBase implementation

NativeBindingBase::NativeBindingBase(INativeProperty *viewModelProperty,
                                     INativeProperty *viewProperty,
                                     Direction direction)
	: m_viewModelProperty(viewModelProperty)
	, m_viewProperty(viewProperty)
	, m_direction(direction)
	, m_bound(true)
	, m_updating(false)
{
	if (!m_viewModelProperty || !m_viewProperty) {
		m_bound = false;
		return;
	}

	// Initialize view with viewmodel value if needed
	if (m_direction & SingleInit) {
		m_viewProperty->setValue(m_viewModelProperty->getValue());
	}

	// Set up viewmodel to view propagation
	if (m_direction & OneWayToView) {
		m_viewModelProperty->registerChangeCallback([this](const std::string &newValue) {
			onViewModelPropertyChanged(newValue);
		});
	}

	// Set up view to viewmodel propagation
	if (m_direction & OneWayToViewModel) {
		m_viewProperty->registerChangeCallback([this](const std::string &newValue) {
			onViewPropertyChanged(newValue);
		});
	}
}

NativeBindingBase::~NativeBindingBase()
{
	unbind();
}

bool NativeBindingBase::isValid() const
{
	return m_bound && m_viewModelProperty && m_viewProperty;
}

void NativeBindingBase::unbind()
{
	if (!m_bound) {
		return;
	}

	m_bound = false;

	if (m_viewModelProperty) {
		m_viewModelProperty->unregisterChangeCallbacks();
	}

	if (m_viewProperty) {
		m_viewProperty->unregisterChangeCallbacks();
	}
}

void NativeBindingBase::onViewModelPropertyChanged(const std::string &newValue)
{
	if (!m_bound || m_updating) {
		return;
	}

	m_updating = true;
	if (m_viewProperty) {
		m_viewProperty->setValue(newValue);
	}
	m_updating = false;
}

void NativeBindingBase::onViewPropertyChanged(const std::string &newValue)
{
	if (!m_bound || m_updating) {
		return;
	}

	m_updating = true;
	if (m_viewModelProperty) {
		m_viewModelProperty->setValue(newValue);
	}
	m_updating = false;
}

// NativeBindingFactory implementation

NativeBindingFactory::NativeBindingFactory() = default;

NativeBindingFactory::~NativeBindingFactory() = default;

std::unique_ptr<INativeBinding> NativeBindingFactory::createBinding(
	INativeProperty *viewModelProperty,
	INativeProperty *viewProperty,
	INativeBinding::Direction direction)
{
	if (!viewModelProperty || !viewProperty) {
		return nullptr;
	}

	return std::make_unique<NativeBindingBase>(viewModelProperty, viewProperty, direction);
}

}
