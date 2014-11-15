#include <QCoreApplication>
#include <QDebug>
#include "qbencodedocument.h"

using std::endl;
int main(int argc, char *argv[])
{
    QByteArray bencode("li12e4:abcde");
    QBencodeParseError err;
    QBencodeDocument doc = QBencodeDocument::fromBencode(bencode, &err);

    qDebug() << err.errorString();
    qDebug() << doc;
    return 0;
}
