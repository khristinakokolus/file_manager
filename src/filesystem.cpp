#include "filesystem.h"
#include "ui_filesystem.h"

#include <QTreeView>
#include <QMenu>
#include <QShortcut>
#include <QDir>
#include <QDebug>
#include <QTextEdit>
#include <QMessageBox>

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

    auto openAction = new QAction("Open", this);
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    auto renameAction = new QAction("Rename", this);
    renameAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    connect(renameAction, SIGNAL(triggered()), this, SLOT(renameItem()));

    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addActions({ openAction, copyAction, insertAction, deleteAction, renameAction });

    ui->treeView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView_2->addActions({ openAction, copyAction, insertAction, deleteAction, renameAction });

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(assignIndex(QModelIndex)));
    connect(ui->treeView_2, SIGNAL(clicked(QModelIndex)), this, SLOT(assignIndex(QModelIndex)));
}



void FileSystem::deleteItem() {
    QMessageBox quitMsg;
    quitMsg.setWindowTitle("FileSystem");
    quitMsg.setText("Do you really want to delete?");
    quitMsg.setStandardButtons(QMessageBox::Yes |
    QMessageBox::Cancel);
    quitMsg.setDefaultButton(QMessageBox::Cancel);
    if (quitMsg.exec() == QMessageBox::Yes)
        deletion();

}

void FileSystem::deletion() {
    QFileInfo fileInfo = fileSysModel->fileInfo(fileFolderIndex);
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
    QFileInfo fileInfo = fileSysModel->fileInfo(fileFolderIndex);

    QString absPath = fileInfo.absoluteFilePath();
    fileNameToCopy = fileInfo.absoluteFilePath();
    extension = fileInfo.completeSuffix();
    baseNameFile = fileInfo.baseName();
    copyFrom = absPath;
}


void FileSystem::copyItemTo() {
    QFileInfo fileInfo = fileSysModel->fileInfo(fileFolderIndex);

    QString absPath = fileInfo.absoluteFilePath();
    QString copyTo = absPath;
    QFile file(fileNameToCopy);

    qInfo() << copyTo + "/" + baseNameFile + "_copy." + extension;
    bool copied = file.copy(copyTo + "/" + baseNameFile + "_copy." + extension);
    qInfo() << copied;
}

void FileSystem::openFile() {
    QFileInfo fileInfo = fileSysModel->fileInfo(fileFolderIndex);
    QString absPath = fileInfo.absoluteFilePath();
    qInfo() << fileFolderIndex;
    qInfo() << absPath;
    QFile file(absPath);

    file.open(QFile::ReadOnly | QFile::Text);
    QTextEdit *txt = new QTextEdit();
    QTextStream ReadFile(&file);
    txt->setText(ReadFile.readAll());
    txt->show();
}

void FileSystem::renameItem() {

}

void FileSystem::assignIndex(const QModelIndex &index) {
    fileFolderIndex = index;
}



FileSystem::~FileSystem()
{
    delete ui;
}
