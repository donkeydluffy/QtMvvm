#ifndef QTMVVM_PRESENTERADAPTER_H
#define QTMVVM_PRESENTERADAPTER_H

#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>
#include <QtCore/qpointer.h>

#include "QtMvvmCore/qtmvvmcore_global.h"
#include "QtMvvmCore/ipresenter.h"
#include "QtMvvmCore/nativepresenter.h"
#include "QtMvvmCore/message.h"

namespace QtMvvm {

//! Adapter to bridge between IPresenter (Qt-based) and INativePresenter (pure C++)
class Q_MVVMCORE_EXPORT PresenterAdapter : public IPresenter
{
public:
	//! Constructor taking a native presenter
	explicit PresenterAdapter(INativePresenter *nativePresenter);
	~PresenterAdapter() override;

	// IPresenter interface implementation
	void present(ViewModel *viewModel, const QVariantHash &params, QPointer<ViewModel> parent = nullptr) override;
	void showDialog(const MessageConfig &config, MessageResult *result) override;

	//! Get the underlying native presenter
	INativePresenter* nativePresenter() const;

	//! Convert Qt QVariantHash to native string map
	static std::map<std::string, std::string> convertParamsToNative(const QVariantHash &params);
	
	//! Convert native string map to Qt QVariantHash
	static QVariantHash convertParamsFromNative(const std::map<std::string, std::string> &params);
	
	//! Convert MessageConfig to NativeMessageConfig
	static NativeMessageConfig convertMessageConfigToNative(const MessageConfig &config);
	
	//! Convert NativeMessageConfig to MessageConfig
	static MessageConfig convertMessageConfigFromNative(const NativeMessageConfig &config);

private:
	INativePresenter *m_nativePresenter;
};

//! Adapter to bridge from INativePresenter to IPresenter
class Q_MVVMCORE_EXPORT NativePresenterAdapter : public INativePresenter
{
public:
	//! Constructor taking a Qt presenter
	explicit NativePresenterAdapter(IPresenter *qtPresenter);
	~NativePresenterAdapter() override;

	// INativePresenter interface implementation
	void present(ViewModel *viewModel, 
	            const std::map<std::string, std::string> &params, 
	            ViewModel *parentViewModel = nullptr) override;
	void showDialog(const NativeMessageConfig &config, MessageResult *result) override;
	void registerViewFactory(std::shared_ptr<INativeViewFactory> factory) override;
	bool canPresent(const std::string &viewModelTypeName) const override;

	//! Get the underlying Qt presenter
	IPresenter* qtPresenter() const;

private:
	IPresenter *m_qtPresenter;
	std::vector<std::shared_ptr<INativeViewFactory>> m_viewFactories;
};

}

#endif // QTMVVM_PRESENTERADAPTER_H
