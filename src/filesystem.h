#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QMainWindow>
#include <QShortcut>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class FileSystem; }
QT_END_NAMESPACE

class FileSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileSystem(QWidget *parent = nullptr);
    void deletion();
    ~FileSystem();

private slots:
    void clickedFirst(const QModelIndex &index);
    void clickedSecond(const QModelIndex &index);
    void deleteItem();
    void copyItemFrom(QString copyFromPath=QString());
    void copyItemTo(QString copyToPath=QString());
    void openFile();
    void renameItem();
    void windowMessage(QFile& file,const char *name, const char *text);

private:
    Ui::FileSystem *ui;
    QFileSystemModel *fileSysModel;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* insertAction;
    QAction* openAction;
    QAction* renameAction;
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

//extern QString copyFrom;

#endif // FILESYSTEM_H
