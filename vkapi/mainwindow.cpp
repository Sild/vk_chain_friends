#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("https://oauth.vk.com/authorize?client_id=4103316&scope=PERMISSIONSI&display=page&v=5.5&redirect_uri=https://oauth.vk.com/blank.html&response_type=token"));
    ui->label->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}
