#include "Terminal/PikaTerminal.hpp"

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>

PikaTerminal::PikaTerminal(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Pika Terminal");
    resize(800, 500);

    m_output = new QPlainTextEdit(this);
    m_output->setReadOnly(true);

    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Enter a command...");

    m_runButton = new QPushButton("Run", this);

    auto *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(m_input);
    inputLayout->addWidget(m_runButton);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_output);
    layout->addLayout(inputLayout);

    m_shell = new QProcess(this);

    connect(
        m_runButton,
        &QPushButton::clicked,
        this,
        &PikaTerminal::executeCommand
    );

    connect(
        m_input,
        &QLineEdit::returnPressed,
        this,
        &PikaTerminal::executeCommand
    );

    connect(
        m_shell,
        &QProcess::readyReadStandardOutput,
        this,
        [this]()
        {
            QByteArray output = m_shell->readAllStandardOutput();

            m_output->moveCursor(QTextCursor::End);
            m_output->insertPlainText(
                QString::fromLocal8Bit(output)
            );
            m_output->moveCursor(QTextCursor::End);
        }
    );

    connect(
        m_shell,
        &QProcess::readyReadStandardError,
        this,
        [this]()
        {
            QByteArray error = m_shell->readAllStandardError();

            m_output->moveCursor(QTextCursor::End);
            m_output->insertPlainText(
                QString::fromLocal8Bit(error)
            );
            m_output->moveCursor(QTextCursor::End);
        }
    );

    m_output->appendPlainText("Pika Terminal");
    m_output->appendPlainText("Starting shell...");
    m_output->appendPlainText("");

    m_shell->start("/bin/bash");

    if (!m_shell->waitForStarted(1000))
    {
        m_output->appendPlainText(
            "Failed to start Bash."
        );
    }
}

PikaTerminal::~PikaTerminal()
{
    if (m_shell &&
        m_shell->state() != QProcess::NotRunning)
    {
        m_shell->terminate();
        m_shell->waitForFinished(1000);
    }
}

void PikaTerminal::executeCommand()
{
    QString command = m_input->text();

    if (command.trimmed().isEmpty())
        return;

    m_output->moveCursor(QTextCursor::End);

    m_output->insertPlainText(
        "$ " + command + "\n"
    );

    m_shell->write(
        command.toLocal8Bit()
    );

    m_shell->write("\n");

    m_input->clear();
}