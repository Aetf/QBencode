#ifndef QT_BOOTSTRAPPED
#include <QCoreApplication>
#endif
#include <QDebug>
#include "qbencodeparser_p.h"

#define PARSER_DEBUG
#ifdef PARSER_DEBUG
static int indent = 0;
#define BEGIN qDebug() << QByteArray(4*indent++, ' ').constData() << "pos = " << (bencode - head) << " begin: "
#define END qDebug() << QByteArray(4*(--indent), ' ').constData() << "end"
#define DEBUG qDebug() << QByteArray(4*indent, ' ').constData()
#else
#define BEGIN if (1) ; else qDebug()
#define END do {} while (0)
#define DEBUG if (1) ; else qDebug()
#endif

#define Return(a) do { state = a; goto Return; } while(0)

static const int nestingLimit = 1024;

// error strings for the Bencode parser
#define BENCODEERR_OK QT_TRANSLATE_NOOP("QBencodeParserError", "no error occurred")
#define BENCODEERR_UNTERM_INT QT_TRANSLATE_NOOP("QBencodeParserError", "unterminated integer")
#define BENCODEERR_UNTERM_LIST QT_TRANSLATE_NOOP("QBencodeParserError", "unterminated list")
#define BENCODEERR_UNTERM_DICT QT_TRANSLATE_NOOP("QBencodeParserError", "unterminated dictionary")
#define BENCODEERR_MISS_STRSEP QT_TRANSLATE_NOOP("QBencodeParserError", "string separator is missing after digits")
#define BENCODEERR_MISS_KEY QT_TRANSLATE_NOOP("QBencodeParserError", "missing string key inside a dict entry")
#define BENCODEERR_MISS_ENTRY QT_TRANSLATE_NOOP("QBencodeParserError", "entry is missing after a key inside a dict")
#define BENCODEERR_ILLEGAL_NUM QT_TRANSLATE_NOOP("QBencodeParserError", "illegal number")
#define BENCODEERR_ILLEGAL_VAL QT_TRANSLATE_NOOP("QBencodeParserError", "illegal value")
#define BENCODEERR_DEEP_NEST QT_TRANSLATE_NOOP("QBencodeParserError", "too deeply nested document")
#define BENCODEERR_DOC_LARGE QT_TRANSLATE_NOOP("QBencodeParserError", "too large document")
#define BENCODEERR_DOC_INCOM QT_TRANSLATE_NOOP("QBencodeParserError", "document incomplete")
#define BENCODEERR_LEAD_ZERO QT_TRANSLATE_NOOP("QBencodeParserError", "leading zero in integer entry")

/*!
  \class QBencodeParseError

  \brief The QBencodeParseError class is used to report errors during Bencode parsing.

*/

/*!
  \enum QBencodeParseError::ParseError

  This enum describes the type of error that occurred during the parsing of a Bencode document.

  \value NoError        No error occurred
  \value UnterminatedInteger An integer entry is not correctly terminated with a closing mark 'e'
  \value UnterminatedList A list entry is not correctly terminated with a closing mark 'e'
  \value UnterminatedDict A dictionary entry is not correctly terminated with a closing mark 'e'
  \value MissingStringSeparator A colon separating length from data inside strings is missing
  \value MissingKey     A string typed key was expected but couldn't be found inside a dictionary
  \value MissingEntry   An entry was expect after a key but the dictionary ended.
  \value IllegalNumber  The number is not well formed
  \value IllegalValue   The value is illegal
  \value DeepNesting    The Bencode document is too deeply nested for the parser to parse it
  \value DocumentTooLarge  The Bencode document is too large for the parser to parse it
  \value DocumentIncomplete  The Bencode document is possibily incomplete. (e.g. a string entry with a length pass the end of the document)
  \value LeadingZero    The integer entry has leading zero which is not allowed in Bencode format. This is only raised in strict mode.
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
    case UnterminatedInteger:
        sz = BENCODEERR_UNTERM_INT;
        break;
    case UnterminatedList:
        sz = BENCODEERR_UNTERM_LIST;
        break;
    case UnterminatedDict:
        sz = BENCODEERR_UNTERM_DICT;
        break;
    case MissingStringSeparator:
        sz = BENCODEERR_MISS_STRSEP;
        break;
    case MissingKey:
        sz = BENCODEERR_MISS_KEY;
        break;
    case MissingEntry:
        sz = BENCODEERR_MISS_ENTRY;
        break;
    case IllegalNumber:
        sz = BENCODEERR_ILLEGAL_NUM;
        break;
    case IllegalValue:
        sz = BENCODEERR_ILLEGAL_VAL;
        break;
    case DeepNesting:
        sz = BENCODEERR_DEEP_NEST;
        break;
    case DocumentTooLarge:
        sz = BENCODEERR_DOC_LARGE;
        break;
    case DocumentIncomplete:
        sz = BENCODEERR_DOC_INCOM;
        break;
    case LeadingZero:
        sz = BENCODEERR_LEAD_ZERO;
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
    if (reachEnd()) {
        return 0;
    }
    char token = *bencode++;
    return token;
}

char Parser::peek() const
{
    if (reachEnd()) {
        return 0;
    }
    return *bencode;
}

bool Parser::eat(char token)
{
    if (end - bencode >= 1 && bencode[0] == token) {
        bencode++;
        return true;
    }
    return false;
}

bool Parser::reachEnd() const
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
        return QBencodeDocument(std::move(currentValue));
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

    DEBUG << token << '(' << hex << (uint)token << ')';
    switch (token) {
    case BeginInteger:
        ret = parseInteger();
        break;
    case BeginList:
        ret = parseList();
        break;
    case BeginDict:
        ret = parseDict();
        break;
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
        break;
    default:
        lastError = QBencodeParseError::IllegalValue;
    }
    return ret;
}

/*
 * list = begin-list [ value *( value ) ] end-mark
 */
