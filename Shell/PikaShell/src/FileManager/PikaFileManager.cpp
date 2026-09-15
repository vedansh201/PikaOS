#include "FileManager/PikaFileManager.hpp"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QUrl>
#include <QMenu>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>

PikaFileManager::PikaFileManager(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Pika Files");
    resize(1000, 650);

    // -----------------------------
    // Navigation bar
    // -----------------------------

    m_backButton = new QPushButton("←", this);
    m_forwardButton = new QPushButton("→", this);
    m_upButton = new QPushButton("↑", this);
    m_newButton = new QPushButton("+ New", this);
    

    m_backButton->setFixedWidth(40);
    m_forwardButton->setFixedWidth(40);
    m_upButton->setFixedWidth(40);

    m_pathBar = new QLineEdit(this);
    m_pathBar->setReadOnly(true);

    auto *navigationLayout = new QHBoxLayout;

    navigationLayout->addWidget(m_backButton);
    navigationLayout->addWidget(m_forwardButton);
    navigationLayout->addWidget(m_upButton);
    navigationLayout->addWidget(m_pathBar);
    navigationLayout->addWidget(m_newButton);
    // -----------------------------
    // File list
    // -----------------------------

    m_fileList = new QListWidget(this);

    m_fileList->setSelectionMode(
        QAbstractItemView::ExtendedSelection
    );

    m_fileList->setContextMenuPolicy(
        Qt::CustomContextMenu
    );
   // Sidebar
    m_sidebar = new QListWidget(this);

    m_sidebar->setFixedWidth(190);
    m_sidebar->setIconSize(QSize(24, 24));
    m_sidebar->setSpacing(3);
    connect(
        m_backButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goBack
    );

    connect(
        m_forwardButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goForward
    );

    connect(
        m_upButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::goUp
    );

    connect(
        m_newButton,
        &QPushButton::clicked,
        this,
        &PikaFileManager::showNewMenu
    );

    connect(
        m_sidebar,
        &QListWidget::itemClicked,
        this,
        &PikaFileManager::openSidebarLocation
    );  
    connect(
        m_fileList,
        &QListWidget::customContextMenuRequested,
        this,
        [this](const QPoint &position)
        {
            QListWidgetItem *item =
                m_fileList->itemAt(position);

            QMenu menu(this);

        // --------------------------------
        // Right-clicked on a file/folder
        // --------------------------------

            if (item)
            {
                QAction *openAction =
                    menu.addAction("📂 Open");

                QAction *renameAction =
                    menu.addAction("✏️ Rename");

                menu.addSeparator();

                QAction *copyAction =
                    menu.addAction("📋 Copy");

                QAction *cutAction =
                    menu.addAction("✂️ Cut");

                menu.addSeparator();

                QAction *deleteAction =
                    menu.addAction("🗑️ Delete");

                menu.addSeparator();

                QAction *propertiesAction =
                    menu.addAction("ℹ️ Properties");

                QAction *selectedAction =
                    menu.exec(
                        m_fileList->viewport()
                            ->mapToGlobal(position)
                    );

                if (selectedAction == openAction)
                {
                    m_fileList->setCurrentItem(item);
                    openItem();
                }
                else if (selectedAction == renameAction)
                {
                    m_fileList->setCurrentItem(item);
                    renameItem();
                }
                else if (selectedAction == copyAction)
                {
                    m_fileList->setCurrentItem(item);
                    copyItem();
                }
                else if (selectedAction == cutAction)
                {
                    m_fileList->setCurrentItem(item);
                    cutItem();
                }
                else if (selectedAction == deleteAction)
                {
                    m_fileList->setCurrentItem(item);

                // Your existing delete function/action
                // should go here if you already have one.
                }
                else if (selectedAction == propertiesAction)
                {
                    m_fileList->setCurrentItem(item);

                // Your existing properties function/action
                // should go here if you already have one.
                }   
            }

        // --------------------------------
        // Right-clicked on empty space
        // --------------------------------

            else
            {
                QAction *pasteAction =
                    menu.addAction("📥 Paste");

                menu.addSeparator();

                QAction *newAction =
                    menu.addAction("＋ New");

                QAction *selectedAction =
                    menu.exec(
                        m_fileList->viewport()
                            ->mapToGlobal(position)
                    );

                if (selectedAction == pasteAction)
                {
                    pasteItem();
                }
                else if (selectedAction == newAction)
                {
                    showNewMenu();
                }
            
            }
        }
    );
    auto addSidebarItem =
        [this](const QString &name,
               const QString &path)
   {
         auto *item =
             new QListWidgetItem(name);

        item->setData(
            Qt::UserRole,
            path
        );

        m_sidebar->addItem(item);
   };

    addSidebarItem(
        "🏠  Home",
        QDir::homePath()
   );

    addSidebarItem(
        "🖥  Desktop",
        QDir::homePath() + "/Desktop"
   );

    addSidebarItem(
        "📄  Documents",
        QDir::homePath() + "/Documents"
   );

    addSidebarItem(
        "⬇  Downloads",
        QDir::homePath() + "/Downloads"
   );

    addSidebarItem(
        "🖼  Pictures",
        QDir::homePath() + "/Pictures"
   );

    addSidebarItem(
        "🎵  Music",
        QDir::homePath() + "/Music"
   );

    addSidebarItem(
        "🎬  Videos",
        QDir::homePath() + "/Videos"
   );

    m_fileList->setViewMode(QListView::ListMode);
    m_fileList->setIconSize(QSize(32, 32));
    m_fileList->setSpacing(4);

    // -----------------------------
    // Main layout
    // -----------------------------

    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(10, 10, 10, 10);
    layout->addLayout(navigationLayout);

    auto *contentLayout = new QHBoxLayout;

    contentLayout->addWidget(m_sidebar);
    contentLayout->addWidget(m_fileList);

    layout->addLayout(contentLayout);
    // -----------------------------
    // Connections
    // -----------------------------

    connect(
       m_fileList,
       &QListWidget::customContextMenuRequested,
       this,
       [this](const QPoint &position)
       {
            QListWidgetItem *item =
                m_fileList->itemAt(position);

            QMenu menu(this);

        // ==========================================
        // Right-click on a file/folder
        // ==========================================

            if (item)
            {
                QAction *openAction =
                    menu.addAction("📂 Open");

                QAction *renameAction =
                    menu.addAction("✏️ Rename");

                menu.addSeparator();

                QAction *copyAction =
                    menu.addAction("📋 Copy");

                QAction *cutAction =
                    menu.addAction("✂️ Cut");

                menu.addSeparator();

                QAction *deleteAction =
                    menu.addAction("🗑️ Delete");

                menu.addSeparator();

                QAction *propertiesAction =
                    menu.addAction("ℹ️ Properties");

                QAction *selectedAction =
                    menu.exec(
                        m_fileList->viewport()->mapToGlobal(
                            position
                        )
                    );

                if (selectedAction == openAction)
                {
                    m_fileList->setCurrentItem(item);
                    openItem();
                }
                else if (selectedAction == renameAction)
                {
                    m_fileList->setCurrentItem(item);
                    renameItem();
                }
                else if (selectedAction == deleteAction)
                {
                    m_fileList->setCurrentItem(item);
                    deleteItem();
                }
                else if (selectedAction == propertiesAction)
                {
                    m_fileList->setCurrentItem(item);
                 showProperties();
                }
            }

        // ==========================================
        // Right-click on empty space
        // ==========================================

            else
            {
                QAction *newFolderAction =
                    menu.addAction("📁 New Folder");

                QAction *newTextFileAction =
                    menu.addAction("📄 New Text File");

                menu.addSeparator();

                QAction *refreshAction =
                    menu.addAction("🔄 Refresh");

                QAction *selectedAction =
                    menu.exec(
                        m_fileList->viewport()->mapToGlobal(
                            position
                        )
                    );

                if (selectedAction == newFolderAction)
                {
                    createFolder();
                }
                else if (selectedAction == newTextFileAction)
                {
                    createTextFile();
                }
                else if (selectedAction == refreshAction)
                {
                  loadDirectory(m_currentPath);
                }
            }
        }
    );

    // -----------------------------
    // Start at Home
    // -----------------------------

    loadDirectory(QDir::homePath());
}


