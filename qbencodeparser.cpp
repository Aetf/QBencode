#ifndef QT_BOOTSTRAPPED
#include <QCoreApplication>
#endif
#include <QDebug>
#include "qbencodeparser_p.h"

//#define PARSER_DEBUG
#ifdef PARSER_DEBUG
static int indent = 0;
#define BEGIN qDebug() << QByteArray(4*indent++, ' ').constData() << "pos=" << current
#define END --indent
#define DEBUG qDebug() << QByteArray(4*indent, ' ').constData()
#else
#define BEGIN if (1) ; else qDebug()
#define END do {} while (0)
#define DEBUG if (1) ; else qDebug()
#endif

static const int nestingLimit = 1024;

// error strings for the Bencode parser
#define BENCODEERR_OK QT_TRANSLATE_NOOP("QBencodeParserError", "no error occurred")
#define BENCODEERR_ILLEGAL_VAL QT_TRANSLATE_NOOP("QBencodeParserError", "illegal value")
#define BENCODEERR_DEEP_NEST QT_TRANSLATE_NOOP("QBencodeParserError", "too deeply nested document")

/*!
  \class QBencodeParseError

  \brief The QBencodeParseError class is used to report errors during Bencode parsing.

*/

/*!
  \enum QBencodeParseError::ParseError

  This enum describes the type of error that occurred during the parsing of a Bencode document.

  \value NoError        No error occurred
  \value IllegalValue   The value is illegal
  \value DeepNesting    The Bencode document is too deeply nested for the parser to parse it
*/

/*!
  \variable QBencodeParseError::error

  Contains the type of the parse error. Is equal to QBencodeParseError::NoError if the document
  was parsed correctly.

  \sa ParseError, errorString()
 */

/*!
  \variable QBencodeParseError::offset

  Contains the offset in the input data where the parse error occurred.

  \sa error, errorString()
*/

/*!
  Returns the human-readable message appropriate to the reported Bencode parsing error.

  \sa error
*/
QString QBencodeParseError::errorString() const
{
    const char *sz = "";
    switch (error) {
    case NoError:
        sz = BENCODEERR_OK;
        break;
    case DeepNesting:
        sz = BENCODEERR_DEEP_NEST;
        break;
    case UnterminatedList:
        sz = "";
        break;
    case UnterminatedInteger:
        sz = "";
        break;
    case IllegalString:
        sz = "";
        break;
    case IllegalNumber:
        sz = "";
        break;
    case IllegalValue:
        sz = "";
        break;
    case DocumentTooLarge:
        sz = "";
        break;
    case DocumentIncomplete:
        sz = "";
        break;
    case LeadingZero:
        sz = "";
        break;
    }
#ifndef QT_BOOTSTRAPPED
    return QCoreApplication::translate("QBencodeParseError", sz);
#else
    return QLatin1String(sz);
#endif
}

using namespace QBencodePrivate;

Parser::Parser(const char *ben, int length)
    : strictMode(false),
      head(ben), bencode(ben),
      data(nullptr), dataLength(0), current(0),
      nestingLevel(0), lastError(QBencodeParseError::NoError)
{
    end = bencode + length;
}

enum {
    BeginInteger = 'i',
    BeginDict = 'd',
    BeginList = 'l',
    Digit0 = '0',
    Digit1 = '1',
    Digit2 = '2',
    Digit3 = '3',
    Digit4 = '4',
    Digit5 = '5',
    Digit6 = '6',
    Digit7 = '7',
    Digit8 = '8',
    Digit9 = '9',
    EndMark = 'e',
    StringSeparator = ':',
};

char Parser::nextToken()
{
    if (bencode >= end)
        return 0;
    char token = *bencode++;
    return token;
}

bool Parser::eat(char token)
{
    if (end - bencode > 1 &&
            bencode[0] == token) {
        bencode++;
        return true;
    }
    return false;
}

bool Parser::reachEnd()
{
    return (bencode >= end);
}

