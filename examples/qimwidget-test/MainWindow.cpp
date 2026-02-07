#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "ImTestWidget.h"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new ImTestWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
