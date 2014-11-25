#include <gtest-strip.h>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include "qbencodewriter_p.h"
#include "qbencodevalue.h"
#include "qbencodedocument.h"

TEST(QBencodeWriterTest, BasicInteger) {
    QBencodeValue val(12);
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("i12e"), bencode);
}

TEST(QBencodeWriterTest, BasicString) {
    QBencodeValue val("abcde");
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("5:abcde"), bencode);
}

TEST(QBencodeWriterTest, BasicList) {
    QBencodeList list;
    list << 12 << "abcde";
    QBencodeValue val(list);
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("li12e5:abcdee"), bencode);
}

TEST(QBencodeWriterTest, BasicDict) {
    QBencodeDict dict;
    dict.insert("key1", 12);
    dict.insert("key2", "abcde");
    dict.insert("aa", "ddd");
    QBencodeValue val(dict);
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("d2:aa3:ddd4:key1i12e4:key25:abcdee"), bencode);
}

TEST(QBencodeWriterTest, EmptyList) {
    QBencodeList list;
    QBencodeValue val(list);
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("le"), bencode);
}

TEST(QBencodeWriterTest, EmptyDict) {
    QBencodeDict dict;
    QBencodeValue val(dict);
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray("de"), bencode);
}

TEST(QBencodeWriterTest, NullValue) {
    QBencodeValue val;
    QBencodeDocument doc(val);
    QByteArray bencode = doc.toBencode();

    EXPECT_EQ(QByteArray(""), bencode);
}

TEST(QBencodeWriterTest, ActualFileRoundAbout) {
    QFile file("tests/data/resume.dat");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly)) << "file open failed";

    QByteArray bytes = file.readAll();
    ASSERT_EQ(file.size(), bytes.size()) << "not all bytes read from file";
    file.close();

    QBencodeDocument doc = QBencodeDocument::fromBencode(bytes);

    QBencodeDocument doc2 = QBencodeDocument::fromBencode(doc.toBencode());

    EXPECT_EQ(doc, doc2);
}