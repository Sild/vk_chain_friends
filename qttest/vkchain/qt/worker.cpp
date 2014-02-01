#include "worker.h"
#include <iostream>

Worker::Worker(const std::vector<int>& id) :
   ids(id), size(id.size()), current(0)
{
}

void Worker::sendToGetFriends(int user_id)
{
   QUrl url("https://api.vk.com/method/friends.get?user_id=" + QString::number(user_id));
   QNetworkAccessManager* manager = new QNetworkAccessManager(this);
   QNetworkRequest request(url);

   connect( manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(catchFriends(QNetworkReply*)));
   manager->get(request);
}

void Worker::catchFriends(QNetworkReply* reply)
{
    QByteArray answer = reply->readAll();
    reply->deleteLater();
    qDebug() << answer;
    std::cout << "one more\n";

    QString str(answer);
    std::string result(str.toStdString());

    std::vector<int> r;
    bool isFinded = false;
    if(!result.empty()) {

        json::Object obj(json::Deserialize(result));
        json::Value response(obj["response"]);

        if (response.GetType() == json::ArrayVal) {
            json::Array ids(response);
            for (
                 std::vector< json::Value >::const_iterator iter = ids.begin(), end = ids.end();
                 iter != end; ++iter
                 ) {
                if (iter->IsNumeric()) {
                    r.push_back(*iter);
                    if (Queue::ins().isAim(*iter)) isFinded = true;
                }
            }
        }
    }
    Queue::ins().push(1, r);
    if (isFinded)
        emit finded();
    if (--current == 0)
        emit finished();
}

void Worker::process()
{
    for (unsigned i = 0; i < size; ++i) {
        sendToGetFriends(ids.at(i));
        ++current;
    }
}

void Worker::stop()
{
}
