#pragma once

#include <QWidget>

class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QProcess;

class PikaTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit PikaTerminal(QWidget *parent = nullptr);
    ~PikaTerminal();

private:
    QPlainTextEdit *m_output;
    QLineEdit *m_input;
    QPushButton *m_runButton;
    QProcess *m_shell;

    void executeCommand();
};