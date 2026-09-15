#pragma once

#include <QObject>

class PikaTerminal;
class PikaFileManager;

class PikaApplicationManager : public QObject
{
public:
    explicit PikaApplicationManager(QObject *parent = nullptr);

    void openTerminal();
    void openFiles();

private:
    PikaTerminal *m_terminal = nullptr;
    PikaFileManager *m_fileManager = nullptr;
};