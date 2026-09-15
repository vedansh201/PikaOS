
#pragma once

#include <QString>

class PikaApplicationManager;

struct PikaCommandResult
{
    bool success;
    QString message;
};

class PikaCommandParser
{
public:
    explicit PikaCommandParser(PikaApplicationManager *appManager = nullptr);

    PikaCommandResult execute(const QString &command);

private:
    PikaApplicationManager *m_appManager = nullptr;
};