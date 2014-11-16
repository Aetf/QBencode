#ifndef QBENCODE_H
#define QBENCODE_H

#include "qbencodevalue.h"

struct QBencodeParseError
{
    enum ParseError {
        NoError =0,
        UnterminatedList,
        UnterminatedInteger,
        IllegalString,
        IllegalNumber,
        IllegalValue,
        DeepNesting,
        DocumentTooLarge,
        DocumentIncomplete,
        LeadingZero,
    };

    QString errorString() const;

    int offset;
    ParseError error;
};

namespace QBencodePrivate {
    class Parser;
}

class QBencodeDocument
{
public:
    QBencodeDocument();
    explicit QBencodeDocument(const QBencodeValue &val);
    ~QBencodeDocument();

    QBencodeDocument(const QBencodeDocument &other);
    QBencodeDocument &operator =(const QBencodeDocument &other);


    static QBencodeDocument fromRawData(const char *data, int size, QBencodeParseError *error = nullptr, bool strictMode = true);
    const char *rawData(int *size) const;

    static QBencodeDocument fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    static QBencodeDocument fromBencode(const QByteArray &ben, QBencodeParseError *error = nullptr, bool strictMode = true);
    QByteArray toBencode() const;

    bool isEmpty() const;
    bool isNull() const;

    QBencodeValue value() const;

    void setValue(const QBencodeValue &value);

    bool operator==(const QBencodeDocument &other) const;
    bool operator!=(const QBencodeDocument &other) const { return !(*this == other); }

private:
    QBencodeDocument(QBencodeValue *value) :root(value) {}
    friend class QBencodeValue;
    friend class QBencodePrivate::Parser;
    friend QDebug operator<<(QDebug, const QBencodeDocument &);

    QBencodeValue *root;
};

QDebug operator<<(QDebug, const QBencodeDocument &);

#endif // QBENCODE_H
