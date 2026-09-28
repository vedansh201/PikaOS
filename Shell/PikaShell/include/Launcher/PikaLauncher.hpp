#pragma once

#include <QWidget>

class PikaApplicationManager;
class QPushButton;

class PikaLauncher : public QWidget
{
public:
    explicit PikaLauncher(
        PikaApplicationManager *appManager,
        QWidget *parent = nullptr
    );

private:
    void setupUi();

    PikaApplicationManager *m_appManager = nullptr;

    QPushButton *m_terminalButton = nullptr;
    QPushButton *m_filesButton = nullptr;
};