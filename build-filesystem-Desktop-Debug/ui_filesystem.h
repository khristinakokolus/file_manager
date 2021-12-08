/********************************************************************************
** Form generated from reading UI file 'filesystem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILESYSTEM_H
#define UI_FILESYSTEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileSystem
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView_2;
    QTreeView *treeView;

    void setupUi(QMainWindow *FileSystem)
    {
        if (FileSystem->objectName().isEmpty())
            FileSystem->setObjectName(QString::fromUtf8("FileSystem"));
        FileSystem->resize(901, 604);
        centralwidget = new QWidget(FileSystem);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView_2 = new QTreeView(centralwidget);
        treeView_2->setObjectName(QString::fromUtf8("treeView_2"));

        horizontalLayout->addWidget(treeView_2);

        treeView = new QTreeView(centralwidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        horizontalLayout->addWidget(treeView);

        FileSystem->setCentralWidget(centralwidget);

        retranslateUi(FileSystem);

        QMetaObject::connectSlotsByName(FileSystem);
    } // setupUi

    void retranslateUi(QMainWindow *FileSystem)
    {
        FileSystem->setWindowTitle(QCoreApplication::translate("FileSystem", "FileSystem", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileSystem: public Ui_FileSystem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILESYSTEM_H
