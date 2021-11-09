#include "filesystem.h"
#include "ui_filesystem.h"

#include <QTreeView>
#include <QMenu>
#include <QShortcut>
#include <QDir>
#include <QDebug>

FileSystem::FileSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileSystem)
{
    ui->setupUi(this);

    QString sysPath = "";
    fileSysModel = new QFileSystemModel(this);
    //fileSysModel->setFilter(QDir::QDir::AllEntries);
    fileSysModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files );
    fileSysModel->setRootPath(sysPath);

    qInfo() << "Start!!";

    ui->treeView->setModel(fileSysModel);
    ui->treeView_2->setModel(fileSysModel);

    auto deleteAction = new QAction("Delete", this);
    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    auto copyAction = new QAction("Copy", this);
    copyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyItemFrom()));

    auto insertAction = new QAction("Insert", this);
    insertAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));
    connect(insertAction, SIGNAL(triggered()), this, SLOT(copyItemTo()));

    ui->treeView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView_2->addActions({ copyAction, insertAction, deleteAction });
}


//void FileSystem::on_systemFirst_doubleClicked(const QModelIndex &index)
//{
//    QListView* listView = (QListView*)sender();
//    QFileInfo fileInfo = fileSysModel->fileInfo(index);
//    if (fileInfo.fileName() == "..") {
//       QDir directory = fileInfo.dir();
//       directory.cdUp();
//       listView->setRootIndex(fileSysModel->index(directory.absolutePath()));
//    }
//    else if (fileInfo.fileName() == ".") {
//        listView->setRootIndex(fileSysModel->index(""));
//    }
//    else if (fileInfo.isDir()) {
//        listView->setRootIndex(index);
//    }

//}


void FileSystem::deleteItem() {
    qInfo() << "To delete!!!";
    QModelIndex index = ui->treeView_2->currentIndex();
    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();

    if (fileInfo.isDir()) {
        QDir dir(absPath);
        qInfo() << absPath;
        bool removedDir = dir.removeRecursively();
        qInfo() << removedDir;
    }
    else if (fileInfo.isFile()){
        QFile file(absPath);
        bool removedFile = file.remove();
        qInfo() << removedFile;
    }

}


void FileSystem::copyItemFrom() {
    qInfo() << "To copy!!!";
    QModelIndex index = ui->treeView_2->currentIndex();
    QFileInfo fileInfo = fileSysModel->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    fileNameToCopy = fileInfo.absoluteFilePath();
    extension = fileInfo.completeSuffix();
    baseNameFile = fileInfo.baseName();
    copyFrom = absPath;
}

void FileSystem::copyItemTo() {
    qInfo() << "To insert!!!";
    QModelIndex index = ui->treeView_2->currentIndex();
    QFileInfo fileInfo = fileSysModel->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    QString copyTo = absPath;
    qInfo() << fileNameToCopy;
    QFile file(fileNameToCopy);


    bool copied = file.copy(copyTo + "/" + baseNameFile + "_copy." + extension);
    qInfo() << copied;
}



FileSystem::~FileSystem()
{
    delete ui;
}
