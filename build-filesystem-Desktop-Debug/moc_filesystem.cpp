/****************************************************************************
** Meta object code from reading C++ file 'filesystem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/filesystem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filesystem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FileSystem_t {
    QByteArrayData data[13];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileSystem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileSystem_t qt_meta_stringdata_FileSystem = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FileSystem"
QT_MOC_LITERAL(1, 11, 12), // "clickedFirst"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 37, 5), // "index"
QT_MOC_LITERAL(5, 43, 13), // "clickedSecond"
QT_MOC_LITERAL(6, 57, 10), // "deleteItem"
QT_MOC_LITERAL(7, 68, 12), // "copyItemFrom"
QT_MOC_LITERAL(8, 81, 12), // "copyFromPath"
QT_MOC_LITERAL(9, 94, 10), // "copyItemTo"
QT_MOC_LITERAL(10, 105, 10), // "copyToPath"
QT_MOC_LITERAL(11, 116, 8), // "openFile"
QT_MOC_LITERAL(12, 125, 10) // "renameItem"

    },
    "FileSystem\0clickedFirst\0\0QModelIndex\0"
    "index\0clickedSecond\0deleteItem\0"
    "copyItemFrom\0copyFromPath\0copyItemTo\0"
    "copyToPath\0openFile\0renameItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileSystem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08 /* Private */,
       5,    1,   62,    2, 0x08 /* Private */,
       6,    0,   65,    2, 0x08 /* Private */,
       7,    1,   66,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x28 /* Private | MethodCloned */,
       9,    1,   70,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x28 /* Private | MethodCloned */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FileSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileSystem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clickedFirst((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->clickedSecond((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->deleteItem(); break;
        case 3: _t->copyItemFrom((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->copyItemFrom(); break;
        case 5: _t->copyItemTo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->copyItemTo(); break;
        case 7: _t->openFile(); break;
        case 8: _t->renameItem(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FileSystem::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_FileSystem.data,
    qt_meta_data_FileSystem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FileSystem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileSystem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileSystem.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FileSystem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
