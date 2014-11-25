#ifndef QBENCODEWRITER_H
#define QBENCODEWRITER_H

#include "qbencodevalue.h"

namespace QBencodePrivate
{

class Writer
{
public:
    static void toBencode(const QBencodeValue &value, QByteArray &bencode);
};


}
#endif // QBENCODEWRITER_H