// --------------------------------------------------
// Navigate to directory
// --------------------------------------------------

void PikaFileManager::navigateTo(const QString &path)
{
    if (path == m_currentPath)
        return;

    if (!m_currentPath.isEmpty())
    {
        m_backHistory.append(m_currentPath);
    }

    m_forwardHistory.clear();

    loadDirectory(path);
}


// --------------------------------------------------
// Load directory
// --------------------------------------------------

void PikaFileManager::loadDirectory(const QString &path)
{
    QDir directory(path);

    if (!directory.exists())
        return;

    m_currentPath = directory.absolutePath();

    m_pathBar->setText(m_currentPath);

    m_fileList->clear();

    QFileIconProvider iconProvider;

    QFileInfoList entries =
        directory.entryInfoList(
            QDir::AllEntries |
            QDir::NoDotAndDotDot,
            QDir::DirsFirst |
            QDir::Name
        );

    for (const QFileInfo &entry : entries)
    {
        auto *item = new QListWidgetItem;

        item->setText(entry.fileName());

        item->setIcon(
            iconProvider.icon(entry)
        );

        item->setData(
            Qt::UserRole,
            entry.absoluteFilePath()
        );

        m_fileList->addItem(item);
    }

    m_backButton->setEnabled(
        !m_backHistory.isEmpty()
    );

    m_forwardButton->setEnabled(
        !m_forwardHistory.isEmpty()
    );
}


