#ifndef QTMVVM_NATIVEMESSAGE_H
#define QTMVVM_NATIVEMESSAGE_H

#include <string>
#include <functional>
#include "QtMvvmCore/qtmvvmcore_global.h"

namespace QtMvvm {

//! Pure C++ interface for message result without Qt dependencies
class Q_MVVMCORE_EXPORT INativeMessageResult
{
public:
	virtual ~INativeMessageResult() = default;

	//! Checks if the dialog has a result
	virtual bool hasResult() const = 0;
	
	//! Get the result value as string
	virtual std::string result() const = 0;
	
	//! Set the result value
	virtual void setResult(const std::string &result) = 0;
	
	//! Get auto-delete flag
	virtual bool autoDelete() const = 0;
	
	//! Set auto-delete flag
	virtual void setAutoDelete(bool autoDelete) = 0;
	
	//! Complete the dialog with a button code
	virtual void complete(int button) = 0;
	
	//! Complete the dialog with button and result
	virtual void complete(int button, const std::string &result) = 0;
	
	//! Discard the message/dialog
	virtual void discardMessage() = 0;
	
	//! Callback type for dialog completion
	using CompletionCallback = std::function<void(int button)>;
	
	//! Set callback to be invoked when dialog is completed
	virtual void setCompletionCallback(CompletionCallback callback) = 0;
	
	//! Callback type for close target
	using CloseCallback = std::function<void()>;
	
	//! Set callback for closing the dialog
	virtual void setCloseCallback(CloseCallback callback) = 0;
};

//! Pure C++ interface for progress control without Qt dependencies
class Q_MVVMCORE_EXPORT INativeProgressControl
{
public:
	virtual ~INativeProgressControl() = default;

	//! Get auto-delete flag
	virtual bool autoDelete() const = 0;
	
	//! Set auto-delete flag
	virtual void setAutoDelete(bool autoDelete) = 0;
	
	//! Check if progress is indeterminate
	virtual bool isIndeterminate() const = 0;
	
	//! Set indeterminate mode
	virtual void setIndeterminate(bool indeterminate) = 0;
	
	//! Get minimum value
	virtual int minimum() const = 0;
	
	//! Set minimum value
	virtual void setMinimum(int minimum) = 0;
	
	//! Get maximum value
	virtual int maximum() const = 0;
	
	//! Set maximum value
	virtual void setMaximum(int maximum) = 0;
	
	//! Get current progress value
	virtual int progress() const = 0;
	
	//! Set current progress value
	virtual void setProgress(int progress) = 0;
	
	//! Set progress as percentage (0.0 to 1.0)
	virtual void setProgress(double progressPercent) = 0;
	
	//! Close the progress dialog
	virtual void close() = 0;
	
	//! Update the label text
	virtual void updateLabel(const std::string &text) = 0;
	
	//! Callback type for cancel notification
	using CancelCallback = std::function<void(int button)>;
	
	//! Set callback for when user cancels
	virtual void setCancelCallback(CancelCallback callback) = 0;
	
	//! Callback type for close notification
	using CloseCallback = std::function<void()>;
	
	//! Set callback for when dialog closes
	virtual void setCloseCallback(CloseCallback callback) = 0;
};

//! Base implementation of INativeMessageResult
class Q_MVVMCORE_EXPORT NativeMessageResultBase : public INativeMessageResult
{
public:
	NativeMessageResultBase();
	~NativeMessageResultBase() override;

	// INativeMessageResult interface implementation
	bool hasResult() const override;
	std::string result() const override;
	void setResult(const std::string &result) override;
	bool autoDelete() const override;
	void setAutoDelete(bool autoDelete) override;
	void complete(int button) override;
	void complete(int button, const std::string &result) override;
	void discardMessage() override;
	void setCompletionCallback(CompletionCallback callback) override;
	void setCloseCallback(CloseCallback callback) override;

protected:
	//! Invoke the close callback
	void invokeCloseCallback();

private:
	std::string m_result;
	bool m_hasResult;
	bool m_autoDelete;
	CompletionCallback m_completionCallback;
	CloseCallback m_closeCallback;
};

//! Base implementation of INativeProgressControl
class Q_MVVMCORE_EXPORT NativeProgressControlBase : public INativeProgressControl
{
public:
	NativeProgressControlBase();
	~NativeProgressControlBase() override;

	// INativeProgressControl interface implementation
	bool autoDelete() const override;
	void setAutoDelete(bool autoDelete) override;
	bool isIndeterminate() const override;
	void setIndeterminate(bool indeterminate) override;
	int minimum() const override;
	void setMinimum(int minimum) override;
	int maximum() const override;
	void setMaximum(int maximum) override;
	int progress() const override;
	void setProgress(int progress) override;
	void setProgress(double progressPercent) override;
	void close() override;
	void updateLabel(const std::string &text) override;
	void setCancelCallback(CancelCallback callback) override;
	void setCloseCallback(CloseCallback callback) override;

protected:
	//! Invoke the cancel callback
	void invokeCancelCallback(int button);
	
	//! Invoke the close callback
	void invokeCloseCallback();

private:
	bool m_autoDelete;
	bool m_indeterminate;
	int m_minimum;
	int m_maximum;
	int m_progress;
	CancelCallback m_cancelCallback;
	CloseCallback m_closeCallback;
};

}

#endif // QTMVVM_NATIVEMESSAGE_H
