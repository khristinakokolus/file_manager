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
    void assignIndex(const QModelIndex &index);
    void deleteItem();
    void copyItemFrom();
    void copyItemTo();
    void openFile();
    void renameItem();


private:
    Ui::FileSystem *ui;
    QFileSystemModel *fileSysModel;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* insertAction;
    QAction* openAction;
    QAction* renameAction;
    QModelIndex fileFolderIndex;
    QString fileNameToCopy;
    QString extension;
    QString baseNameFile;
    QString copyFrom;
    QString copyTo;
    QMenu* contextMenu;

};

//extern QString copyFrom;

#endif // FILESYSTEM_H
