/********************************************************************************
** Form generated from reading UI file 'filemanager.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEMANAGER_H
#define UI_FILEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileManager
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTableView *tableView_2;
    QTableView *tableView;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *FileManager)
    {
        if (FileManager->objectName().isEmpty())
            FileManager->setObjectName(QString::fromUtf8("FileManager"));
        FileManager->resize(926, 561);
        centralwidget = new QWidget(FileManager);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName(QString::fromUtf8("tableView_2"));
        tableView_2->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tableView_2->horizontalHeader()->setCascadingSectionResizes(true);
        tableView_2->horizontalHeader()->setDefaultSectionSize(100);
        tableView_2->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout->addWidget(tableView_2);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setStretchLastSection(false);

        horizontalLayout->addWidget(tableView);

        FileManager->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(FileManager);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 926, 26));
        FileManager->setMenuBar(menuBar);

        retranslateUi(FileManager);

        QMetaObject::connectSlotsByName(FileManager);
    } // setupUi

    void retranslateUi(QMainWindow *FileManager)
    {
        FileManager->setWindowTitle(QCoreApplication::translate("FileManager", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileManager: public Ui_FileManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEMANAGER_H
