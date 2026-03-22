#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converter.h"
#include <QDoubleValidator>
#include <QSignalBlocker>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDoubleValidator *val = new QDoubleValidator(this);
    val->setLocale(QLocale::English);

    ui->editLenTop->setValidator(val);
    ui->editLenBot->setValidator(val);
    ui->editMassTop->setValidator(val);
    ui->editMassBot->setValidator(val);
    ui->editTempTop->setValidator(val);
    ui->editTempBot->setValidator(val);

    connect(ui->editLenTop, &QLineEdit::textEdited, this, [=]() { updateCalculation(true); });
    connect(ui->editLenBot, &QLineEdit::textEdited, this, [=]() { updateCalculation(false); });
    connect(ui->editMassTop, &QLineEdit::textEdited, this, [=]() { updateCalculation(true); });
    connect(ui->editMassBot, &QLineEdit::textEdited, this, [=]() { updateCalculation(false); });
    connect(ui->editTempTop, &QLineEdit::textEdited, this, [=]() { updateCalculation(true); });
    connect(ui->editTempBot, &QLineEdit::textEdited, this, [=]() { updateCalculation(false); });

    connect(ui->comboLenTop, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });
    connect(ui->comboLenBot, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });
    connect(ui->comboMassTop, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });
    connect(ui->comboMassBot, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });
    connect(ui->comboTempTop, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });
    connect(ui->comboTempBot, &QComboBox::currentIndexChanged, this, [=]() { updateCalculation(true); });

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=]() { updateCalculation(true); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCalculation(bool fromTop)
{
    int index = ui->tabWidget->currentIndex();
    QLineEdit *src, *dst;
    QComboBox *sUnit, *dUnit;

    if (index == 0) {
        src = fromTop ? ui->editLenTop : ui->editLenBot;
        dst = fromTop ? ui->editLenBot : ui->editLenTop;
        sUnit = fromTop ? ui->comboLenTop : ui->comboLenBot;
        dUnit = fromTop ? ui->comboLenBot : ui->comboLenTop;
    } else if (index == 1) {
        src = fromTop ? ui->editMassTop : ui->editMassBot;
        dst = fromTop ? ui->editMassBot : ui->editMassTop;
        sUnit = fromTop ? ui->comboMassTop : ui->comboMassBot;
        dUnit = fromTop ? ui->comboMassBot : ui->comboMassTop;
    } else {
        src = fromTop ? ui->editTempTop : ui->editTempBot;
        dst = fromTop ? ui->editTempBot : ui->editTempTop;
        sUnit = fromTop ? ui->comboTempTop : ui->comboTempBot;
        dUnit = fromTop ? ui->comboTempBot : ui->comboTempTop;
    }

    bool ok;
    double input = src->text().toDouble(&ok);

    if (!ok) {
        dst->clear();
        return;
    }

    Converter::Mode mode = static_cast<Converter::Mode>(index);
    double result = Converter::convert(input, sUnit->currentText(), dUnit->currentText(), mode);

    QSignalBlocker blocker(dst);
    dst->setText(QString::number(result, 'g', 10));
}
