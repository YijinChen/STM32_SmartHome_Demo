/****************************************************************************
** Meta object code from reading C++ file 'smarthome.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../smarthome.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smarthome.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSsmarthomeENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSsmarthomeENDCLASS = QtMocHelpers::stringData(
    "smarthome",
    "connect_server_success",
    "",
    "message_receive",
    "msg",
    "QMqttTopicName",
    "topic",
    "on_livingroomButton_clicked",
    "on_bedroomButton_clicked",
    "on_kitchenButton_clicked",
    "on_washroomButton_clicked",
    "on_alarmButton_clicked",
    "on_tabWidget_tabBarClicked",
    "index"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSsmarthomeENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    2,   63,    2, 0x08,    2 /* Private */,
       7,    0,   68,    2, 0x08,    5 /* Private */,
       8,    0,   69,    2, 0x08,    6 /* Private */,
       9,    0,   70,    2, 0x08,    7 /* Private */,
      10,    0,   71,    2, 0x08,    8 /* Private */,
      11,    0,   72,    2, 0x08,    9 /* Private */,
      12,    1,   73,    2, 0x08,   10 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject smarthome::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSsmarthomeENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSsmarthomeENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSsmarthomeENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<smarthome, std::true_type>,
        // method 'connect_server_success'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'message_receive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QMqttTopicName &, std::false_type>,
        // method 'on_livingroomButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_bedroomButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_kitchenButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_washroomButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_alarmButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_tabBarClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void smarthome::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<smarthome *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connect_server_success(); break;
        case 1: _t->message_receive((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QMqttTopicName>>(_a[2]))); break;
        case 2: _t->on_livingroomButton_clicked(); break;
        case 3: _t->on_bedroomButton_clicked(); break;
        case 4: _t->on_kitchenButton_clicked(); break;
        case 5: _t->on_washroomButton_clicked(); break;
        case 6: _t->on_alarmButton_clicked(); break;
        case 7: _t->on_tabWidget_tabBarClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QMqttTopicName >(); break;
            }
            break;
        }
    }
}

const QMetaObject *smarthome::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *smarthome::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSsmarthomeENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int smarthome::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
