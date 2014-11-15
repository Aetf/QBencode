#ifndef QBENCODE_H
#define QBENCODE_H

#include "qbencodevalue.h"

struct QBencodeParseError
{
    enum ParseError {
        NoError =0,
    };

    QString errorString() const;

    int offset;
    ParseError error;
};

class QBencodeDocument
{
public:
    QBencodeDocument();
    explicit QBencodeDocument(const QBencodeValue &val);
    ~QBencodeDocument();

    QBencodeDocument(const QBencodeDocument &other);
    QBencodeDocument &operator =(const QBencodeDocument &other);


    static QBencodeDocument fromRawData(const char *data, int size);
    const char *rawData(int *size) const;

    static QBencodeDocument fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    static QBencodeDocument fromBencodeBinary(const QByteArray &data, QBencodeParseError *error);
    QByteArray toBencodeByteArray() const;

    static QBencodeDocument fromBencodeString(const QString &str, QBencodeParseError *error = nullptr);
    QString toBencodeString() const;

    bool isEmpty() const;
    bool isNull() const;

    QBencodeValue value() const;

    void setValue(const QBencodeValue &value);

    bool operator==(const QBencodeDocument &other) const;
    bool operator!=(const QBencodeDocument &other) const { return !(*this == other); }

private:
    friend class QBencodeValue;
    friend QDebug operator<<(QDebug, const QBencodeDocument &);

    QBencodeValue *root;
};

QDebug operator<<(QDebug, const QBencodeDocument &);

#endif // QBENCODE_H
