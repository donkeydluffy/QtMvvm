#include "messageadapter.h"
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

namespace QtMvvm {

// MessageResultToNativeAdapter implementation

MessageResultToNativeAdapter::MessageResultToNativeAdapter(MessageResult *qtMessageResult)
	: m_qtMessageResult(qtMessageResult)
	, m_dialogDoneConnection()
{
}

MessageResultToNativeAdapter::~MessageResultToNativeAdapter()
{
	if (m_dialogDoneConnection) {
		QObject::disconnect(m_dialogDoneConnection);
	}
}

bool MessageResultToNativeAdapter::hasResult() const
{
	return m_qtMessageResult ? m_qtMessageResult->hasResult() : false;
}

std::string MessageResultToNativeAdapter::result() const
{
	if (!m_qtMessageResult) {
		return std::string();
	}
	return m_qtMessageResult->result().toString().toStdString();
}

void MessageResultToNativeAdapter::setResult(const std::string &result)
{
	if (m_qtMessageResult) {
		m_qtMessageResult->setResult(QString::fromStdString(result));
	}
}

bool MessageResultToNativeAdapter::autoDelete() const
{
	return m_qtMessageResult ? m_qtMessageResult->autoDelete() : true;
}

void MessageResultToNativeAdapter::setAutoDelete(bool autoDelete)
{
	if (m_qtMessageResult) {
		m_qtMessageResult->setAutoDelete(autoDelete);
	}
}

void MessageResultToNativeAdapter::complete(int button)
{
	if (m_qtMessageResult) {
		m_qtMessageResult->complete(static_cast<MessageConfig::StandardButton>(button));
	}
}

void MessageResultToNativeAdapter::complete(int button, const std::string &result)
{
	if (m_qtMessageResult) {
		m_qtMessageResult->complete(
			static_cast<MessageConfig::StandardButton>(button),
			QString::fromStdString(result)
		);
	}
}

void MessageResultToNativeAdapter::discardMessage()
{
	if (m_qtMessageResult) {
		m_qtMessageResult->discardMessage();
	}
}

void MessageResultToNativeAdapter::setCompletionCallback(CompletionCallback callback)
{
	if (!m_qtMessageResult) {
		return;
	}

	// Disconnect previous connection
	if (m_dialogDoneConnection) {
		QObject::disconnect(m_dialogDoneConnection);
	}

	// Connect to Qt signal
	if (callback) {
		m_dialogDoneConnection = QObject::connect(m_qtMessageResult.data(), &MessageResult::dialogDone,
			[callback](MessageConfig::StandardButton button) {
				callback(static_cast<int>(button));
			});
	}
}

void MessageResultToNativeAdapter::setCloseCallback(CloseCallback callback)
{
	// Qt MessageResult doesn't have a direct close callback
	// It uses the dialogDone signal for completion
	// This is a limitation of the adapter
	(void)callback;
}

MessageResult* MessageResultToNativeAdapter::qtMessageResult() const
{
	return m_qtMessageResult.data();
}

// NativeToMessageResultAdapter implementation

NativeToMessageResultAdapter::NativeToMessageResultAdapter(INativeMessageResult *nativeMessageResult)
	: MessageResult()
	, m_nativeMessageResult(nativeMessageResult)
{
	if (m_nativeMessageResult) {
		// Set up completion callback to emit Qt signal
		m_nativeMessageResult->setCompletionCallback([this](int button) {
			emit dialogDone(static_cast<MessageConfig::StandardButton>(button), QPrivateSignal());
		});
		
		// Sync auto-delete property
		setAutoDelete(m_nativeMessageResult->autoDelete());
	}
}

NativeToMessageResultAdapter::~NativeToMessageResultAdapter() = default;

INativeMessageResult* NativeToMessageResultAdapter::nativeMessageResult() const
{
	return m_nativeMessageResult;
}

// ProgressControlToNativeAdapter implementation

ProgressControlToNativeAdapter::ProgressControlToNativeAdapter(ProgressControl *qtProgressControl)
	: m_qtProgressControl(qtProgressControl)
	, m_canceledConnection()
	, m_closedConnection()
{
}

ProgressControlToNativeAdapter::~ProgressControlToNativeAdapter()
{
	if (m_canceledConnection) {
		QObject::disconnect(m_canceledConnection);
	}
	if (m_closedConnection) {
		QObject::disconnect(m_closedConnection);
	}
}

bool ProgressControlToNativeAdapter::autoDelete() const
{
	return m_qtProgressControl ? m_qtProgressControl->autoDelete() : true;
}

void ProgressControlToNativeAdapter::setAutoDelete(bool autoDelete)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setAutoDelete(autoDelete);
	}
}

