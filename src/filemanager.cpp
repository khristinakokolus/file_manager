#include "../inc/filemanager.h"
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
#include <QProgressDialog>
#include <QMimeDatabase>


#include <QMutex>

#include <QTextEdit>
FileManager::FileManager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileManager)
{
    ui->setupUi(this);
    this->setWindowTitle("File manager");

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



    QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
    QMenu *findFilesMenu = menuBar()->addMenu(tr("Find Files"));
    QMenu *createDirectoryMenu = menuBar()->addMenu(tr("Create directory"));
    QMenu *createFileMenu = menuBar()->addMenu(tr("Create file"));
    helpMenu->addAction(tr("How to use quide"));
    findFilesMenu->addAction(tr("Find"));
    createDirectoryMenu->addAction(tr("Create"));
    createFileMenu->addAction(tr("Create"));


    connect(helpMenu, SIGNAL(triggered(QAction*))
            , SLOT(help()));
    connect(findFilesMenu, SIGNAL(triggered(QAction*))
            , SLOT(searchFiles()));
    connect(createDirectoryMenu, SIGNAL(triggered(QAction*))
            , SLOT(createDirectory()));
    connect(createFileMenu, SIGNAL(triggered(QAction*))
            , SLOT(createFile()));


    auto deleteAction = new QAction("Delete", this);
    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    auto copyAction = new QAction("Copy", this);
    copyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyItemFrom()));

    auto createDirectoryAction = new QAction("Create directory", this);
    createDirectoryAction->setShortcut(QKeySequence(Qt::Key_D));
    connect(createDirectoryAction, SIGNAL(triggered()), this, SLOT(createDirectory()));

    auto createFileAction = new QAction("Create file", this);
    createFileAction->setShortcut(QKeySequence(Qt::Key_F));
    connect(createFileAction, SIGNAL(triggered()), this, SLOT(createFile()));

    auto insertAction = new QAction("Insert", this);
    insertAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));
    connect(insertAction, SIGNAL(triggered()), this, SLOT(copyItemTo()));

    auto openAction = new QAction("Open", this);
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    auto openHexAction = new QAction("Open in hex", this);
    openHexAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    connect(openHexAction, SIGNAL(triggered()), this, SLOT(openHexFile()));

    auto renameAction = new QAction("Rename", this);
    renameAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    connect(renameAction, SIGNAL(triggered()), this, SLOT(renameItem()));

    auto runAction = new QAction("Run", this);
    runAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(runAction, SIGNAL(triggered()), this, SLOT(runProgram()));

    auto searchAction = new QAction("Search", this);
    searchAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    connect(searchAction, SIGNAL(triggered()), this, SLOT(searchFiles()));

    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->addActions({ openAction, openHexAction, copyAction, createDirectoryAction, createFileAction,
                                insertAction, deleteAction, renameAction, runAction, searchAction });

    ui->tableView_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView_2->addActions({ openAction, openHexAction, copyAction, createDirectoryAction, createFileAction,
                                  insertAction, deleteAction, renameAction, runAction, searchAction });
}


