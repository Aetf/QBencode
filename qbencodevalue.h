#ifndef QBENCODEVALUE_H
#define QBENCODEVALUE_H

#include <QString>
#include <QList>
#include <QHash>

class QVariant;
class QDebug;
class QByteArray;
class QBencodeValue;

typedef QHash<QString, QBencodeValue> QBencodeDict;
typedef QList<QBencodeValue> QBencodeList;

class QBencodeValue
{
public:
    enum Type {
        String = 0x0,
        Integer = 0x1,
        List = 0x2,
        Dict = 0x3,
        Undefined = 0x80
    };

    QBencodeValue(Type = Undefined);
    QBencodeValue(const QString &s);
    QBencodeValue(const QByteArray &bytes);
    QBencodeValue(int n);
    QBencodeValue(qint64 n);
    QBencodeValue(const QBencodeList &list);
    QBencodeValue(const QBencodeDict &dict);

    ~QBencodeValue();

    QBencodeValue(const QBencodeValue &other);
    QBencodeValue &operator =(const QBencodeValue &other);

    static QBencodeValue fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    inline Type type() const { return t; }
    inline bool isString() const { return type() == String; }
    inline bool isInteger() const { return type() == Integer; }
    inline bool isList() const { return type() == List; }
    inline bool isDict() const { return type() == Dict; }
    inline bool isUndefined() const { return type() == Undefined; }

    qint64 toInteger(qint64 defaultValue = 0) const;
    QString toString(const QString &defaultValue = QString()) const;
    QByteArray toByteArray(const QByteArray &defaultValue = QByteArray()) const;
    QBencodeList toList() const;
    QBencodeList toList(const QBencodeList &defaultValue) const;
    QBencodeDict toDict() const;
    QBencodeDict toDict(const QBencodeDict &defaultValue) const;

    bool operator==(const QBencodeValue &other) const;
    bool operator!=(const QBencodeValue &other) const;

private:
    friend QDebug operator<<(QDebug, const QBencodeValue &);

    void extractBytesFromQString(const QString &s);
    void detach();
    void copyFrom(const QBencodeValue &other);

    Type t;
    QVariant *d;
    QBencodeList *list;
    QBencodeDict *dict;
};

QDebug operator<<(QDebug, const QBencodeValue &);

#endif // QBENCODEVALUE_H
