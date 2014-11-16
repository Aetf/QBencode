#ifndef QBENCODEPARSER_P_H
#define QBENCODEPARSER_P_H

#include <QVarLengthArray>
#include "qbencodedocument.h"

namespace QBencodePrivate {

class Parser
{
public:
    Parser(const char *ben, int length);

    QBencodeDocument parse(QBencodeParseError *error, bool strictMode = false);

private:
    inline char nextToken();
    inline bool eat(char token);
    inline QBencodeValue *takeCurrentValue();
    inline char peek() const;
    inline bool reachEnd() const;

    bool parseValue();

    bool parseString();
    bool parseInteger();
    bool parseList();
    bool parseDict();
    bool parseEntry();

    bool parseNumber(qint64 *n);

    bool strictMode;

    const char *head;
    const char *bencode;
    const char *end;

    QBencodeValue currentValue;

    char *data;
    int dataLength;
    int current;
    int nestingLevel;
    QBencodeParseError::ParseError lastError;

    inline int reserveSpace(int space)
    {
        if (current + space >= dataLength) {
            dataLength = 2 * dataLength + space;
            data = (char *)realloc(data, dataLength);
        }
        int pos = current;
        current += space;
        return pos;
    }
};

}

#endif // QBENCODEPARSER_P_H