// --------------------------------------------------
// Open file/folder
// --------------------------------------------------

void PikaFileManager::openItem()
{
    auto *item = m_fileList->currentItem();

    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QFileInfo info(path);

    // Open folders inside Pika Files
    if (info.isDir())
    {
        navigateTo(path);
        return;
    }

    // Open files using the system's default application
    if (info.isFile())
    {
        QDesktopServices::openUrl(
            QUrl::fromLocalFile(path)
        );
    }
}
// --------------------------------------------------
// Go up
// --------------------------------------------------

void PikaFileManager::goUp()
{
    QDir directory(m_currentPath);

    if (!directory.cdUp())
        return;

    navigateTo(
        directory.absolutePath()
    );
}


// --------------------------------------------------
// Back
// --------------------------------------------------

void PikaFileManager::goBack()
{
    if (m_backHistory.isEmpty())
        return;

    m_forwardHistory.append(m_currentPath);

    QString previousPath =
        m_backHistory.takeLast();

    loadDirectory(previousPath);
}


// --------------------------------------------------
// Forward
// --------------------------------------------------

void PikaFileManager::goForward()
{
    if (m_forwardHistory.isEmpty())
        return;

    m_backHistory.append(m_currentPath);

    QString nextPath =
        m_forwardHistory.takeLast();

    loadDirectory(nextPath);
}

void PikaFileManager::openSidebarLocation(
    QListWidgetItem *item
)
{
    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QDir directory(path);

    if (!directory.exists())
        return;

    navigateTo(
        directory.absolutePath()
    );
}
void PikaFileManager::showNewMenu()
{
    QMenu menu(this);

    QAction *newFolder =
        menu.addAction("📁 New Folder");

    QAction *newFile =
        menu.addAction("📄 New Text File");

    QAction *selected =
        menu.exec(
            m_newButton->mapToGlobal(
                QPoint(
                    0,
                    m_newButton->height()
                )
            )
        );

    if (selected == newFolder)
    {
        createFolder();
    }
    else if (selected == newFile)
    {
        createTextFile();
    }
}

