#include "qtwidgetadapter.h"
#include <QtMvvmCore/viewmodel.h>

namespace QtMvvm {

// QtWidgetAdapter implementation

QtWidgetAdapter::QtWidgetAdapter(QWidget *widget)
	: m_widget(widget)
	, m_parentView(nullptr)
{
}

QtWidgetAdapter::~QtWidgetAdapter() = default;

void QtWidgetAdapter::show()
{
	if (m_widget) {
		m_widget->show();
	}
}

void QtWidgetAdapter::hide()
{
	if (m_widget) {
		m_widget->hide();
	}
}

void QtWidgetAdapter::close()
{
	if (m_widget) {
		m_widget->close();
	}
}

bool QtWidgetAdapter::isVisible() const
{
	return m_widget ? m_widget->isVisible() : false;
}

void QtWidgetAdapter::setTitle(const std::string &title)
{
	if (m_widget) {
		m_widget->setWindowTitle(QString::fromStdString(title));
	}
}

std::string QtWidgetAdapter::title() const
{
	return m_widget ? m_widget->windowTitle().toStdString() : std::string();
}

void* QtWidgetAdapter::viewModelPtr() const
{
	if (!m_widget) {
		return nullptr;
	}
	
	// Try to find the ViewModel in the widget's parent hierarchy
	// Note: This assumes the ViewModel is set as the parent QObject of the widget,
	// which is the standard QtMvvm pattern. If custom view creation doesn't follow
	// this pattern, this method may return nullptr.
	auto vm = qobject_cast<ViewModel*>(m_widget->parent());
	return vm;
}

void QtWidgetAdapter::setParentView(INativeView *parent)
{
	m_parentView = parent;
	if (m_widget && parent) {
		auto parentWidget = getWidget(parent);
		if (parentWidget) {
			m_widget->setParent(parentWidget);
		}
	}
}

INativeView* QtWidgetAdapter::parentView() const
{
	return m_parentView;
}

QWidget* QtWidgetAdapter::widget() const
{
	return m_widget.data();
}

QtWidgetAdapter* QtWidgetAdapter::createAdapter(QWidget *widget)
{
	return widget ? new QtWidgetAdapter(widget) : nullptr;
}

QWidget* QtWidgetAdapter::getWidget(INativeView *view)
{
	auto adapter = dynamic_cast<QtWidgetAdapter*>(view);
	return adapter ? adapter->widget() : nullptr;
}

// QtWidgetViewFactory implementation

QtWidgetViewFactory::QtWidgetViewFactory()
	: m_creators()
{
}

QtWidgetViewFactory::~QtWidgetViewFactory() = default;

void QtWidgetViewFactory::registerWidgetCreator(const std::string &viewModelTypeName, WidgetCreator creator)
{
	m_creators[viewModelTypeName] = creator;
}

INativeView* QtWidgetViewFactory::createView(ViewModel *viewModel, INativeView *parentView)
{
	if (!viewModel) {
		return nullptr;
	}

	// Get the view model type name
	std::string typeName = viewModel->metaObject()->className();
	
	// Find the creator
	auto it = m_creators.find(typeName);
	if (it == m_creators.end()) {
		return nullptr;
	}

	// Get parent widget if available
	QWidget *parentWidget = nullptr;
	if (parentView) {
		parentWidget = QtWidgetAdapter::getWidget(parentView);
	}

	// Create the widget
	auto widget = it->second(viewModel, parentWidget);
	if (!widget) {
		return nullptr;
	}

	// Create and return the adapter
	return new QtWidgetAdapter(widget);
}

bool QtWidgetViewFactory::canCreateView(const std::string &viewModelTypeName) const
{
	return m_creators.find(viewModelTypeName) != m_creators.end();
}

}
