#ifndef QTMVVM_VIEWMODELADAPTER_H
#define QTMVVM_VIEWMODELADAPTER_H

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/qvariant.h>

#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/viewmodel.h"
#include "QtMvvmCore/nativeviewmodel.h"

namespace QtMvvm {

//! Adapter to make Qt ViewModel work as INativeViewModel
class Q_MVVMCORE_EXPORT ViewModelToNativeAdapter : public INativeViewModel
{
public:
	//! Constructor taking a Qt ViewModel
	explicit ViewModelToNativeAdapter(ViewModel *qtViewModel);
	~ViewModelToNativeAdapter() override;

	// INativeViewModel interface implementation
	void onInit(const std::map<std::string, std::string> &params) override;
	void onResult(uint32_t requestCode, const std::string &result) override;
	void setResultCallback(ResultCallback callback) override;
	void setInstanceInvokedCallback(InstanceInvokedCallback callback) override;

	//! Get the underlying Qt ViewModel
	ViewModel* qtViewModel() const;

	//! Convert native params to Qt QVariantHash
	static QVariantHash convertParamsToQt(const std::map<std::string, std::string> &params);
	
	//! Convert Qt QVariantHash to native params
	static std::map<std::string, std::string> convertParamsFromQt(const QVariantHash &params);

private:
	QPointer<ViewModel> m_qtViewModel;
	QMetaObject::Connection m_resultConnection;
	QMetaObject::Connection m_instanceConnection;
};

//! Adapter to make INativeViewModel work as Qt ViewModel
class Q_MVVMCORE_EXPORT NativeToViewModelAdapter : public ViewModel
{
	Q_OBJECT

public:
	//! Constructor taking a native ViewModel
	Q_INVOKABLE explicit NativeToViewModelAdapter(INativeViewModel *nativeViewModel, QObject *parent = nullptr);
	~NativeToViewModelAdapter() override;

	// ViewModel interface implementation
	void onInit(const QVariantHash &params) override;
	void onResult(quint32 requestCode, const QVariant &result) override;

	//! Get the underlying native ViewModel
	INativeViewModel* nativeViewModel() const;

private:
	INativeViewModel *m_nativeViewModel;
};

}

#endif // QTMVVM_VIEWMODELADAPTER_H
