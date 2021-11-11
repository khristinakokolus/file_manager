#include "filesystem.h"
#include "ui_filesystem.h"

#include <QTreeView>
#include <QMenu>
#include <QShortcut>
#include <QDir>
#include <QDebug>
#include <QTextEdit>
#include <QMessageBox>
#include <QInputDialog>

FileSystem::FileSystem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileSystem)
{
    ui->setupUi(this);

    QString sysPath = "";
    fileSysModel = new QFileSystemModel(this);
    fileSysModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files );
    fileSysModel->setRootPath(sysPath);

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



void FileSystem::windowMessage(QFile& file, const char *name, const char *text){
    QMessageBox::critical(
        this,
        tr(name),
        tr(text).arg(file.fileName(), file.errorString() )
        );
    return;
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

QModelIndex FileSystem::getIndex() {
    QModelIndex index;

    if (clickedTreeViewFirst == true) {
        index = ui->treeView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->treeView_2->currentIndex();
        clickedTreeViewSecond = false;
    }
    return index;

}


void FileSystem::deletion() {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();


    if (fileInfo.isDir()) {
            QDir dir(absPath);
            qInfo() << absPath;
            bool removedDir = dir.removeRecursively();
            qInfo() << removedDir;
            if (!removedDir){
                QMessageBox::critical(
                    this,
                    tr("Remove failed"),
                    tr("Could not delete directory: %1").arg(dir.dirName())
                    );
                return;
            }
        }
        else if (fileInfo.isFile()){
            QFile file(absPath);
            bool removedFile = file.remove();
            qInfo() << removedFile;
            if (!removedFile){
                windowMessage(file,"Remove failed", "Could not delete file: %1\n%2");
                return;
            }
        }
}


void FileSystem::copyItemFrom(QString copyFromPath) {
    QModelIndex index = getIndex();

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
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileSysModel->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    if(!fileNameToCopy.isNull()) {
            copyTo = absPath;
            QFile file(fileNameToCopy);
            bool copied = false;
            auto new_name = copyTo + "/" + baseNameFile;
            qInfo() << new_name + "." + extension;

            if (!QFileInfo::exists(new_name + "." + extension)){
                copied = file.copy(new_name + "." + extension);
            }
            else{
                copied = file.copy(new_name + "_copy" + "." + extension);
            }
            if (!copied) {
                windowMessage(file, "Copy failed", "Could not copy file: %1\n%2");
                return;
            }
            return;
        }

        if (copyToPath.isNull()) {
            copyTo = absPath + "/" + dirName;
        }
        else {
            copyTo = copyToPath;
        }
            QDir destDir(copyTo);
            if(!destDir.exists()) {
                destDir.mkdir(copyTo);
            }


        for(int i = 0; i< files.count(); i++) {
            QFile file(files[i]);
            QFileInfo fileInfo(files[i]);
            extension = fileInfo.completeSuffix();
            baseNameFile = fileInfo.baseName();
            auto new_name = copyTo + "/" + baseNameFile;
            bool copied = false;
            if (!QFileInfo::exists(new_name + "." + extension)){
                copied = file.copy(copyFrom + "/" + files[i], new_name + "." + extension);
            }
            else{
                copied = file.copy(copyFrom + "/" + files[i], new_name + "_copy" + "." + extension);
            }
            if (!copied) {
                windowMessage(file, "Copy failed", "Could not copy file: %1\n%2");
            }
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
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();;
    QFile file(absPath);

    bool opened = file.open(QFile::ReadOnly | QFile::Text);
        if ( !opened ) {
            windowMessage(file, "Open failed", "Could not open file for reading: %1\n%2");
            return;
        }

        QTextEdit *txt = new QTextEdit();
        QTextStream ReadFile(&file);
        txt->setText(ReadFile.readAll());
        txt->show();
}

void FileSystem::renameItem() {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileSysModel->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();

    bool ok;
       QString text = QInputDialog::getText(0, "Input dialog",
                                            "New name:", QLineEdit::Normal,
                                            "", &ok);
       if (ok && !text.isEmpty()) {
           if (fileInfo.isDir()) {
                   QDir dir(absPath);
                   qInfo() << absPath;
                   bool renamedDir = dir.rename(dir.path(), fileInfo.dir().absolutePath()+'/'+text);
                   qInfo() << renamedDir;
                   if (!renamedDir){
                       QMessageBox::critical(
                           this,
                           tr("Rename failed"),
                           tr("Could not rename directory: %1").arg(dir.dirName())
                           );
                       return;
                   }
               }
               else if (fileInfo.isFile()){
                   QFile file(absPath);
                   extension = fileInfo.completeSuffix();
                   bool renamedFile = file.rename(file.fileName(), fileInfo.absolutePath()+'/'+text + "." + extension);
                   qInfo() << renamedFile;
                   if (!renamedFile){
                       windowMessage(file,"Rename failed", "Could not rename file: %1\n%2");
                       return;
                   }
           }
      }
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
