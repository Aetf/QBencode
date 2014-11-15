#include <QDebug>
#include <QVariant>
#include "qbencodedocument.h"

/*! \class QBencodeDocument

    \brief The QBencodeDocument class provides a way to read and write Bencode data.

    QBencodeDocument is a class that wraps a complete Bencode document and can read and
    write this document both from a UTF-8 encoded text based representation as well
    as binary data in case of the existance of non-string string entries.

    A Bencode document can be converted from its text-based representation to a QBencodeDocument
    using QBencodeDocument::fromBencodeString(). toBencodeString() converts it back to text. The
    parser is very fast and efficient and converts the Bencode to internal binary representation.

    Validity of the parsed document can be queried with !isNull().

    A document can be queried as to whether it contains data using isEmpty(). The value contained
    in the document can be retrieved using value() and then read or manipulated.
*/

/*!
 * Constructs an empty and invalid document.
 */
QBencodeDocument::QBencodeDocument()
    :root(nullptr)
{
}

/*!
 * Creates a QBencodeDocument from \a val
 */
QBencodeDocument::QBencodeDocument(const QBencodeValue &val)
    :root(nullptr)
{
    setValue(val);
}

/*!
 * Deletes the document.
 */
QBencodeDocument::~QBencodeDocument()
{
    if (root != nullptr) {
        delete root;
    }
}

/*!
 * Creates a copy of the \a other document.
 */
QBencodeDocument::QBencodeDocument(const QBencodeDocument &other)
{
    root = new QBencodeValue(*other.root);
}

/*!
 * Assigns the \a other document to this QBencodeDocument.
 * Returns a reference to this object.
 */
QBencodeDocument& QBencodeDocument::operator =(const QBencodeDocument &other)
{
    if (root != nullptr) {
        delete root;
    }
    root = new QBencodeValue(*other.root);
    return *this;
}

/*!
 Creates a QBencodeDocument that uses the first \a size bytes from \a data.
 It assumes \a data contains a Bencoded document. The created document does
 not take ownership of \a data and the caller has to guarantee that \a data
 will not be deleted or modified as long as any QBencodeDocument still references
 the data.

 Returns a QBencodeDocument representing the data.

 \sa rawData(), fromBencodeBinary()
 */
/*static*/ QBencodeDocument QBencodeDocument::fromRawData(const char *data, int size)
{
    // TODO: stub
}


/*!
  Returns the Bencoded data in binary.
  \a size will contain the size of the returned data.

  This method is useful to e.g. protect non-string string-typed
  entry.
 */
const char* QBencodeDocument::rawData(int *size) const
{
    // TODO: stub
    return nullptr;
}

/*!
  Creates a QBencodeDocument from the QVariant \a variant.

  If the \a variant contains any other type than a QBencodedValue
  accepted type, the returned document is invalid.

  \sa toVariant()
 */
/*static*/ QBencodeDocument QBencodeDocument::fromVariant(const QVariant &variant)
{
    QBencodeDocument doc;
    QBencodeValue val = QBencodeValue::fromVariant(variant);
    if (!val.isUndefined()) {
        doc.setValue(val);
    }
    return doc;
}

/*!
  Returns a QVariant representing the Bencode document.

  The returned variant will be empty if the document isEmpty()
  or isNull(). Otherwise this method is equivolant to call
  toVariant() on value().

  \sa fromVariant(), QBencodeValue::toVariant()
 */
QVariant QBencodeDocument::toVariant() const
{
    if (isEmpty()) {
        return QVariant();
    } else {
        return root->toVariant();
    }
}

/*!
  Parse Bencode format \a data and creates a QBencodeDocument from it.

  If the \a data is not valid, the method returns a null document.

  The optional \a error variable can be used to pass in a QBencodeParseError
  data structure that will contain information about possible errors encountered
  during parsing.

  \note This method is prefered over fromBencodeString() to avoid potential data
  damage. See toBencodeBinary() for detailed explanation.

  \sa toBencodeBinary(), QBencodeParseError, isNull()
 */
/*static*/ QBencodeDocument QBencodeDocument::fromBencodeBinary(const QByteArray &data, QBencodeParseError *error)
{
    // TODO: stub
}

/*!
  Returns Bencoded data in binary representation.

  \note There is no standard encoding for string-type entries in Bencode.
  While in most implementation is default to UTF-8, other encodings are still
  valid. e.g. GBK encoding found in Chinese systems. There are implementations
  allow arbitary binary in string-type entries. The caller should use a consistent
  encoding among all systems reading/writing the data and encode/decode the string
  explictly. The overloaded methods accepting and returning string representation
  should not be simply used without be certain about the encoding used around to
  avoid potential data damage.

  \sa fromBencodeByteArray(), toBencodeString(), fromBencodeString()
 */
QByteArray QBencodeDocument::toBencodeByteArray() const
{
    // TODO: stub
}

/*!
  Parse a UTF-8 encoded string as Bencode format and creates a
  QBencodeDocument from it.

  If the \a str is not valid, the method returns a null document.

  The optional \a error variable can be used to pass in a QBencodeParseError
  data structure that will contain information about possible errors encountered
  during parsing.

  \warning fromBencodeBinary() is prefered over this method to avoid potential
  data damage. See toBencodeBinary() for detailed explanation.

  \sa toBencodeString(), QBencodeParseError, isNull(), toBencodeBinary()
 */
/*static*/ QBencodeDocument QBencodeDocument::fromBencodeString(const QString &str, QBencodeParseError *error)
{
    // TODO: stub
}

/*!
  Converts the QBencodeDocument to Bencode format and
  represented by a UTF-8 encoded string.

  \warning toBencodeBinary() is prefered over this method to avoid potential
  data damage. See toBencodeBinary() for detailed explanation.

  \sa fromBencodeString(), toBencodeBinary()
 */
QString QBencodeDocument::toBencodeString() const
{
    // TODO: stub
    return QByteArray();
}

/*!
 * Returns \c true if the document doesn't contain any data.
 */
bool QBencodeDocument::isEmpty() const
{
    return isNull() || root->isUndefined();
}

/*!
  Returns \c true if this document is null.

  Null documents are documents created through the default constructor.

  Documents created from UTF-8 encoded text or the binary format are
  validated during parsing. If validation fails, the returned document
  will also be null.
 */
bool QBencodeDocument::isNull() const
{
    return root == nullptr;
}

/*!
  Returns the QBencodeValue contained in the document.

  Returns a value of type \c Undefined if the document is empty.

  \sa isEmpty(), setValue(), QBencodeValue::Type
 */
QBencodeValue QBencodeDocument::value() const
{
    if (isEmpty()) {
        return QBencodeValue();
    }
    return *root;
}

/*!
  Sets \a value as the root object of this document.

  \sa value()
 */
void QBencodeDocument::setValue(const QBencodeValue &value)
{
    if (root != nullptr) {
        delete root;
    }
    root = new QBencodeValue(value);
}

/*!
  Returns \c true if the \a other document is equal to this document.
 */
bool QBencodeDocument::operator==(const QBencodeDocument &other) const
{
    if (root == nullptr) {
        return other.root == nullptr;
    } else {
        return other.root == nullptr ? false
                                     : (*root) == (*other.root);
    }
}

/*!
  \fn bool QBencodeDocument::operator!=(const QBencodeDocument &other) const

  Returns \c true if \a other is not equal to this document.
 */

QDebug operator<<(QDebug out, const QBencodeDocument &doc)
{
    if (doc.isEmpty()) {
        out << "(QBencodeDocument, <Empty>)";
    } else {
        out << "(QBencodeDocument, " << *doc.root << ")";
    }
    return out;
}
