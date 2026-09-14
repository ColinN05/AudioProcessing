#pragma once

#include <QPlainTextEdit>
#include <streambuf>
#include <iostream>

class TextEditStreamBuf : public std::streambuf
{
public:
    TextEditStreamBuf(QPlainTextEdit* textEdit);
    std::string readBuffer();
protected:
    std::streambuf::int_type overflow(std::streambuf::int_type ch) override;
private:
    QPlainTextEdit* m_TextEdit;
    std::string m_Buffer;
};
