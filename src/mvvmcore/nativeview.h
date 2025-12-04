#ifndef QTMVVM_NATIVEVIEW_H
#define QTMVVM_NATIVEVIEW_H

#include <string>
#include <memory>
#include <functional>
#include "QtMvvmCore/qtmvvmcore_global.h"

namespace QtMvvm {

// Forward declarations
class ViewModel;

//! Pure C++ interface for a view without Qt dependencies
class Q_MVVMCORE_EXPORT INativeView
{
public:
	virtual ~INativeView() = default;

	//! Shows the view
	virtual void show() = 0;
	
	//! Hides the view
	virtual void hide() = 0;
	
	//! Closes the view
	virtual void close() = 0;
	
	//! Checks if the view is currently visible
	virtual bool isVisible() const = 0;
	
	//! Sets the window title or view identifier
	virtual void setTitle(const std::string &title) = 0;
	
	//! Gets the window title or view identifier
	virtual std::string title() const = 0;
	
	//! Gets the associated ViewModel pointer (can be null)
	virtual void* viewModelPtr() const = 0;
	
	//! Sets the parent view for this view
	virtual void setParentView(INativeView *parent) = 0;
	
	//! Gets the parent view (can be null)
	virtual INativeView* parentView() const = 0;
};

//! Interface for views that can present child views
class Q_MVVMCORE_EXPORT INativePresentingView
{
public:
	virtual ~INativePresentingView() = default;
	
	//! Attempts to present a child view
	//! @return true if the view was successfully presented
	virtual bool tryPresent(INativeView *view) = 0;
};

//! Factory interface for creating native views
class Q_MVVMCORE_EXPORT INativeViewFactory
{
public:
	virtual ~INativeViewFactory() = default;
	
	//! Creates a view instance for the given view model
	//! @param viewModel The view model to create a view for
	//! @param parentView Optional parent view
	//! @return A new view instance or nullptr if creation failed
	virtual INativeView* createView(ViewModel *viewModel, INativeView *parentView = nullptr) = 0;
	
	//! Checks if this factory can create a view for the given view model type
	//! @param viewModelTypeName The type name of the view model
	//! @return true if this factory can create a view for the view model
	virtual bool canCreateView(const std::string &viewModelTypeName) const = 0;
};

}

#endif // QTMVVM_NATIVEVIEW_H
