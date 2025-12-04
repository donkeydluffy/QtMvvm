#include "nativeexampleview.h"
#include "ui_nativeexampleview.h"
#include <QMessageBox>
#include <iostream>

NativeExampleView::NativeExampleView(QtMvvm::ViewModel *viewModel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NativeExampleView)
    , m_nativeViewModel(nullptr)
    , m_adapter(nullptr)
{
    ui->setupUi(this);
    
    // Check if we received an adapter or need to create one
    m_adapter = dynamic_cast<QtMvvm::NativeToViewModelAdapter*>(viewModel);
    
    if (m_adapter) {
        // We have an adapter, get the native ViewModel
        m_nativeViewModel = dynamic_cast<NativeExampleViewModel*>(m_adapter->nativeViewModel());
        std::cout << "[View] Received adapted native ViewModel" << std::endl;
    } else {
        std::cout << "[View] No adapter found, viewModel received directly" << std::endl;
    }
    
    if (!m_nativeViewModel) {
        QMessageBox::critical(this, "Error", "Failed to get native ViewModel!");
        return;
    }
    
    // Setup property change callbacks (Native -> View)
    m_nativeViewModel->setNameChangedCallback([this](const std::string &name) {
        ui->nameEdit->setText(QString::fromStdString(name));
    });
    
    m_nativeViewModel->setCounterChangedCallback([this](int counter) {
        ui->counterLabel->setText(QString::number(counter));
    });
    
    m_nativeViewModel->setActiveChangedCallback([this](bool active) {
        ui->activeCheckBox->setChecked(active);
    });
    
    // Setup result callback
    m_nativeViewModel->setResultCallback([this](const std::string &result) {
        QMessageBox::information(this, "Result", 
            QString::fromStdString("ViewModel result: " + result));
    });
    
    // Connect UI signals
    connect(ui->incrementButton, &QPushButton::clicked, this, &NativeExampleView::onIncrementClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &NativeExampleView::onResetClicked);
    connect(ui->showMessageButton, &QPushButton::clicked, this, &NativeExampleView::onShowMessageClicked);
    connect(ui->testProgressButton, &QPushButton::clicked, this, &NativeExampleView::onTestProgressClicked);
    connect(ui->nameEdit, &QLineEdit::textChanged, this, &NativeExampleView::onNameEdited);
    connect(ui->activeCheckBox, &QCheckBox::toggled, this, &NativeExampleView::onActiveToggled);
    
    // Initialize UI with current values
    updateUI();
    
    std::cout << "[View] NativeExampleView created successfully" << std::endl;
}

NativeExampleView::~NativeExampleView()
{
    delete ui;
}

void NativeExampleView::onIncrementClicked()
{
    if (m_nativeViewModel) {
        m_nativeViewModel->incrementCounter();
    }
}

void NativeExampleView::onResetClicked()
{
    if (m_nativeViewModel) {
        m_nativeViewModel->resetCounter();
    }
}

void NativeExampleView::onShowMessageClicked()
{
    if (m_nativeViewModel) {
        m_nativeViewModel->showMessage();
    }
}

void NativeExampleView::onTestProgressClicked()
{
    if (m_nativeViewModel) {
        m_nativeViewModel->testProgress();
        QMessageBox::information(this, "Progress", "Progress test triggered (check console output)");
    }
}

void NativeExampleView::onNameEdited(const QString &text)
{
    if (m_nativeViewModel) {
        m_nativeViewModel->setName(text.toStdString());
    }
}

void NativeExampleView::onActiveToggled(bool checked)
{
    if (m_nativeViewModel) {
        m_nativeViewModel->setActive(checked);
    }
}

void NativeExampleView::updateUI()
{
    if (!m_nativeViewModel) return;
    
    ui->nameEdit->setText(QString::fromStdString(m_nativeViewModel->getName()));
    ui->counterLabel->setText(QString::number(m_nativeViewModel->getCounter()));
    ui->activeCheckBox->setChecked(m_nativeViewModel->isActive());
}
