#include "qbencodewriter_p.h"
#include <QStringList>
#include <QByteArray>

using namespace QBencodePrivate;
static void valueToBencode(const QBencodeValue &value, QByteArray &bencode);

static void dictContentToBencode(const QBencodeDict &dict, QByteArray &bencode);
static void listContentToBencode(const QBencodeList &list, QByteArray &bencode);
static void stringToBencode(const QByteArray &str, QByteArray &bencode);

static void valueToBencode(const QBencodeValue &value, QByteArray &bencode)
{
    auto type = value.type();
    switch (type) {
        case QBencodeValue::String:
            stringToBencode(value.toByteArray(), bencode);
            break;
        case QBencodeValue::Integer:
            bencode += "i";
            bencode += QByteArray::number(value.toInteger());
            bencode += "e";
            break;
        case QBencodeValue::List:
            bencode += "l";
            listContentToBencode(value.toList(), bencode);
            bencode += "e";
            break;
        case QBencodeValue::Dict:
            bencode += "d";
            dictContentToBencode(value.toDict(), bencode);
            bencode += "e";
            break;
        default:
            bencode += "";
    }
}

static void listContentToBencode(const QBencodeList& list, QByteArray& bencode)
{
    if (list.isEmpty()) return;

    for(auto val : list) {
        valueToBencode(val, bencode);
    }
}

static void dictContentToBencode(const QBencodeDict& dict, QByteArray& bencode)
{
    if (dict.isEmpty()) return;

    QStringList keys(dict.keys());
    keys.sort();

    for(auto key : keys) {
        stringToBencode(key.toUtf8(), bencode);
        valueToBencode(dict[key], bencode);
    }
}

static void stringToBencode(const QByteArray& str, QByteArray& bencode)
{
    bencode += QByteArray::number(str.size());
    bencode += ":";
    bencode += str;
}

void Writer::toBencode(const QBencodeValue &value, QByteArray &bencode)
{
    bencode.clear();
    valueToBencode(value, bencode);
}
