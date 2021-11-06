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
    deleteAction->setShortcut(QKeySequence(Qt::Key_D));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    ui->treeView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView_2->addActions({ deleteAction });


//    ui->treeView_2->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->treeView_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
//    contextMenu = new QMenu(ui->treeView_2);
//    ui->treeView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
//    deleteAction = new QAction("Delete",contextMenu);
//    deleteAction->setShortcut(QKeySequence(Qt::Key_D));

//    ui->treeView_2->addAction(deleteAction);


//    connect(deleteAction, SIGNAL(triggered()), this, SLOT(on_treeView_2_activated(QModelIndex)));



//    shDel = new QShortcut(this);
//    shDel->setKey(QKeySequence::Delete);

//    connect(shDel, SIGNAL(activated()), this, SLOT(on_treeView_2_activated(QModelIndex)));

    //connect(ui->treeView_2, SIGNAL(activated()), this, SLOT(on_treeView_2_activated(QModelIndex)));

//    ui->systemFirst->setModel(fileSysModel);
//    ui->systemSecond->setModel(fileSysModel);

//    connect(ui->systemSecond, SIGNAL(doubleClicked(QModelIndex)),
//            this, SLOT(on_systemFirst_doubleClicked(QModelIndex)));
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

FileSystem::~FileSystem()
{
    delete ui;
}

void FileSystem::deleteItem() {
    qInfo() << "To delete!!!";
    QModelIndex index = ui->treeView_2->currentIndex();
    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();
    QString fileName = fileInfo.fileName();
    qInfo() <<  absPath;
    QFile file(absPath);
    bool removed = file.remove();
    qInfo() << removed;
}


void FileSystem::on_treeView_2_doubleClicked(const QModelIndex &index)
{
   // QTreeView* treeViewDelete = (QTreeView*)sender();
    qInfo() << "I am double clicked!!";
    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();
    QString fileName = fileInfo.fileName();
    qInfo() <<  absPath;
    QFile file(absPath);
    //bool removed = file.remove();
    bool copied = file.copy(fileName + "_copy");
    qInfo() << copied;


    //QDir directory = fileInfo.dir();
    //treeViewDelete->setRootIndex(fileSysModel->index(directory.absolutePath()));

}


void FileSystem::on_treeView_2_activated(const QModelIndex &index)
{
    qInfo() << "I here to delete!!";
    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();
    QString fileName = fileInfo.fileName();
    qInfo() <<  absPath;
    QFile file(absPath);
    bool removed = file.remove();
    qInfo() << removed;
}
