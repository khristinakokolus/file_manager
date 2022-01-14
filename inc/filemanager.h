#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QMainWindow>
#include <QShortcut>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class FileManager; }
QT_END_NAMESPACE

class FileManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileManager(QWidget *parent = nullptr);
    QModelIndex getIndex();
    QStringList findFiles(const QStringList &files, const QString &text);
    void openFileByPath();
    void deletion();
    ~FileManager();

private slots:
    void clickedFirst(const QModelIndex &index);
    void clickedSecond(const QModelIndex &index);
    void deleteItem();
    void copyItemFrom(QString copyFromPath=QString());
    void copyItemTo(QString copyToPath=QString());
    void openFile();
    void openHexFile();
    void renameItem();
    void runProgram();
    void searchFiles();
    void createDirectory();
    void createFile();
    void help();
    void archiveFile();
    void windowMessage(QFile& file,const char *name, const char *text);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::FileManager *ui;
    QFileSystemModel *fileManager;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* insertAction;
    QAction* openAction;
    QAction* openHexAction;
    QAction* renameAction;
    QAction* runAction;
    QAction* archiveFileAction;
    QModelIndex fileFolderIndex;
    QMenu* contextMenu;
    bool clickedTreeViewFirst;
    bool clickedTreeViewSecond;
    QString fileNameToCopy;
    QString extension;
    QString baseNameFile;
    QString copyFrom;
    QStringList files;
    QString dirName;
    QString copyTo;
    QMap<QString, QPair<QString, QString>> map;
};

#endif // FILEMANAGER_H

