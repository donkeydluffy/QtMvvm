#ifndef QTMVVM_BINDINGADAPTER_H
#define QTMVVM_BINDINGADAPTER_H

#include <QtCore/qobject.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qpointer.h>

#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/binding.h"
#include "QtMvvmCore/nativebinding.h"

namespace QtMvvm {

//! Adapter to make Qt property work as INativeProperty
class Q_MVVMCORE_EXPORT QtPropertyAdapter : public INativeProperty
{
public:
	//! Constructor taking a QObject and property name
	QtPropertyAdapter(QObject *object, const char *propertyName);
	//! Constructor taking a QObject and QMetaProperty
	QtPropertyAdapter(QObject *object, const QMetaProperty &property);
	~QtPropertyAdapter() override;

	// INativeProperty interface implementation
	std::string name() const override;
	std::string getValue() const override;
	void setValue(const std::string &value) override;
	void registerChangeCallback(ChangeCallback callback) override;
	void unregisterChangeCallbacks() override;

	//! Get the underlying QObject
	QObject* object() const;
	
	//! Get the underlying property
	QMetaProperty property() const;

private:
	QPointer<QObject> m_object;
	QMetaProperty m_property;
	ChangeCallback m_callback;
	QMetaObject::Connection m_connection;
};

//! Adapter to make INativeBinding work as Qt Binding
class Q_MVVMCORE_EXPORT NativeBindingAdapter
{
public:
	//! Create a Qt Binding from native binding
	static Binding createQtBinding(std::unique_ptr<INativeBinding> nativeBinding);
	
	//! Create a native binding from Qt Binding
	static std::unique_ptr<INativeBinding> createNativeBinding(
		QObject *viewModel, const char *viewModelProperty,
		QObject *view, const char *viewProperty,
		INativeBinding::Direction direction = INativeBinding::TwoWay
	);
	
	//! Convert Qt Binding direction to native direction
	static INativeBinding::Direction convertDirection(Binding::BindingDirection qtDirection);
	
	//! Convert native direction to Qt Binding direction
	static Binding::BindingDirection convertDirection(INativeBinding::Direction nativeDirection);
};

}

#endif // QTMVVM_BINDINGADAPTER_H