bool ProgressControlToNativeAdapter::isIndeterminate() const
{
	return m_qtProgressControl ? m_qtProgressControl->isIndeterminate() : false;
}

void ProgressControlToNativeAdapter::setIndeterminate(bool indeterminate)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setIndeterminate(indeterminate);
	}
}

int ProgressControlToNativeAdapter::minimum() const
{
	return m_qtProgressControl ? m_qtProgressControl->minimum() : 0;
}

void ProgressControlToNativeAdapter::setMinimum(int minimum)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setMinimum(minimum);
	}
}

int ProgressControlToNativeAdapter::maximum() const
{
	return m_qtProgressControl ? m_qtProgressControl->maximum() : 100;
}

void ProgressControlToNativeAdapter::setMaximum(int maximum)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setMaximum(maximum);
	}
}

int ProgressControlToNativeAdapter::progress() const
{
	return m_qtProgressControl ? m_qtProgressControl->progress() : 0;
}

void ProgressControlToNativeAdapter::setProgress(int progress)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setProgress(progress);
	}
}

void ProgressControlToNativeAdapter::setProgress(double progressPercent)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->setProgress(progressPercent);
	}
}

void ProgressControlToNativeAdapter::close()
{
	if (m_qtProgressControl) {
		m_qtProgressControl->close();
	}
}

void ProgressControlToNativeAdapter::updateLabel(const std::string &text)
{
	if (m_qtProgressControl) {
		m_qtProgressControl->updateLabel(QString::fromStdString(text));
	}
}

void ProgressControlToNativeAdapter::setCancelCallback(CancelCallback callback)
{
	if (!m_qtProgressControl) {
		return;
	}

	// Disconnect previous connection
	if (m_canceledConnection) {
		QObject::disconnect(m_canceledConnection);
	}

	// Connect to Qt signal
	if (callback) {
		m_canceledConnection = QObject::connect(m_qtProgressControl.data(), &ProgressControl::canceled,
			[callback](MessageConfig::StandardButton button) {
				callback(static_cast<int>(button));
			});
	}
}

void ProgressControlToNativeAdapter::setCloseCallback(CloseCallback callback)
{
	if (!m_qtProgressControl) {
		return;
	}

	// Disconnect previous connection
	if (m_closedConnection) {
		QObject::disconnect(m_closedConnection);
	}

	// Connect to Qt signal
	if (callback) {
		m_closedConnection = QObject::connect(m_qtProgressControl.data(), &ProgressControl::closed,
			[callback]() {
				callback();
			});
	}
}

ProgressControl* ProgressControlToNativeAdapter::qtProgressControl() const
{
	return m_qtProgressControl.data();
}

// NativeToProgressControlAdapter implementation

NativeToProgressControlAdapter::NativeToProgressControlAdapter(INativeProgressControl *nativeProgressControl, QObject *parent)
	: ProgressControl(parent)
	, m_nativeProgressControl(nativeProgressControl)
{
	if (m_nativeProgressControl) {
		// Set up cancel callback to emit Qt signal
		m_nativeProgressControl->setCancelCallback([this](int button) {
			emit canceled(static_cast<MessageConfig::StandardButton>(button), QPrivateSignal());
		});
		
		// Set up close callback to emit Qt signal
		m_nativeProgressControl->setCloseCallback([this]() {
			emit closed(QPrivateSignal());
		});
		
		// Sync auto-delete property
		setAutoDelete(m_nativeProgressControl->autoDelete());
	}
}

NativeToProgressControlAdapter::~NativeToProgressControlAdapter() = default;

INativeProgressControl* NativeToProgressControlAdapter::nativeProgressControl() const
{
	return m_nativeProgressControl;
}

}
