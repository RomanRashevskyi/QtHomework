#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewTriggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitTriggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutTriggered);

    connect(ui->actionCut, &QAction::triggered, ui->editor, &QTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->editor, &QTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->editor, &QTextEdit::paste);
    connect(ui->actionSelect_All, &QAction::triggered, ui->editor, &QTextEdit::selectAll);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewTriggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, "New Document", "Enter document name:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        ui->editor->clear();
        ui->statusbar->showMessage("Created: " + name, 3000);
    }
}

void MainWindow::onOpenTriggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File", "", "Text files (*.txt);;All files (*)");
    if (!path.isEmpty()) {
        ui->statusbar->showMessage("Opened: " + path, 3000);
    }
}

void MainWindow::onSaveTriggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save File", "", "Text files (*.txt);;All files (*)");
    if (!path.isEmpty()) {
        ui->statusbar->showMessage("Saved to: " + path, 3000);
    }
}

void MainWindow::onExitTriggered()
{
    auto reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }
}

void MainWindow::onAboutTriggered()
{
    QMessageBox::about(this, "About QuickPad", "QuickPad is a simple text editor developed with Qt.");
}
