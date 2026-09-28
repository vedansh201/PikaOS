#pragma once

#include <QObject>

class PikaTerminal;
class PikaFileManager;
class QWidget;

class PikaApplicationManager : public QObject
{
    Q_OBJECT

public:
    explicit PikaApplicationManager(QObject *parent = nullptr);

    void openTerminal();
    void openFiles();

    void toggleTerminal();
    void toggleFiles();

    bool isTerminalOpen() const;
    bool isFilesOpen() const;

    bool isTerminalVisible() const;
    bool isFilesVisible() const;

    bool isTerminalActive() const;
    bool isFilesActive() const;

signals:
    void applicationsChanged();
    void activeApplicationChanged();

private:
    void updateActiveApplication(QWidget *oldWidget, QWidget *newWidget);

    PikaTerminal *m_terminal = nullptr;
    PikaFileManager *m_fileManager = nullptr;

    QWidget *m_activeApplication = nullptr;
};