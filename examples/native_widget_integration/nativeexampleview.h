#ifndef NATIVEEXAMPLEVIEW_H
#define NATIVEEXAMPLEVIEW_H

#include <QMainWindow>
#include <QtMvvmCore/viewmodeladapter.h>
#include "nativeexampleviewmodel.h"

namespace Ui {
class NativeExampleView;
}

// This is a Qt Widget that uses a NATIVE ViewModel (via adapter)
class NativeExampleView : public QMainWindow
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit NativeExampleView(QtMvvm::ViewModel *viewModel, QWidget *parent = nullptr);
    ~NativeExampleView();

private slots:
    void onIncrementClicked();
    void onResetClicked();
    void onShowMessageClicked();
    void onTestProgressClicked();
    void onNameEdited(const QString &text);
    void onActiveToggled(bool checked);

private:
    void updateUI();

    Ui::NativeExampleView *ui;
    NativeExampleViewModel *m_nativeViewModel;
    QtMvvm::NativeToViewModelAdapter *m_adapter;
};

#endif // NATIVEEXAMPLEVIEW_H
