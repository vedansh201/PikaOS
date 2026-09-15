#include "Core/PikaApplicationManager.hpp"

#include "Terminal/PikaTerminal.hpp"
#include "FileManager/PikaFileManager.hpp"

PikaApplicationManager::PikaApplicationManager(QObject *parent)
    : QObject(parent)
{
}

void PikaApplicationManager::openTerminal()
{
    if (m_terminal == nullptr)
    {
        m_terminal = new PikaTerminal();
        m_terminal->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(
            m_terminal,
            &QObject::destroyed,
            this,
            [this]()
            {
                m_terminal = nullptr;
            }
        );
    }

    m_terminal->show();
    m_terminal->raise();
    m_terminal->activateWindow();
}

void PikaApplicationManager::openFiles()
{
    if (m_fileManager == nullptr)
    {
        m_fileManager = new PikaFileManager();
        m_fileManager->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(
            m_fileManager,
            &QObject::destroyed,
            this,
            [this]()
            {
                m_fileManager = nullptr;
            }
        );
    }

    m_fileManager->show();
    m_fileManager->raise();
    m_fileManager->activateWindow();
}