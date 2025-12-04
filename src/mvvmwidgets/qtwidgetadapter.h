#ifndef QTMVVM_QTWIDGETADAPTER_H
#define QTMVVM_QTWIDGETADAPTER_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qpointer.h>

#include "QtMvvmCore/nativeview.h"
#include "QtMvvmWidgets/qtmvvmwidgets_global.h"

namespace QtMvvm {

//! Adapter that makes a QWidget compatible with the INativeView interface
class Q_MVVMWIDGETS_EXPORT QtWidgetAdapter : public INativeView
{
public:
	//! Constructor taking a QWidget
	explicit QtWidgetAdapter(QWidget *widget);
	~QtWidgetAdapter() override;

	// INativeView interface implementation
	void show() override;
	void hide() override;
	void close() override;
	bool isVisible() const override;
	void setTitle(const std::string &title) override;
	std::string title() const override;
	void* viewModelPtr() const override;
	void setParentView(INativeView *parent) override;
	INativeView* parentView() const override;

	//! Get the underlying QWidget
	QWidget* widget() const;

	//! Static helper to create an adapter for a widget
	static QtWidgetAdapter* createAdapter(QWidget *widget);

	//! Static helper to get the widget from a native view (if it's a QtWidgetAdapter)
	static QWidget* getWidget(INativeView *view);

private:
	QPointer<QWidget> m_widget;
	INativeView *m_parentView;
};

//! View factory that creates QWidget-based views
class Q_MVVMWIDGETS_EXPORT QtWidgetViewFactory : public INativeViewFactory
{
public:
	//! Function type for creating widgets
	using WidgetCreator = std::function<QWidget*(ViewModel*, QWidget*)>;

	QtWidgetViewFactory();
	~QtWidgetViewFactory() override;

	//! Register a widget creator for a specific view model type
	void registerWidgetCreator(const std::string &viewModelTypeName, WidgetCreator creator);

	// INativeViewFactory interface implementation
	INativeView* createView(ViewModel *viewModel, INativeView *parentView = nullptr) override;
	bool canCreateView(const std::string &viewModelTypeName) const override;

private:
	std::map<std::string, WidgetCreator> m_creators;
};

}

#endif // QTMVVM_QTWIDGETADAPTER_H
