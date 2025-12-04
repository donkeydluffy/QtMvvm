#include "nativepresenter.h"

namespace QtMvvm {

// NativeMessageConfig implementation
NativeMessageConfig::NativeMessageConfig(const std::string &type)
	: type(type)
{
}

// NativePresenterBase implementation
NativePresenterBase::NativePresenterBase()
	: m_viewFactories()
{
}

NativePresenterBase::~NativePresenterBase() = default;

void NativePresenterBase::registerViewFactory(std::shared_ptr<INativeViewFactory> factory)
{
	if (factory) {
		m_viewFactories.push_back(factory);
	}
}

bool NativePresenterBase::canPresent(const std::string &viewModelTypeName) const
{
	return findViewFactory(viewModelTypeName) != nullptr;
}

INativeViewFactory* NativePresenterBase::findViewFactory(const std::string &viewModelTypeName) const
{
	for (const auto &factory : m_viewFactories) {
		if (factory && factory->canCreateView(viewModelTypeName)) {
			return factory.get();
		}
	}
	return nullptr;
}

const std::vector<std::shared_ptr<INativeViewFactory>>& NativePresenterBase::viewFactories() const
{
	return m_viewFactories;
}

}
