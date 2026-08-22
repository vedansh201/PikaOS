#include "Terminal/PikaTerminal.hpp"

#include <qtermwidget.h>
#include <QVBoxLayout>
#include <QFont>

PikaTerminal::PikaTerminal(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Pika Terminal");
    resize(900, 600);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_terminal = new QTermWidget(this);

    // Linux-style terminal appearance
    m_terminal->setColorScheme("Linux");

    m_terminal->setTerminalFont(
        QFont("JetBrains Mono", 11)
    );

    m_terminal->setScrollBarPosition(
        QTermWidget::ScrollBarRight
    );

    layout->addWidget(m_terminal);

    // Start Bash
    m_terminal->startShellProgram();

    m_terminal->setFocus();
}

PikaTerminal::~PikaTerminal()
{
}