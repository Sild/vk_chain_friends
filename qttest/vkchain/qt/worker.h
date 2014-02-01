#ifndef WORKER_H
#define WORKER_H

#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>

#include <QDebug>

#include <vector>

#include "json.h"
#include "queue.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const std::vector<int>& id);

    void sendToGetFriends(int user_id);

signals:
    void finished();
    void finded();

public slots:
    void catchFriends(QNetworkReply* reply);
    void process();
    void stop();

private:
    int size;
    int current;
    std::vector<int> ids;
};

#endif // WORKER_H
