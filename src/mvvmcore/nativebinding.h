#ifndef QTMVVM_NATIVEBINDING_H
#define QTMVVM_NATIVEBINDING_H

#include <string>
#include <functional>
#include <memory>
#include "QtMvvmCore/qtmvvmcore_global.h"

namespace QtMvvm {

//! Pure C++ interface for property binding without Qt dependencies
class Q_MVVMCORE_EXPORT INativeBinding
{
public:
	virtual ~INativeBinding() = default;

	//! Binding direction flags
	enum Direction {
		SingleInit = 0x01,        //!< Only initialize the view by setting it to the viewmodel's value on creation
		OneWayToView = 0x02 | SingleInit,  //!< All changes done to the viewmodel are propagated to the view
		OneWayToViewModel = 0x04, //!< All changes done to the view are propagated to the viewmodel
		TwoWay = OneWayToView | OneWayToViewModel  //!< All changes on both sides are propagated to the other one
	};

	//! Specifies whether the binding is a valid and active binding
	virtual bool isValid() const = 0;

	//! Remove the binding by disconnecting all change handlers
	virtual void unbind() = 0;
};

//! Property accessor interface for getting and setting values
class Q_MVVMCORE_EXPORT INativeProperty
{
public:
	virtual ~INativeProperty() = default;

	//! Get the property name
	virtual std::string name() const = 0;

	//! Get the property value as string
	virtual std::string getValue() const = 0;

	//! Set the property value from string
	virtual void setValue(const std::string &value) = 0;

	//! Callback type for property change notification
	using ChangeCallback = std::function<void(const std::string &newValue)>;

	//! Register a callback to be invoked when property changes
	virtual void registerChangeCallback(ChangeCallback callback) = 0;

	//! Unregister all change callbacks
	virtual void unregisterChangeCallbacks() = 0;
};

//! Factory for creating native bindings between properties
class Q_MVVMCORE_EXPORT INativeBindingFactory
{
public:
	virtual ~INativeBindingFactory() = default;

	//! Create a binding between two properties
	//! @param viewModelProperty The viewmodel property to bind
	//! @param viewProperty The view property to bind
	//! @param direction The binding direction
	//! @return A new binding instance or nullptr if creation failed
	virtual std::unique_ptr<INativeBinding> createBinding(
		INativeProperty *viewModelProperty,
		INativeProperty *viewProperty,
		INativeBinding::Direction direction = INativeBinding::TwoWay
	) = 0;
};

//! Base implementation of INativeBinding
class Q_MVVMCORE_EXPORT NativeBindingBase : public INativeBinding
{
public:
	NativeBindingBase(INativeProperty *viewModelProperty,
	                  INativeProperty *viewProperty,
	                  Direction direction);
	~NativeBindingBase() override;

	// INativeBinding interface implementation
	bool isValid() const override;
	void unbind() override;

protected:
	//! Called when viewmodel property changes
	virtual void onViewModelPropertyChanged(const std::string &newValue);
	
	//! Called when view property changes
	virtual void onViewPropertyChanged(const std::string &newValue);

private:
	INativeProperty *m_viewModelProperty;
	INativeProperty *m_viewProperty;
	Direction m_direction;
	bool m_bound;
	bool m_updating; // Prevent circular updates
};

//! Default implementation of INativeBindingFactory
class Q_MVVMCORE_EXPORT NativeBindingFactory : public INativeBindingFactory
{
public:
	NativeBindingFactory();
	~NativeBindingFactory() override;

	std::unique_ptr<INativeBinding> createBinding(
		INativeProperty *viewModelProperty,
		INativeProperty *viewProperty,
		INativeBinding::Direction direction = INativeBinding::TwoWay
	) override;
};

}

#endif // QTMVVM_NATIVEBINDING_H
