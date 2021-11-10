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
    fileSysModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files );
    fileSysModel->setRootPath(sysPath);

    qInfo() << "Start!!";
    clickedTreeViewFirst = false;
    clickedTreeViewSecond = false;

    ui->treeView->setModel(fileSysModel);
    ui->treeView_2->setModel(fileSysModel);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedFirst(QModelIndex)));
    connect(ui->treeView_2, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedSecond(QModelIndex)));

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
    QModelIndex index;
    qInfo() << clickedTreeViewFirst;
    qInfo() << clickedTreeViewSecond;

    if (clickedTreeViewFirst == true) {
        index = ui->treeView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->treeView_2->currentIndex();
        clickedTreeViewSecond = false;
    }

    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();


    if (fileInfo.isDir()) {
            QDir dir(absPath);
            qInfo() << absPath;
            bool removedDir = dir.removeRecursively();
            qInfo() << removedDir;
            if (!removedDir){
                qInfo() << "here";
                QMessageBox quitMsg;
                quitMsg.setWindowTitle("FileSystem");
                quitMsg.setText("An error occurred - " + dir.dirName() + " directory cannot be deleted now.");
                quitMsg.setStandardButtons(QMessageBox::Ok);
                quitMsg.setDefaultButton(QMessageBox::Ok);
                if (quitMsg.exec() == QMessageBox::Ok)
                    return;
            }
        }
        else if (fileInfo.isFile()){
            QFile file(absPath);
            bool removedFile = file.remove();
            qInfo() << removedFile;
            if (!removedFile){
                QMessageBox quitMsg;
                quitMsg.setWindowTitle("FileSystem");
                quitMsg.setText("An error occurred - " + file.fileName() + " cannot be deleted now.");
                quitMsg.setStandardButtons(QMessageBox::Ok);
                quitMsg.setDefaultButton(QMessageBox::Ok);
                if (quitMsg.exec() == QMessageBox::Ok)
                    return;
            }
        }
}


void FileSystem::copyItemFrom(QString copyFromPath) {
    QModelIndex index;
    if (clickedTreeViewFirst == true) {
        index = ui->treeView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->treeView_2->currentIndex();
        clickedTreeViewSecond = false;
    }
    QFileInfo fileInfo = fileSysModel->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    fileNameToCopy = QString();
        extension = QString();
        baseNameFile = QString();

        if (copyFromPath.isNull()) {
            copyFrom = absPath;
        }
        else {
            copyFrom = copyFromPath;
        }

        QDir sourceDir(copyFrom);
        if(!sourceDir.exists()){
            fileNameToCopy = fileInfo.absoluteFilePath();
            extension = fileInfo.completeSuffix();
            baseNameFile = fileInfo.baseName();
        }

        files = sourceDir.entryList(QDir::Files);
        dirName = sourceDir.dirName();
}


void FileSystem::copyItemTo(QString copyToPath) {
    QModelIndex index;
    qInfo() << clickedTreeViewFirst;
    qInfo() << clickedTreeViewSecond;

    if (clickedTreeViewFirst == true) {
        index = ui->treeView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->treeView_2->currentIndex();
        clickedTreeViewSecond = false;
    }
    QFileInfo fileInfo = fileSysModel->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    if(!fileNameToCopy.isNull()) {
            copyTo = absPath;
            QFile file(fileNameToCopy);
            //qInfo() << fileNameToCopy;
            bool copied = file.copy(copyTo + "/" + baseNameFile + "_copy." + extension);
            if (!copied) {
                QMessageBox quitMsg;
                quitMsg.setWindowTitle("FileSystem");
                quitMsg.setText("An error occurred - "+ file.fileName() + " cannot be copied.");
                quitMsg.setStandardButtons(QMessageBox::Ok);
                quitMsg.setDefaultButton(QMessageBox::Ok);
                if (quitMsg.exec() == QMessageBox::Ok)
                    return;
            }
            //qInfo() << copied;
            return;
        }

        if (copyToPath.isNull()) {
            copyTo = absPath + "/" + dirName;
            QDir destDir(copyTo);
            destDir.mkdir(copyTo);
        }
        else {
            copyTo = copyToPath;
            QDir destDir(copyTo);
            if(!destDir.exists()) {
                destDir.mkdir(copyTo);
            }
        }

        for(int i = 0; i< files.count(); i++) {
            //qInfo() << files[i];
            QFile file(files[i]);
            QFileInfo fileInfo(files[i]);
            extension = fileInfo.completeSuffix();
            baseNameFile = fileInfo.baseName();
            bool copied = file.copy(copyFrom + "/" + files[i], copyTo + "/" + baseNameFile + "_copy." + extension);
            if (!copied) {
                QMessageBox quitMsg;
                quitMsg.setWindowTitle("FileSystem");
                quitMsg.setText("An error occurred - " + files[i] + " this file cannot be copied.");
                quitMsg.setStandardButtons(QMessageBox::Ok);
                quitMsg.setDefaultButton(QMessageBox::Ok);
                if (quitMsg.exec() == QMessageBox::Ok)
                    return;
            }
            //qInfo() << copied;
        }
        files.clear();
        QDir sourceDir(copyFrom);
        files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        for(int i = 0; i< files.count(); i++) {
            map.insert(dirName+"/"+files[i], qMakePair(copyFrom, copyTo));
        }

        for(int i = 0; i< files.count(); i++) {
            map.remove(dirName+"/"+files[i]);
            FileSystem::copyItemFrom(copyFrom + "/" + files[i]);
            FileSystem::copyItemTo(copyTo + "/" + dirName);
        }
        if(!(map.isEmpty())) {
            QString from = map.first().first;
            QString to = map.first().second;
            FileSystem::copyItemFrom(from + "/" + map.firstKey().split('/')[1]);
            map.remove(map.firstKey());
            FileSystem::copyItemTo(to + "/" + dirName);
        }
    }

void FileSystem::openFile() {
    QModelIndex index;
    if (clickedTreeViewFirst == true) {
        index = ui->treeView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->treeView_2->currentIndex();
        clickedTreeViewSecond = false;
    }

    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();;
    QFile file(absPath);

    bool opened = file.open(QFile::ReadOnly | QFile::Text);
        if ( !opened ) {
            QMessageBox::critical(
                this,
                tr("Open failed"),
                tr("Could not open file for reading: %1\n%2").arg(file.fileName(), file.errorString() )
                );
            return;
        }

        QTextEdit *txt = new QTextEdit();
        QTextStream ReadFile(&file);
        txt->setText(ReadFile.readAll());
        txt->show();
}

void FileSystem::renameItem() {

}

void FileSystem::clickedFirst(const QModelIndex &index) {
    clickedTreeViewFirst = true;
}


void FileSystem::clickedSecond(const QModelIndex &index) {
    clickedTreeViewSecond = true;
}


FileSystem::~FileSystem()
{
    delete ui;
}