void FileManager::help(){
    QString documentationText = "Welcome to a simple File Manager!\n\n\n Here is a small quide how to use it. All actions that can be performed using right mouse button:\n\n\n"
                                "1. Delete - delete the file or directory. Shortcut - Delete\n\n"
                                "2. Copy - copy file or directory. Shortcut - CTRL+C\n\n"
                                "3. Create directory - create new directory. Shortcut - D\n\n"
                                "4. Create file - create new file. Shortcut - F\n\n"
                                "5. Insert - insert copied file to the certain directory. Shortcut - CTRL+V\n\n"
                                "6. Open - open file text file in text editor. Shortcut - CTRL+O\n\n"
                                "7. Open in hex - open file as hex representation. Shortcut - CTRL+H\n\n"
                                "8. Rename - rename file or directory. Shortcut - CTRL+R\n\n"
                                "9. Run - run the program. Shortcut - Enter\n\n"
                                "10. Search - search for files in a certain directory and its subdirectories. Shortcut - CTRL+F\n\n";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Usage guide");
    msgBox.setText(documentationText);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgBox.exec();
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


void FileManager::openHexFile() {
    QMutex m_dataMtx;

    QModelIndex index = getIndex();
    QFileInfo fileInfo = fileManager->fileInfo(index);
    QString absPath = fileInfo.absoluteFilePath();;
    QFile file(absPath);
    if (!file.open(QIODevice::ReadWrite))
        {
            windowMessage(file, "Open failed", "Could not open file: %1\n%2");
          return;
        }
    int bytes_per_line = 16;

//    QMutexLocker lock(&m_dataMtx);
    QByteArray arr = file.readAll();
    int last_idx = arr.size() / bytes_per_line;

    QTextEdit *textEdit = new QTextEdit();
//    setFont(QFont("Courier", 10));
    for (int line_idx = 0;  line_idx <= last_idx; line_idx += 1){
        QString address = QString("%1").arg(line_idx * bytes_per_line, 10, 16, QChar('0'));

        textEdit->moveCursor((QTextCursor::End));
        textEdit->insertPlainText(address+"\t|\t");
        int blength = 0;
        for(int i=0; i< bytes_per_line && (line_idx * bytes_per_line + i) < arr.size(); i++){
            QString val = QString::number((arr.at(line_idx * bytes_per_line + i) & 0xF0) >> 4, 16);
            textEdit->moveCursor((QTextCursor::End));
            textEdit->insertPlainText(val);

            val = QString::number((arr.at(line_idx * bytes_per_line + i) & 0xF), 16);
            textEdit->moveCursor((QTextCursor::End));
            textEdit->insertPlainText(val+' ');
            blength = i;
        }
        if (blength != bytes_per_line){
            for (int i = 0; i <= bytes_per_line - blength; i++){
                textEdit->moveCursor((QTextCursor::End));
                textEdit->insertPlainText("     ");
            }

        }
        textEdit->moveCursor((QTextCursor::End));
        textEdit->insertPlainText("\t|\t");
        for (int i=0; (line_idx * bytes_per_line + i) < arr.size() && (i < bytes_per_line); i++)
        {
            char ch = arr[line_idx * bytes_per_line + i];
            if ((ch < 0x20) || (ch > 0x7e))
            ch = '.';
            textEdit->moveCursor((QTextCursor::End));
            textEdit->insertPlainText(QString(ch)+' ');
        }
        textEdit->append("\n");



    }
    textEdit->show();

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

void FileManager::searchFiles() {
    bool ok;

    QString path = QInputDialog::getText(0, "Directory to search in",
                                         "Directory:", QLineEdit::Normal,
                                         "", &ok);
    QString fileToFind = QInputDialog::getText(0, "Finding files",
                                         "File to find:", QLineEdit::Normal,
                                         "", &ok);
    QStringList filter;
    if (!fileToFind.isEmpty())
        filter << fileToFind;
    QDirIterator it(path, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next();
    files = findFiles(files, fileToFind);
    files.sort();

    QString found;

    if (files.empty()) {
        found = "There are no files containing in the name " "'"  + fileToFind + "'     ";
    }
    else {
        found = "Found files containing in the name " "'"  + fileToFind + "': " + "\n";
        for (auto &file : files) {
            found += file + "\n";
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Found files");
    msgBox.setText(found);
    msgBox.addButton(QMessageBox::Open);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgBox.setStandardButtons(QMessageBox::Open |
    QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Open)
        openFileByPath();
}


void FileManager::openFileByPath() {
    bool ok;
    QString fileToOpen = QInputDialog::getText(0, "File opening",
                                         "File to open:", QLineEdit::Normal,
                                         "", &ok);
    QFile file(fileToOpen);
    bool opened = QDesktopServices::openUrl(QUrl::fromLocalFile(fileToOpen));
        if ( !opened ) {
            windowMessage(file, "Open failed", "Could not open file: %1\n%2");
            return;
        }
}

QStringList FileManager::findFiles(const QStringList &files, const QString &text) {
    QProgressDialog progressDialog(this);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Find Files"));


    QStringList foundFiles;
    qInfo() << files.size();
    for (int i = 0; i < files.size(); ++i) {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %n...", nullptr, files.size()).arg(i));
        QCoreApplication::processEvents();
        const QString fileName = files.at(i);
        QFile file(fileName);
        if (fileName.contains(text, Qt::CaseInsensitive)) {
            foundFiles << files[i];
        }
        if (progressDialog.wasCanceled())
            break;
    }
    return foundFiles;

}

void FileManager::createDirectory() {
    bool ok;

    QString absPath = QInputDialog::getText(0, "Directory whaere to create new directory",
                                         "Directory name:", QLineEdit::Normal,
                                         "", &ok);

    QString dirName = QInputDialog::getText(0, "Creating new directory",
                                            "New directory name:", QLineEdit::Normal,
                                            "", &ok);
    QDir dir(absPath);
    dir.mkdir(dirName);
}

void FileManager::createFile() {
    bool ok;
    QString absPath = QInputDialog::getText(0, "Directory where to create new file",
                                         "Directory name:", QLineEdit::Normal,
                                         "", &ok);
    QString fileName = QInputDialog::getText(0, "Creating new file",
                                            "New file name:", QLineEdit::Normal,
                                            "", &ok);
    QFile file(absPath + fileName);
    file.open(QIODevice::WriteOnly);
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

