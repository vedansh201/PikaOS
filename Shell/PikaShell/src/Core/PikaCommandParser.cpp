#include "Core/PikaCommandParser.hpp"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QProcess>
#include <QSysInfo>
#include <QStandardPaths>
#include <QFile>
#include <QDateTime>

static QString findFile(const QString &fileName)
{
    QStringList searchPaths;

    searchPaths << QStandardPaths::writableLocation(
        QStandardPaths::HomeLocation
    );

    searchPaths << QStandardPaths::writableLocation(
        QStandardPaths::DesktopLocation
    );

    searchPaths << QStandardPaths::writableLocation(
        QStandardPaths::DownloadLocation
    );

    for (const QString &path : searchPaths)
    {
        QDirIterator iterator(
            path,
            QStringList() << fileName,
            QDir::Files,
            QDirIterator::Subdirectories
        );

        if (iterator.hasNext())
            return iterator.next();
    }

    return {};
}

PikaCommandResult PikaCommandParser::execute(const QString &command)
{
    QString input = command.trimmed().toLower();

    qDebug() << "Pika received:" << command;
    qDebug() << "Pika parsed:" << input;

    if (input.isEmpty())
    {
        return {
            false,
            "I didn't hear anything!"
        };
    }

    // Help
    if (input == "help")
    {
        return {
            true,
            "Try: open terminal, open files, open <filename>, create folder <name>, create file <name>, system info, battery, time, date"
        };
    }

    // Open terminal
    if (input == "open terminal")
    {
        QProcess::startDetached("konsole");

        return {
            true,
            "Opening the terminal..."
        };
    }

    // Open home directory
    if (input == "open files")
    {
        QString home =
            QStandardPaths::writableLocation(
                QStandardPaths::HomeLocation
            );

        QProcess::startDetached(
            "xdg-open",
            {home}
        );

        return {
            true,
            "Opening your files..."
        };
    }

    // Create folder
    if (input.startsWith("create folder "))
    {
        QString folderName =
            command.mid(QString("create folder ").length()).trimmed();

        if (folderName.isEmpty())
        {
            return {
                false,
                "You need to give me a folder name."
            };
        }

        QString home =
            QStandardPaths::writableLocation(
                QStandardPaths::HomeLocation
            );

        QDir dir(home);

        if (dir.mkdir(folderName))
        {
            return {
                true,
                "Created folder: " + folderName
            };
        }

        return {
            false,
            "I couldn't create that folder."
        };
    }
    // Create file
    if (input.startsWith("create file "))
    {
        QString fileName =
            command.mid(QString("create file ").length()).trimmed();

        if (fileName.isEmpty())
        {
            return {
                false,
                "You need to give me a file name."
            };
        }

        QString home =
            QStandardPaths::writableLocation(
                QStandardPaths::HomeLocation
           );

        QString filePath =
            QDir(home).filePath(fileName);

        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly))
        {
            file.close();

            return {
                true,
                "Created file: " + fileName
            };
        }

        return {
            false,
            "I couldn't create that file."
        };
    }

    // Open a specific file
    if (input.startsWith("open "))
    {
        QString fileName =
            command.mid(QString("open ").length()).trimmed();

        if (fileName.isEmpty())
        {
            return {
                false,
                "Tell me what you want me to open."
            };
        }

        QString filePath = findFile(fileName);

        if (filePath.isEmpty())
        {
            return {
                false,
                "I couldn't find " + fileName
            };
        }

        QProcess::startDetached(
            "xdg-open",
            {filePath}
        );

        return {
            true,
            "Opening " + fileName + "..."
        };
    }
    // Open a specific folder
    if (input.startsWith("open folder "))
    {
       QString folderName =
           command.mid(QString("open folder ").length()).trimmed();

        if (folderName.isEmpty())
        {
            return {
                false,
                "Tell me which folder to open."
            };
        }

        QString home =
            QStandardPaths::writableLocation(
                QStandardPaths::HomeLocation
            );

        QString folderPath =
            QDir(home).filePath(folderName);

        QDir folder(folderPath);

        if (!folder.exists())
        {
            return {
                false,
                "I couldn't find the folder " + folderName
            };
        }

        QProcess::startDetached(
           
            "xdg-open",
        {folderPath}
    );

    return {
        true,
        "Opening folder " + folderName + "..."
    };
}    
    // System information
    if (input == "system info")
    {
        QString info =
            "OS: " + QSysInfo::prettyProductName() +
            "\nKernel: " + QSysInfo::kernelType() +
            " " + QSysInfo::kernelVersion() +
            "\nArchitecture: " + QSysInfo::currentCpuArchitecture();

        return {
            true,
            info
        };
    }
    // Battery status
    if (
       input == "battery" ||
       input == "battery info" ||
       input == "what is my battery" ||
       input == "battery status"
    )
    {
        QFile capacityFile("/sys/class/power_supply/BAT0/capacity");
        QFile statusFile("/sys/class/power_supply/BAT0/status");

        if (!capacityFile.open(QIODevice::ReadOnly) ||
            !statusFile.open(QIODevice::ReadOnly))
        {
            return {
                false,
                "I couldn't read the battery status."
            };
        }

        QString capacity =
            QString::fromUtf8(capacityFile.readAll()).trimmed();

        QString status =
            QString::fromUtf8(statusFile.readAll()).trimmed();

        return {
            true,
            "Battery: " + capacity + "%\nStatus: " + status
        };
    }
    if (
        input == "time" ||
        input == "what time is it" ||
        input == "tell me the time"
    )
    {
        QString currentTime =
            QDateTime::currentDateTime().toString("hh:mm:ss");

        return {
            true,
            "The time is " + currentTime
        };
    }
    if (
        input == "date" ||
        input == "what is the date" ||
        input == "what's the date" ||
        input == "what day is it"
    )
    {
        QString currentDate =
            QDateTime::currentDateTime().toString("dddd, MMMM d, yyyy");

        return {
            true,
            "Today is " + currentDate
        };
    }

    // Unknown command
    return {
        false,
        "I don't know that command yet."
    };
}