QBencodeDocument Parser::parse(QBencodeParseError *error, bool strictMode)
{
#ifdef PARSER_DEBUG
    indent = 0;
    qDebug() << ">>>>> parser begin";
#endif
    this->strictMode = strictMode;
    if (parseValue()) {
        if (error) {
            error->offset = 0;
            error->error = QBencodeParseError::NoError;
        }
        return QBencodeDocument(currentValue);
    } else {
#ifdef PARSER_DEBUG
        qDebug() << ">>>>> parser error";
#endif
        if (error) {
            error->offset = bencode - head;
            error->error = lastError;
        }
        return QBencodeDocument();
    }
}

bool Parser::parseValue()
{
    char token = nextToken();
    bool ret = false;

    DEBUG << hex << (uint)token;
    switch (token) {
    case BeginInteger:
        ret = parseInteger();
    case BeginList:
        ret = parseList();
    case BeginDict:
        ret = parseDict();
    case Digit0:
    case Digit1:
    case Digit2:
    case Digit3:
    case Digit4:
    case Digit5:
    case Digit6:
    case Digit7:
    case Digit8:
    case Digit9:
        ret = parseString();
    default:
        lastError = QBencodeParseError::IllegalValue;
    }
    END;
    return ret;
}

/*
 * list = begin-list [ value *( value ) ] end-mark
 */
bool Parser::parseList()
{
    BEGIN << "parseList";

    if (++nestingLevel > nestingLimit) {
        lastError = QBencodeParseError::DeepNesting;
        return false;
    }

    if (reachEnd()) {
        lastError = QBencodeParseError::UnterminatedList;
        return false;
    }

    QBencodeList values;
    if (*bencode == EndMark) {
        nextToken();
    } else {
        while (true) {
            if (!parseValue())
                return false;
            values.append(*currentValue);
            char token = nextToken();
            if (token == EndMark) {
                break;
            } else if (reachEnd()) {
                lastError = QBencodeParseError::UnterminatedList;
                return false;
            }
        }
    }
    currentValue = new QBencodeValue(values);

    DEBUG << "size =" << values.size();
    DEBUG << "pos =" << (bencode - head);
    END;

    --nestingLevel;
    return true;
}

/*
 * integer = begin-integer number end-mark
 */
bool Parser::parseInteger()
{
    BEGIN << "parseInteger" << bencode;

    qint64 n;
    if (!parseNumber(&n))
        return false;

    // eat EndMark
    if (!eat(EndMark)) {
        lastError = QBencodeParseError::UnterminatedInteger;
        return false;
    }

    currentValue = new QBencodeValue(n);

    END;
    return true;
}

bool Parser::parseNumber(qint64 *n)
{
    const char *start = bencode;

    // minus
    eat('-');

    // leading zero check
    if (strictMode) {
        int zeroCnt = 0;
        while (eat('0')) {
            ++zeroCnt;
        }
        if (zeroCnt > 1) {
            lastError = QBencodeParseError::LeadingZero;
            return false;
        }
    }

    while (!reachEnd() && *bencode >= '0' && *bencode <= '9')
        ++bencode;

    QByteArray number(start, bencode - start);
    DEBUG << "number" << number;

    bool ok;
    *n = number.toLongLong(&ok);
    if (!ok) {
        lastError = QBencodeParseError::IllegalNumber;
        return false;
    }
    return true;
}

/*
 * string = number string-saperator string-data
 */
bool Parser::parseString()
{
    BEGIN << "parseString" << bencode;
    // put back begin token, this should be safe as this method is only
    // called after at least one call to nextToken()
    --bencode;

    qint64 len;
    if (!parseNumber(&len))
        return false;

    if (!eat(StringSeparator)) {
        lastError = QBencodeParseError::IllegalString;
        return false;
    }

    if (bencode + len > end) {
        lastError = QBencodeParseError::DocumentIncomplete;
        return false;
    }

    QByteArray string(bencode, len);
    DEBUG << "string" << string;

    currentValue = new QBencodeValue(string);
    END;
    return true;
}

bool Parser::parseDict()
{
    BEGIN << "parseDict";

    // TODO: Parser::parseDict method stub
    DEBUG << "stub";

    END;
    return true;
}
