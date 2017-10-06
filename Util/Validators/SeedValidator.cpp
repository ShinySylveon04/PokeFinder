#include "SeedValidator.hpp"

SeedValidator::SeedValidator(QObject *parent) : QValidator(parent)
{

}

void SeedValidator::fixup(QString & input) const
{
    input.remove(QRegExp("[^0-9A-F]"));
}

QValidator::State SeedValidator::validate(QString & input, int & pos) const
{
    bool pass;
    uint32_t seed = input.toUInt(&pass, 16);;
    (void) pos;

    if (input == "")
    {
        return QValidator::Acceptable;
    }
    else if (!pass)
    {
        return QValidator::Invalid;
    }
    else if(seed > 0xffffffff)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}
