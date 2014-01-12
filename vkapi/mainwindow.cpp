#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("https://oauth.vk.com/authorize?client_id=4103316&scope=PERMISSIONSI&display=page&v=5.5&redirect_uri=https://oauth.vk.com/blank.html&response_type=token"));
    ui->label->clear();

    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(checkUrl(QUrl)));
    connect(this, SIGNAL(auth_success(QString)), this, SLOT(getFriends(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::checkUrl(QUrl url) {
    url = url.toString().replace("#", "?");
    QUrlQuery query(url);
    QString token = query.queryItemValue("access_token");
    if (!token.isEmpty()) {
        emit auth_success(token);
    }
}

void MainWindow::getFriends(QString token)
{
    ui->label->setText(token);
}