bool Parser::parseList()
{
    BEGIN << "parseList";
    bool state = true;

    if (++nestingLevel > nestingLimit) {
        lastError = QBencodeParseError::DeepNesting;
        Return(false);
    }

    if (reachEnd()) {
        lastError = QBencodeParseError::UnterminatedList;
        Return(false);
    }

    {
        QBencodeList values;
        if (peek() == EndMark) { // empty list
            nextToken();
        } else {
            while (true) {
                if (!parseValue()) {
                    Return(false);
                }
                values.append(std::move(currentValue));
                if (peek() == EndMark) {
                    break;
                } else if (reachEnd()) {
                    lastError = QBencodeParseError::UnterminatedList;
                    Return(false);
                }
            }
        }
        currentValue = QBencodeValue(values);
        DEBUG << "size =" << values.size();
    }

    DEBUG << "pos =" << (bencode - head);

Return:
    END;
    --nestingLevel;
    return state;
}

/*
 * integer = begin-integer number end-mark
 */
bool Parser::parseInteger()
{
    BEGIN << "parseInteger" << bencode;

    qint64 n;
    if (!parseNumber(&n)) {
        return false;
    }

    // eat EndMark
    if (!eat(EndMark)) {
        lastError = QBencodeParseError::UnterminatedInteger;
        return false;
    }

    currentValue = QBencodeValue(n);

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

    while (!reachEnd() && *bencode >= '0' && *bencode <= '9') {
        ++bencode;
    }

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
    if (!parseNumber(&len)) {
        return false;
    }

    if (!eat(StringSeparator)) {
        lastError = QBencodeParseError::MissingStringSeparator;
        return false;
    }

    if (bencode + len > end) {
        lastError = QBencodeParseError::DocumentIncomplete;
        return false;
    }

    QByteArray string(bencode, len);
    DEBUG << "string" << string;

    bencode += len;
    currentValue = QBencodeValue(string);
    END;
    return true;
}

bool Parser::parseDict()
{
    BEGIN << "parseDict";
    bool state = true;

    if (++nestingLevel > nestingLimit) {
        lastError = QBencodeParseError::DeepNesting;
        Return(false);
    }

    if (reachEnd()) {
        lastError = QBencodeParseError::UnterminatedDict;
        Return(false);
    }

    {
        QBencodeDict entries;
        bool shouldBeValue = false;
        QBencodeValue key;
        if (peek() == EndMark) { // empty dict
            nextToken();
        } else {
            while (true) {
                if (!parseValue()) {
                    Return(false);
                }

                // FUTURE: check entry sequence in stric mode
                if (!shouldBeValue) {
                    if (currentValue.type() != QBencodeValue::String) {
                        lastError = QBencodeParseError::MissingKey;
                        Return(false);
                    }
                    key = currentValue;
                    shouldBeValue = true;
                } else {
                    entries.insert(key.toString(), currentValue);
                    shouldBeValue = false;
                }

                if (peek() == EndMark) {
                    if (shouldBeValue) {
                        lastError = QBencodeParseError::MissingEntry;
                        Return(false);
                    }
                    break;
                } else if (reachEnd()) {
                    lastError = QBencodeParseError::UnterminatedDict;
                    Return(false);
                }
            }
        }
        currentValue = QBencodeValue(entries);
        DEBUG << "size =" << entries.size();
    }

    DEBUG << "pos =" << (bencode - head);

Return:
    --nestingLevel;
    END;
    return state;
}
