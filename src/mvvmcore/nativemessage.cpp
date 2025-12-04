#include "nativemessage.h"

namespace QtMvvm {

// NativeMessageResultBase implementation

NativeMessageResultBase::NativeMessageResultBase()
	: m_result()
	, m_hasResult(false)
	, m_autoDelete(true)
	, m_completionCallback()
	, m_closeCallback()
{
}

NativeMessageResultBase::~NativeMessageResultBase() = default;

bool NativeMessageResultBase::hasResult() const
{
	return m_hasResult;
}

std::string NativeMessageResultBase::result() const
{
	return m_result;
}

void NativeMessageResultBase::setResult(const std::string &result)
{
	m_result = result;
	m_hasResult = true;
}

bool NativeMessageResultBase::autoDelete() const
{
	return m_autoDelete;
}

void NativeMessageResultBase::setAutoDelete(bool autoDelete)
{
	m_autoDelete = autoDelete;
}

void NativeMessageResultBase::complete(int button)
{
	if (m_completionCallback) {
		m_completionCallback(button);
	}
}

void NativeMessageResultBase::complete(int button, const std::string &result)
{
	setResult(result);
	complete(button);
}

void NativeMessageResultBase::discardMessage()
{
	invokeCloseCallback();
}

void NativeMessageResultBase::setCompletionCallback(CompletionCallback callback)
{
	m_completionCallback = callback;
}

void NativeMessageResultBase::setCloseCallback(CloseCallback callback)
{
	m_closeCallback = callback;
}

void NativeMessageResultBase::invokeCloseCallback()
{
	if (m_closeCallback) {
		m_closeCallback();
	}
}

// NativeProgressControlBase implementation

NativeProgressControlBase::NativeProgressControlBase()
	: m_autoDelete(true)
	, m_indeterminate(false)
	, m_minimum(0)
	, m_maximum(100)
	, m_progress(0)
	, m_cancelCallback()
	, m_closeCallback()
{
}

NativeProgressControlBase::~NativeProgressControlBase() = default;

bool NativeProgressControlBase::autoDelete() const
{
	return m_autoDelete;
}

void NativeProgressControlBase::setAutoDelete(bool autoDelete)
{
	m_autoDelete = autoDelete;
}

bool NativeProgressControlBase::isIndeterminate() const
{
	return m_indeterminate;
}

void NativeProgressControlBase::setIndeterminate(bool indeterminate)
{
	m_indeterminate = indeterminate;
}

int NativeProgressControlBase::minimum() const
{
	return m_minimum;
}

void NativeProgressControlBase::setMinimum(int minimum)
{
	m_minimum = minimum;
}

int NativeProgressControlBase::maximum() const
{
	return m_maximum;
}

void NativeProgressControlBase::setMaximum(int maximum)
{
	m_maximum = maximum;
}

int NativeProgressControlBase::progress() const
{
	return m_progress;
}

void NativeProgressControlBase::setProgress(int progress)
{
	m_progress = progress;
}

void NativeProgressControlBase::setProgress(double progressPercent)
{
	if (m_indeterminate) {
		return;
	}
	int range = m_maximum - m_minimum;
	m_progress = m_minimum + static_cast<int>(progressPercent * range);
}

void NativeProgressControlBase::close()
{
	invokeCloseCallback();
}

void NativeProgressControlBase::updateLabel(const std::string &text)
{
	// Base implementation does nothing
	// Subclasses should override to update UI
	(void)text;
}

void NativeProgressControlBase::setCancelCallback(CancelCallback callback)
{
	m_cancelCallback = callback;
}

void NativeProgressControlBase::setCloseCallback(CloseCallback callback)
{
	m_closeCallback = callback;
}

void NativeProgressControlBase::invokeCancelCallback(int button)
{
	if (m_cancelCallback) {
		m_cancelCallback(button);
	}
}

void NativeProgressControlBase::invokeCloseCallback()
{
	if (m_closeCallback) {
		m_closeCallback();
	}
}

}
