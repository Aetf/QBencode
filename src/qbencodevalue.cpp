#include <QVariant>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include "qbencodevalue.h"

QBencodeValue::QBencodeValue(Type type)
    : d(nullptr), list(nullptr), dict(nullptr)
{
    t = type;
}

QBencodeValue::QBencodeValue(const QString &s)
    : list(nullptr), dict(nullptr)
{
    t = String;
    extractBytesFromQString(s);
}

QBencodeValue::QBencodeValue(const QByteArray &bytes)
    : list(nullptr), dict(nullptr)
{
    t = String;
    d = new QVariant(bytes);
}

QBencodeValue::QBencodeValue(int n)
    : list(nullptr), dict(nullptr)
{
    t = Integer;
    d = new QVariant(n);
}

QBencodeValue::QBencodeValue(qint64 n)
    : list(nullptr), dict(nullptr)
{
    t = Integer;
    d = new QVariant(n);
}

QBencodeValue::QBencodeValue(const QBencodeList &list)
    : d(nullptr), dict(nullptr)
{
    t = List;
    this->list = new QBencodeList(list);
}

QBencodeValue::QBencodeValue(const QBencodeDict &dict)
    : d(nullptr), list(nullptr)
{
    t = Dict;
    this->dict = new QBencodeDict(dict);
}

QBencodeValue::~QBencodeValue()
{
    detach();
}

QBencodeValue::QBencodeValue(const QBencodeValue &other)
    : d(nullptr), list(nullptr), dict(nullptr)
{
    deepCopyFrom(other);
}

QBencodeValue &QBencodeValue::operator =(const QBencodeValue &other)
{
    detach();
    deepCopyFrom(other);
    return *this;
}

QBencodeValue::QBencodeValue(QBencodeValue &&other)
    : t(Null), d(nullptr), list(nullptr), dict(nullptr)
{
    auto tmpT = t;
    auto tmpD = d;
    auto tmpDict = dict;
    auto tmpList = list;
    t = other.t;
    d = other.d;
    dict = other.dict;
    list = other.list;
    other.t = tmpT;
    other.d = tmpD;
    other.dict = tmpDict;
    other.list = tmpList;
}

QBencodeValue &QBencodeValue::operator =(QBencodeValue &&other)
{
    auto tmpT = t;
    auto tmpD = d;
    auto tmpDict = dict;
    auto tmpList = list;
    t = other.t;
    d = other.d;
    dict = other.dict;
    list = other.list;
    other.t = tmpT;
    other.d = tmpD;
    other.dict = tmpDict;
    other.list = tmpList;

    return *this;
}

/*static*/ QBencodeValue QBencodeValue::fromVariant(const QVariant &variant)
{
    switch (variant.type()) {
    case QMetaType::QString:
        return QBencodeValue(variant.toString());
    case QMetaType::QByteArray:
        return QBencodeValue(variant.toByteArray());
    case QMetaType::Int:
    case QMetaType::LongLong:
        return QBencodeValue(variant.toLongLong());
    case QMetaType::QVariantList: {
        QBencodeList benlist;
        for (auto item : variant.toList()) {
            benlist.append(fromVariant(item));
        }
        return QBencodeValue(benlist);
    }
    case QMetaType::QVariantHash: {
        QBencodeDict bendict;
        QHash<QString, QVariant> hash = variant.toHash();
        for (auto key : hash.keys()) {
            bendict.insert(key, fromVariant(hash[key]));
        }
        return QBencodeValue(bendict);
    }
    default:
        return QBencodeValue();
    }
}

QVariant QBencodeValue::toVariant() const
{
    switch (t) {
    case String:
    case Integer:
        return QVariant(*d);
    case Null:
    case Undefined:
    default:
        return QVariant();
    case List:
    {
        QVariantList varList;
        for (auto val : *list) {
            varList.append(val.toVariant());
        }
        return varList;
    }
    case Dict:
    {
        QVariantHash varHash;
        for (auto key : dict->keys()) {
            varHash.insert(key, (*dict)[key].toVariant());
        }
        return varHash;
    }
    }
}

qint64 QBencodeValue::toInteger(qint64 defaultValue) const
{
    if (t == Integer) {
        return d->toLongLong();
    } else {
        return defaultValue;
    }
}

/*!
  Returns the value as string, assume UTF-8 encoded.
 */
QString QBencodeValue::toString(const QString &defaultValue) const
{
    if (t == String) {
        return QString(d->toByteArray());
    } else {
        return defaultValue;
    }
}

QByteArray QBencodeValue::toByteArray(const QByteArray &defaultValue) const
{
    if (t == String) {
        return d->toByteArray();
    } else {
        return defaultValue;
    }
}

QBencodeList QBencodeValue::toList() const
{
    if (t == List) {
        return *list;
    } else {
        return QBencodeList();
    }
}

QBencodeList QBencodeValue::toList(const QBencodeList &defaultValue) const
{
    if (t == List) {
        return *list;
    } else {
        return defaultValue;
    }
}

QBencodeDict QBencodeValue::toDict() const
{
    if (t == Dict) {
        return *dict;
    } else {
        return QBencodeDict();
    }
}

QBencodeDict QBencodeValue::toDict(const QBencodeDict &defaultValue) const
{
    if (t == Dict) {
        return *dict;
    } else {
        return defaultValue;
    }
}

bool QBencodeValue::operator==(const QBencodeValue &other) const
{
    if (t != other.t) { return false; }
    switch (t) {
    case Integer:
    case String:
        return *d == *other.d;
    case List:
        return *list == *other.list;
    case Dict:
        return *dict == *other.dict;
    case Undefined:
        return true;
    default:
        return false;
    }
}

bool QBencodeValue::operator!=(const QBencodeValue &other) const
{
    return !(*this == other);
}

void QBencodeValue::extractBytesFromQString(const QString &s)
{
    d = new QVariant(s.toUtf8());
}

void QBencodeValue::detach()
{
    if (d != nullptr) {
        delete d;
        d = nullptr;
    }
    if (list != nullptr) {
        delete list;
        list = nullptr;
    }
    if (dict != nullptr) {
        delete dict;
        dict = nullptr;
    }
    t = Undefined;
}

void QBencodeValue::deepCopyFrom(const QBencodeValue &other)
{
    t = other.t;
    switch (t) {
    case Dict:
        dict = new QBencodeDict(*other.dict);
        break;
    case List:
        list = new QBencodeList(*other.list);
        break;
    case Undefined:
    case Null:
        break;
    default:
        d = new QVariant(*other.d);
        break;
    }
}

QDebug operator<<(QDebug out, const QBencodeValue &obj)
{
    out << "QBencodeValue(";
    switch (obj.t) {
    case QBencodeValue::Undefined:
        out << "Undefined";
        break;
    case QBencodeValue::Null:
        out << "Null";
        break;
    case QBencodeValue::String:
        out << "String, " << obj.toByteArray();
        break;
    case QBencodeValue::Integer:
        out << "Integer, " << obj.toInteger();
        break;
    case QBencodeValue::List:
        out << "List, " << obj.toList();
        break;
    case QBencodeValue::Dict:
        out << "Dict, " << obj.toDict();
        break;
    }
    out << ")";
    return out;
}
