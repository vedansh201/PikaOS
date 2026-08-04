#pragma once

#include <QWidget>


class Panel;

class PikaWindow : public QWidget
{
public:
    explicit PikaWindow(QWidget *parent = nullptr);

private:
    void setupUi();

    Panel *m_panel;
};