void PikaFileManager::createFolder()
{
    bool ok = false;

    QString name =
        QInputDialog::getText(
            this,
            "New Folder",
            "Folder name:",
            QLineEdit::Normal,
            "New Folder",
            &ok
        );

    if (!ok || name.trimmed().isEmpty())
        return;

    name = name.trimmed();

    QDir directory(m_currentPath);

    if (!directory.mkdir(name))
    {
        QMessageBox::warning(
            this,
            "Could not create folder",
            "The folder could not be created.\n"
            "It may already exist or you may not have permission."
        );

        return;
    }

    loadDirectory(m_currentPath);
}

void PikaFileManager::createTextFile()
{
    bool ok = false;

    QString name =
        QInputDialog::getText(
            this,
            "New Text File",
            "File name:",
            QLineEdit::Normal,
            "New Text File.txt",
            &ok
        );

    if (!ok || name.trimmed().isEmpty())
        return;

    name = name.trimmed();

    QString filePath =
        QDir(m_currentPath).filePath(name);

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
            this,
            "Could not create file",
            "The file could not be created.\n"
            "You may not have permission to write here."
        );

        return;
    }

    file.close();

    loadDirectory(m_currentPath);
}

void PikaFileManager::renameItem()
{
    QListWidgetItem *item =
        m_fileList->currentItem();

    if (!item)
        return;

    QString oldPath =
        item->data(Qt::UserRole).toString();

    QFileInfo info(oldPath);

    if (!info.exists())
        return;

    bool ok = false;

    QString newName =
        QInputDialog::getText(
            this,
            "Rename",
            "New name:",
            QLineEdit::Normal,
            info.fileName(),
            &ok
        );

    if (!ok || newName.trimmed().isEmpty())
        return;

    newName = newName.trimmed();

    QString newPath =
        QDir(m_currentPath).filePath(newName);

    if (newPath == oldPath)
        return;

    if (QFileInfo::exists(newPath))
    {
        QMessageBox::warning(
            this,
            "Rename failed",
            "An item with that name already exists."
        );

        return;
    }

    QDir directory;

    if (!directory.rename(oldPath, newPath))
    {
        QMessageBox::warning(
            this,
            "Rename failed",
            "The item could not be renamed."
        );

        return;
    }

    loadDirectory(m_currentPath);
}
void PikaFileManager::deleteItem()
{
    QListWidgetItem *item =
        m_fileList->currentItem();

    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QFileInfo info(path);

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Delete",
            "Are you sure you want to delete:\n\n" +
                info.fileName() + "?",
            QMessageBox::Yes |
            QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    bool success = QFile::moveToTrash(path);

    if (!success)
        {
        QMessageBox::warning(
            this,
            "Delete Failed",
            "Could not move this item to the Trash:\n\n" + path
        );

        return;
    }

    loadDirectory(m_currentPath);
}
void PikaFileManager::showProperties()
{
    QListWidgetItem *item =
        m_fileList->currentItem();

    if (!item)
        return;

    QString path =
        item->data(Qt::UserRole).toString();

    QFileInfo info(path);

    QString type;

    if (info.isDir())
    {
        type = "Folder";
    }
    else
    {
        QString suffix =
            info.suffix().toLower();

        if (suffix == "txt")
            type = "Text File";
        else if (suffix == "png" ||
                 suffix == "jpg" ||
                 suffix == "jpeg")
            type = "Image";
        else if (suffix == "mp3" ||
                 suffix == "wav" ||
                 suffix == "ogg")
            type = "Audio File";
        else if (suffix == "pdf")
            type = "PDF Document";
        else
            type = "File";
    }

    QString size;

    if (info.isDir())
    {
        QDir directory(path);

        int itemCount =
            directory.entryList(
                QDir::AllEntries |
                QDir::NoDotAndDotDot
            ).count();

        size =
            QString::number(itemCount) +
            (itemCount == 1 ? " item" : " items");
    }
    else
    {
        qint64 bytes = info.size();

        if (bytes < 1024)
        {
            size =
                QString::number(bytes) +
                " bytes";
        }
        else if (bytes < 1024 * 1024)
        {
            size =
                QString::number(
                    bytes / 1024.0,
                    'f',
                    1
                ) +
                " KB";
        }
        else
        {
            size =
                QString::number(
                    bytes / (1024.0 * 1024.0),
                    'f',
                    1
                ) +
                " MB";
        }
    }

    QString details =
        "Name:       " + info.fileName() + "\n\n"
        "Type:       " + type + "\n"
        "Location:   " + info.absolutePath() + "\n"
        "Size:       " + size + "\n"
        "Modified:   " +
            info.lastModified().toString(
                "dd MMM yyyy, hh:mm AP"
            );

    QMessageBox::information(
        this,
        "Properties",
        details
    );
}

