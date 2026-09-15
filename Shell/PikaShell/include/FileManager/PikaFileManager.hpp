#pragma once

#include <QWidget>
#include <QString>
#include <QStringList>

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QPushButton;
class QMenu;

class PikaFileManager : public QWidget
{
    Q_OBJECT


public:
    explicit PikaFileManager(QWidget *parent = nullptr);

private:
    QListWidget *m_fileList;
    QListWidget *m_sidebar;
    QLineEdit *m_pathBar;

    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
    QPushButton *m_upButton;

    QString m_currentPath;
    QPushButton *m_newButton;
    QStringList m_backHistory;
    QStringList m_forwardHistory;

    void loadDirectory(const QString &path);
    void openItem();
    void showNewMenu();
    void createFolder();
    void createTextFile();
    void goBack();
    void goForward();
    void goUp();
    void navigateTo(const QString &path);
    void renameItem();
    void deleteItem();
    void showProperties();
    void showContextMenu(const QPoint &pos);
    void openSidebarLocation(QListWidgetItem *item);
    void copyItem();
    void cutItem();
    void pasteItem();

    QStringList m_clipboardPaths;
    bool m_cutOperation;
};