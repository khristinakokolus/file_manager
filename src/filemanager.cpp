#include "filemanager.h"
#include "ui_filemanager.h"

#include <QTreeView>
#include <QMenu>
#include <QShortcut>
#include <QDir>
#include <QDebug>
#include <QTextEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QProcess>

FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileManager)
{
    ui->setupUi(this);

    QString sysPath = "";
    fileManager = new QFileSystemModel(this);
    fileManager->setFilter(QDir::AllEntries);
    fileManager->setRootPath(sysPath);

    clickedTreeViewFirst = false;
    clickedTreeViewSecond = false;

    ui->tableView->setModel(fileManager);
    ui->tableView_2->setModel(fileManager);

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_tableView_doubleClicked(QModelIndex)));
    connect(ui->tableView_2, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_tableView_2_doubleClicked(QModelIndex)));

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedFirst(QModelIndex)));
    connect(ui->tableView_2, SIGNAL(clicked(QModelIndex)), this, SLOT(clickedSecond(QModelIndex)));

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

    auto runAction = new QAction("Run", this);
    runAction->setShortcut(QKeySequence(Qt::Key_Enter));
    connect(runAction, SIGNAL(triggered()), this, SLOT(runProgram()));

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addActions({ openAction, copyAction, insertAction, deleteAction, renameAction, runAction });

    ui->tableView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView_2->addActions({ openAction, copyAction, insertAction, deleteAction, renameAction, runAction });
}



void FileManager::windowMessage(QFile& file, const char *name, const char *text){
    QMessageBox::critical(
        this,
        tr(name),
        tr(text).arg(file.fileName(), file.errorString() )
        );
    return;
}


void FileManager::deleteItem() {
    QMessageBox quitMsg;
    quitMsg.setWindowTitle("FileManager");
    quitMsg.setText("Do you really want to delete?");
    quitMsg.setStandardButtons(QMessageBox::Yes |
    QMessageBox::Cancel);
    quitMsg.setDefaultButton(QMessageBox::Cancel);
    if (quitMsg.exec() == QMessageBox::Yes)
        deletion();

}

QModelIndex FileManager::getIndex() {
    QModelIndex index;

    if (clickedTreeViewFirst == true) {
        index = ui->tableView->currentIndex();
        clickedTreeViewFirst = false;
    }
    else if (clickedTreeViewSecond == true) {
        index = ui->tableView_2->currentIndex();
        clickedTreeViewSecond = false;
    }
    return index;

}


void FileManager::deletion() {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileManager->fileInfo(index);
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


void FileManager::copyItemFrom(QString copyFromPath) {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileManager->fileInfo(index);

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


void FileManager::copyItemTo(QString copyToPath) {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileManager->fileInfo(index);

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
            FileManager::copyItemFrom(copyFrom + "/" + files[i]);
            FileManager::copyItemTo(copyTo + "/" + dirName);
        }
        if(!(map.isEmpty())) {
            QString from = map.first().first;
            QString to = map.first().second;
            FileManager::copyItemFrom(from + "/" + map.firstKey().split('/')[1]);
            map.remove(map.firstKey());
            FileManager::copyItemTo(to + "/" + dirName);
        }
    }

void FileManager::openFile() {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileManager->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();;
    QFile file(absPath);
    bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(absPath));
        if ( !opened ) {
            windowMessage(file, "Open failed", "Could not open file: %1\n%2");
            return;
        }
}

void FileManager::renameItem() {
    QModelIndex index = getIndex();

    QFileInfo fileInfo = fileManager->fileInfo(index);
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



void FileManager::runProgram() {
    QModelIndex index = getIndex();
    QFileInfo fileInfo = fileManager->fileInfo(index);

    QString absPath = fileInfo.absoluteFilePath();
    QProcess::startDetached(absPath);

}


void FileManager::clickedFirst(const QModelIndex &index) {
    clickedTreeViewFirst = true;
}


void FileManager::clickedSecond(const QModelIndex &index) {
    clickedTreeViewSecond = true;
}


FileManager::~FileManager()
{
    delete ui;
}


void FileManager::on_tableView_doubleClicked(const QModelIndex &index)
{
    QFileInfo fileInfo = fileManager->fileInfo(index);
    if (fileInfo.fileName() == "..") {
       QDir directory = fileInfo.dir();
       directory.cdUp();
       ui->tableView->setRootIndex(fileManager->index(directory.absolutePath()));
    }
    else if (fileInfo.fileName() == ".") {
        ui->tableView->setRootIndex(fileManager->index(""));
    }
    else if (fileInfo.isDir()) {
        ui->tableView->setRootIndex(index);
    }

}


void FileManager::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    QFileInfo fileInfo = fileManager->fileInfo(index);
    if (fileInfo.fileName() == "..") {
       QDir directory = fileInfo.dir();
       directory.cdUp();
       ui->tableView_2->setRootIndex(fileManager->index(directory.absolutePath()));
    }
    else if (fileInfo.fileName() == ".") {
        ui->tableView_2->setRootIndex(fileManager->index(""));
    }
    else if (fileInfo.isDir()) {
        ui->tableView_2->setRootIndex(index);
    }
}

