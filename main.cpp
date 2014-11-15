#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

using std::endl;
int main(int argc, char *argv[])
{
    qint64 li = 123;
    QVariant data(li);
    QJsonDocument doc;
    qDebug() << "doc isEmpty:" << doc.isEmpty() << " isNull:" << doc.isNull();
    qDebug() << data << endl;
    qDebug() << data.type() << endl;
    return 0;
}
