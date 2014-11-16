#include <gtest-strip.h>
#include <QByteArray>
#include "qbencodeparser_p.h"

TEST(QBencodeParserTest, List) {
    QByteArray bencode("li12e4:abcde");
    QBencodeParseError err;
    QBencodePrivate::Parser parser(bencode.constData(), bencode.size());
    QBencodeDocument doc = parser.parse(&err);

    QBencodeValue strabcd(QByteArray("abcd"));
    QBencodeValue int12(12);
    QBencodeList list = { int12, strabcd };
    QBencodeValue root(list);

    EXPECT_EQ(root, doc.value());
}
