#pragma once

#include <QWidget>

class QLabel;
class QTimer;

class Panel : public QWidget
{
    

public:
    explicit Panel(QWidget *parent = nullptr);

private:
    void setupUi();
    void updateClock();

    QLabel *m_clock = nullptr;
    QTimer *m_timer = nullptr;
};