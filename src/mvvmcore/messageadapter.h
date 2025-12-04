#ifndef QTMVVM_MESSAGEADAPTER_H
#define QTMVVM_MESSAGEADAPTER_H

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/message.h"
#include "QtMvvmCore/nativemessage.h"

namespace QtMvvm {

//! Adapter to make Qt MessageResult work as INativeMessageResult
class Q_MVVMCORE_EXPORT MessageResultToNativeAdapter : public INativeMessageResult
{
public:
	//! Constructor taking a Qt MessageResult
	explicit MessageResultToNativeAdapter(MessageResult *qtMessageResult);
	~MessageResultToNativeAdapter() override;

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

	//! Get the underlying Qt MessageResult
	MessageResult* qtMessageResult() const;

private:
	QPointer<MessageResult> m_qtMessageResult;
	QMetaObject::Connection m_dialogDoneConnection;
};

//! Adapter to make INativeMessageResult work as Qt MessageResult
class Q_MVVMCORE_EXPORT NativeToMessageResultAdapter : public MessageResult
{
	Q_OBJECT

public:
	//! Constructor taking a native MessageResult
	Q_INVOKABLE explicit NativeToMessageResultAdapter(INativeMessageResult *nativeMessageResult);
	~NativeToMessageResultAdapter() override;

	//! Get the underlying native MessageResult
	INativeMessageResult* nativeMessageResult() const;

private:
	INativeMessageResult *m_nativeMessageResult;
};

//! Adapter to make Qt ProgressControl work as INativeProgressControl
class Q_MVVMCORE_EXPORT ProgressControlToNativeAdapter : public INativeProgressControl
{
public:
	//! Constructor taking a Qt ProgressControl
	explicit ProgressControlToNativeAdapter(ProgressControl *qtProgressControl);
	~ProgressControlToNativeAdapter() override;

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

	//! Get the underlying Qt ProgressControl
	ProgressControl* qtProgressControl() const;

private:
	QPointer<ProgressControl> m_qtProgressControl;
	QMetaObject::Connection m_canceledConnection;
	QMetaObject::Connection m_closedConnection;
};

//! Adapter to make INativeProgressControl work as Qt ProgressControl
class Q_MVVMCORE_EXPORT NativeToProgressControlAdapter : public ProgressControl
{
	Q_OBJECT

public:
	//! Constructor taking a native ProgressControl
	Q_INVOKABLE explicit NativeToProgressControlAdapter(INativeProgressControl *nativeProgressControl, QObject *parent = nullptr);
	~NativeToProgressControlAdapter() override;

	//! Get the underlying native ProgressControl
	INativeProgressControl* nativeProgressControl() const;

private:
	INativeProgressControl *m_nativeProgressControl;
};

}

#endif // QTMVVM_MESSAGEADAPTER_H
