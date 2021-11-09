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
    ~FileSystem();

private slots:
    void deleteItem();
    void copyItemFrom();
    void copyItemTo();

private:
    Ui::FileSystem *ui;
    QFileSystemModel *fileSysModel;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* insertAction;
    QString fileNameToCopy;
    QString extension;
    QString baseNameFile;
    QString copyFrom;
    QString copyTo;
    QMenu* contextMenu;

};

//extern QString copyFrom;

#endif // FILESYSTEM_H
