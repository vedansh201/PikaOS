#pragma once

#include <QString>

struct PikaCommandResult
{
    bool success;
    QString message;
};

class PikaCommandParser
{
public:
    PikaCommandResult execute(const QString &command);
};