void PikaFileManager::copyItem()
{
    m_clipboardPaths.clear();

    QList<QListWidgetItem *> selectedItems =
        m_fileList->selectedItems();

    if (selectedItems.isEmpty())
        return;

    for (QListWidgetItem *item : selectedItems)
    {
        m_clipboardPaths.append(
            item->data(Qt::UserRole).toString()
        );
    }

    m_cutOperation = false;

    qDebug()
        << "Copied:"
        << m_clipboardPaths;
}


void PikaFileManager::cutItem()
{
    m_clipboardPaths.clear();

    QList<QListWidgetItem *> selectedItems =
        m_fileList->selectedItems();

    if (selectedItems.isEmpty())
        return;

    for (QListWidgetItem *item : selectedItems)
    {
        m_clipboardPaths.append(
            item->data(Qt::UserRole).toString()
        );
    }

    m_cutOperation = true;

    qDebug()
        << "Cut:"
        << m_clipboardPaths;
}

void PikaFileManager::pasteItem()
{
    if (m_clipboardPaths.isEmpty())
        return;

    bool allSuccessful = true;

    for (const QString &sourcePath : m_clipboardPaths)
    {
        QFileInfo sourceInfo(sourcePath);

        if (!sourceInfo.exists())
        {
            allSuccessful = false;
            continue;
        }

        QString destination =
            m_currentPath + "/" + sourceInfo.fileName();

        if (QFileInfo::exists(destination))
        {
            QMessageBox::warning(
                this,
                "Pika Files",
                "An item named \"" +
                    sourceInfo.fileName() +
                    "\" already exists."
            );

            allSuccessful = false;
            continue;
        }

        bool success = false;

        if (sourceInfo.isDir())
        {
            QDir sourceDir(sourcePath);

            success =
                QDir().mkdir(destination);

            if (success)
            {
                for (const QFileInfo &entry :
                     sourceDir.entryInfoList(
                         QDir::AllEntries |
                         QDir::NoDotAndDotDot))
                {
                    QString target =
                        destination + "/" +
                        entry.fileName();

                    if (entry.isDir())
                    {
                        if (!QDir().mkpath(target))
                        {
                            success = false;
                            break;
                        }
                    }
                    else
                    {
                        if (!QFile::copy(
                                entry.absoluteFilePath(),
                                target))
                        {
                            success = false;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            success =
                QFile::copy(
                    sourcePath,
                    destination
                );
        }

        if (!success)
        {
            allSuccessful = false;
            continue;
        }

        // Cut = move, so remove the original
        if (m_cutOperation)
        {
            if (sourceInfo.isDir())
            {
                QDir sourceDir(sourcePath);

                if (!sourceDir.removeRecursively())
                    allSuccessful = false;
            }
            else
            {
                if (!QFile::remove(sourcePath))
                    allSuccessful = false;
            }
        }
    }

    if (!allSuccessful)
    {
        QMessageBox::warning(
            this,
            "Pika Files",
            "Some items could not be pasted."
        );
    }

    loadDirectory(m_currentPath);

    if (m_cutOperation)
    {
        m_clipboardPaths.clear();
        m_cutOperation = false;
    }
}