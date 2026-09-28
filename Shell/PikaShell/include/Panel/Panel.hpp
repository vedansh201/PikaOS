#pragma once

#include <QWidget>

class QLabel;
class QTimer;
class QPushButton;
class QHBoxLayout;
class PikaApplicationManager;
class PikaLauncher;

class Panel : public QWidget
{
public:
    explicit Panel(PikaApplicationManager *appManager,
                   QWidget *parent = nullptr);

    void updateApplications();
    void updateActiveApplication();

private:
    void setupUi();
    void updateClock();
    void updateButtonStates();
    void showLauncher();

    PikaApplicationManager *m_appManager = nullptr;

    QHBoxLayout *m_layout = nullptr;
    PikaLauncher *m_launcher = nullptr;
    QLabel *m_clock = nullptr;
    QTimer *m_timer = nullptr;

    QPushButton *m_terminalButton = nullptr;
    QPushButton *m_filesButton = nullptr;
};
