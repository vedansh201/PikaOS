#pragma once

#include <QWidget>
#include <QStringList>

class QTermWidget;
class QLineEdit;

class PikaTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit PikaTerminal(QWidget *parent = nullptr);
    ~PikaTerminal();

private:
    QTermWidget *m_terminal;
};
