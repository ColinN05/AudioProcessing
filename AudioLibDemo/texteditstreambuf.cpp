#include "texteditstreambuf.h"

TextEditStreamBuf::TextEditStreamBuf(QPlainTextEdit* textEdit)
    : m_TextEdit(textEdit) {}

std::streambuf::int_type TextEditStreamBuf::overflow(std::streambuf::int_type ch)
{
    if (ch != std::streambuf::traits_type::eof()) 
    {
        m_Buffer += static_cast<char>(ch);
        if (ch == '\n') 
        {
            m_TextEdit->appendPlainText(
                QString::fromStdString(m_Buffer)
            );
            m_Buffer.clear();
        }
    }
    return ch;
}

std::string TextEditStreamBuf::readBuffer()
{
    std::string result = m_Buffer;
    m_Buffer.clear();
    return result;
}

