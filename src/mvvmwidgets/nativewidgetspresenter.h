#ifndef QTMVVM_NATIVEWIDGETSPRESENTER_H
#define QTMVVM_NATIVEWIDGETSPRESENTER_H

#include <QtCore/qobject.h>
#include <QtMvvmCore/nativepresenter.h>
#include <QtMvvmCore/presenteradapter.h>
#include "QtMvvmWidgets/widgetspresenter.h"
#include "QtMvvmWidgets/qtwidgetadapter.h"
#include "QtMvvmWidgets/qtmvvmwidgets_global.h"

namespace QtMvvm {

//! Native presenter implementation that uses Qt Widgets under the hood
class Q_MVVMWIDGETS_EXPORT NativeWidgetsPresenter : public QObject, public NativePresenterBase
{
	Q_OBJECT

public:
	//! Constructor
	Q_INVOKABLE explicit NativeWidgetsPresenter(QObject *parent = nullptr);
	~NativeWidgetsPresenter() override;

	//! Register this presenter as the active presenter for the CoreApp
	static void registerAsPresenter();

	// INativePresenter interface implementation
	void present(ViewModel *viewModel, 
	            const std::map<std::string, std::string> &params, 
	            ViewModel *parentViewModel = nullptr) override;
	void showDialog(const NativeMessageConfig &config, MessageResult *result) override;

	//! Get the underlying Qt widgets presenter
	WidgetsPresenter* widgetsPresenter() const;

private:
	WidgetsPresenter *m_widgetsPresenter;
	std::unique_ptr<PresenterAdapter> m_adapter;
};

}

#endif // QTMVVM_NATIVEWIDGETSPRESENTER_H
