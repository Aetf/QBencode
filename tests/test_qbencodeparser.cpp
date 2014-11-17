#include <gtest-strip.h>
#include <QByteArray>
#include "qbencodeparser_p.h"

TEST(QBencodeParserTest, BasicInteger) {
    QByteArray bencode("i12e");
    QBencodePrivate::Parser parser(bencode.constData(), bencode.size());

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isInteger()) << '\'' << bencode.constData() << "' should yield an integer";
    EXPECT_EQ(12, val.toInteger());
}

TEST(QBencodeParserTest, BasicString) {
    QByteArray bencode("4:abcd");
    QBencodePrivate::Parser parser(bencode.constData(), bencode.size());

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isString()) << '\'' << bencode.constData() << "' should yield a string";
    EXPECT_EQ("abcd", val.toString());
}

TEST(QBencodeParserTest, BasicList) {
    QByteArray bencode("li12e4:abcde");
    QBencodePrivate::Parser parser(bencode.constData(), bencode.size());

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isList()) << '\'' << bencode.constData() << "' should yield a list";
    QBencodeList list = val.toList();

    ASSERT_EQ(2, list.size());
    EXPECT_EQ(12, list[0].toInteger());
    EXPECT_EQ("abcd", list[1].toString());
}

TEST(QBencodecodeParserTest, EmptyList) {
    QBencodePrivate::Parser parser("le", 2);

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isList()) << "'le' should yield a list";
    QBencodeList list = val.toList();

    EXPECT_EQ(0, list.size());
}

TEST(QBencodeParserTest, EmptyDict) {
    QBencodePrivate::Parser parser("de", 2);

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isDict()) << "'de' should yield a list";
    QBencodeDict dict = val.toDict();

    EXPECT_EQ(0, dict.size());
}

TEST(QBencodeParserTest, BasicDict) {
    QByteArray bencode("d1:ai12e2:ab4:dddde");
    QBencodePrivate::Parser parser(bencode.constData(), bencode.size());

    QBencodeParseError err;
    QBencodeValue val = parser.parse(&err).value();
    EXPECT_EQ(QBencodeParseError::NoError, err.error);

    ASSERT_TRUE(val.isDict()) << '\'' << bencode.constData() << "' should yield a dict";
    QBencodeDict dict = val.toDict();

    ASSERT_EQ(2, dict.size());
    EXPECT_EQ(12, dict["a"].toInteger());
    EXPECT_EQ("dddd", dict["ab"].toString());
}

