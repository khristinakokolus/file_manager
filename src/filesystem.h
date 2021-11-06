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
//    void on_click_delete(const QModelIndex &index);
//    void on_systemFirst_doubleClicked(const QModelIndex &index);
//    void on_treeView_activated(const QModelIndex &index);
//    void showContextMenu(const QPoint&);

    void deleteItem();
    void on_treeView_2_doubleClicked(const QModelIndex &index);

    void on_treeView_2_activated(const QModelIndex &index);

//signals:
//    void clicked_delete(const QModelIndex &index);

private:
    Ui::FileSystem *ui;
    QFileSystemModel *fileSysModel;
    QAction* deleteAction;
    QMenu* contextMenu;
    QShortcut *shDel;

};
#endif // FILESYSTEM_H
