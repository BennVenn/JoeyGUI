/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
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
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "pollSerialPorts",
    "",
    "on_actionUpdate_Firmware_triggered",
    "on_pushButton_DownloadROM_clicked",
    "handleSerialData",
    "FetchCartData",
    "Init",
    "requestAndReceiveROM",
    "uint8_t",
    "MBC",
    "uint32_t",
    "offset",
    "requestAndReceiveRAM",
    "sendByte",
    "Mode",
    "Address",
    "Data",
    "getBytes",
    "swapBits",
    "temp",
    "QRY",
    "AppendWord",
    "datastring",
    "value",
    "populateComboBoxWithALGFiles",
    "folderPath",
    "readALGFile",
    "filePath",
    "ALGData&",
    "algData",
    "InterrogateCFI",
    "isCFIpresent",
    "on_actionAbout_triggered",
    "on_actionAbout_QT_triggered",
    "on_pushButton_DownloadSave_clicked",
    "on_pushButton_UploadSave_clicked",
    "on_pushButton_Detect_clicked",
    "on_pushButton_UploadROM_clicked",
    "on_comboBox_FlashCartType_currentIndexChanged",
    "index",
    "on_pushButton_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[84];
    char stringdata0[11];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[35];
    char stringdata4[34];
    char stringdata5[17];
    char stringdata6[14];
    char stringdata7[5];
    char stringdata8[21];
    char stringdata9[8];
    char stringdata10[4];
    char stringdata11[9];
    char stringdata12[7];
    char stringdata13[21];
    char stringdata14[9];
    char stringdata15[5];
    char stringdata16[8];
    char stringdata17[5];
    char stringdata18[9];
    char stringdata19[9];
    char stringdata20[5];
    char stringdata21[4];
    char stringdata22[11];
    char stringdata23[11];
    char stringdata24[6];
    char stringdata25[29];
    char stringdata26[11];
    char stringdata27[12];
    char stringdata28[9];
    char stringdata29[9];
    char stringdata30[8];
    char stringdata31[15];
    char stringdata32[13];
    char stringdata33[25];
    char stringdata34[28];
    char stringdata35[35];
    char stringdata36[33];
    char stringdata37[29];
    char stringdata38[32];
    char stringdata39[46];
    char stringdata40[6];
    char stringdata41[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 15),  // "pollSerialPorts"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 34),  // "on_actionUpdate_Firmware_trig..."
        QT_MOC_LITERAL(63, 33),  // "on_pushButton_DownloadROM_cli..."
        QT_MOC_LITERAL(97, 16),  // "handleSerialData"
        QT_MOC_LITERAL(114, 13),  // "FetchCartData"
        QT_MOC_LITERAL(128, 4),  // "Init"
        QT_MOC_LITERAL(133, 20),  // "requestAndReceiveROM"
        QT_MOC_LITERAL(154, 7),  // "uint8_t"
        QT_MOC_LITERAL(162, 3),  // "MBC"
        QT_MOC_LITERAL(166, 8),  // "uint32_t"
        QT_MOC_LITERAL(175, 6),  // "offset"
        QT_MOC_LITERAL(182, 20),  // "requestAndReceiveRAM"
        QT_MOC_LITERAL(203, 8),  // "sendByte"
        QT_MOC_LITERAL(212, 4),  // "Mode"
        QT_MOC_LITERAL(217, 7),  // "Address"
        QT_MOC_LITERAL(225, 4),  // "Data"
        QT_MOC_LITERAL(230, 8),  // "getBytes"
        QT_MOC_LITERAL(239, 8),  // "swapBits"
        QT_MOC_LITERAL(248, 4),  // "temp"
        QT_MOC_LITERAL(253, 3),  // "QRY"
        QT_MOC_LITERAL(257, 10),  // "AppendWord"
        QT_MOC_LITERAL(268, 10),  // "datastring"
        QT_MOC_LITERAL(279, 5),  // "value"
        QT_MOC_LITERAL(285, 28),  // "populateComboBoxWithALGFiles"
        QT_MOC_LITERAL(314, 10),  // "folderPath"
        QT_MOC_LITERAL(325, 11),  // "readALGFile"
        QT_MOC_LITERAL(337, 8),  // "filePath"
        QT_MOC_LITERAL(346, 8),  // "ALGData&"
        QT_MOC_LITERAL(355, 7),  // "algData"
        QT_MOC_LITERAL(363, 14),  // "InterrogateCFI"
        QT_MOC_LITERAL(378, 12),  // "isCFIpresent"
        QT_MOC_LITERAL(391, 24),  // "on_actionAbout_triggered"
        QT_MOC_LITERAL(416, 27),  // "on_actionAbout_QT_triggered"
        QT_MOC_LITERAL(444, 34),  // "on_pushButton_DownloadSave_cl..."
        QT_MOC_LITERAL(479, 32),  // "on_pushButton_UploadSave_clicked"
        QT_MOC_LITERAL(512, 28),  // "on_pushButton_Detect_clicked"
        QT_MOC_LITERAL(541, 31),  // "on_pushButton_UploadROM_clicked"
        QT_MOC_LITERAL(573, 45),  // "on_comboBox_FlashCartType_cur..."
        QT_MOC_LITERAL(619, 5),  // "index"
        QT_MOC_LITERAL(625, 21)   // "on_pushButton_clicked"
    },
    "MainWindow",
    "pollSerialPorts",
    "",
    "on_actionUpdate_Firmware_triggered",
    "on_pushButton_DownloadROM_clicked",
    "handleSerialData",
    "FetchCartData",
    "Init",
    "requestAndReceiveROM",
    "uint8_t",
    "MBC",
    "uint32_t",
    "offset",
    "requestAndReceiveRAM",
    "sendByte",
    "Mode",
    "Address",
    "Data",
    "getBytes",
    "swapBits",
    "temp",
    "QRY",
    "AppendWord",
    "datastring",
    "value",
    "populateComboBoxWithALGFiles",
    "folderPath",
    "readALGFile",
    "filePath",
    "ALGData&",
    "algData",
    "InterrogateCFI",
    "isCFIpresent",
    "on_actionAbout_triggered",
    "on_actionAbout_QT_triggered",
    "on_pushButton_DownloadSave_clicked",
    "on_pushButton_UploadSave_clicked",
    "on_pushButton_Detect_clicked",
    "on_pushButton_UploadROM_clicked",
    "on_comboBox_FlashCartType_currentIndexChanged",
    "index",
    "on_pushButton_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  164,    2, 0x08,    1 /* Private */,
       3,    0,  165,    2, 0x08,    2 /* Private */,
       4,    0,  166,    2, 0x08,    3 /* Private */,
       5,    0,  167,    2, 0x08,    4 /* Private */,
       6,    0,  168,    2, 0x08,    5 /* Private */,
       7,    0,  169,    2, 0x08,    6 /* Private */,
       8,    2,  170,    2, 0x08,    7 /* Private */,
      13,    2,  175,    2, 0x08,   10 /* Private */,
      14,    3,  180,    2, 0x08,   13 /* Private */,
      18,    2,  187,    2, 0x08,   17 /* Private */,
      19,    1,  192,    2, 0x08,   20 /* Private */,
      21,    2,  195,    2, 0x08,   22 /* Private */,
      22,    2,  200,    2, 0x08,   25 /* Private */,
      25,    1,  205,    2, 0x08,   28 /* Private */,
      27,    2,  208,    2, 0x08,   30 /* Private */,
      31,    0,  213,    2, 0x08,   33 /* Private */,
      32,    0,  214,    2, 0x08,   34 /* Private */,
      33,    0,  215,    2, 0x08,   35 /* Private */,
      34,    0,  216,    2, 0x08,   36 /* Private */,
      35,    0,  217,    2, 0x08,   37 /* Private */,
      36,    0,  218,    2, 0x08,   38 /* Private */,
      37,    0,  219,    2, 0x08,   39 /* Private */,
      38,    0,  220,    2, 0x08,   40 /* Private */,
      39,    1,  221,    2, 0x08,   41 /* Private */,
      41,    0,  224,    2, 0x08,   43 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,   12,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,   12,
    QMetaType::Void, QMetaType::Char, QMetaType::UInt, QMetaType::UInt,   15,   16,   17,
    QMetaType::QByteArray, QMetaType::Char, QMetaType::UInt,   15,   16,
    QMetaType::QByteArray, QMetaType::QByteArray,   20,
    QMetaType::UChar, QMetaType::UInt, QMetaType::UInt,   16,   17,
    QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::UInt,   23,   24,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 29,   28,   30,
    QMetaType::Void,
    QMetaType::UInt,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'pollSerialPorts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionUpdate_Firmware_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_DownloadROM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'FetchCartData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Init'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'requestAndReceiveROM'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        // method 'requestAndReceiveRAM'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        // method 'sendByte'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'getBytes'
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'swapBits'
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'QRY'
        QtPrivate::TypeAndForceComplete<unsigned char, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'AppendWord'
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'populateComboBoxWithALGFiles'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'readALGFile'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<ALGData &, std::false_type>,
        // method 'InterrogateCFI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'isCFIpresent'
        QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>,
        // method 'on_actionAbout_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAbout_QT_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_DownloadSave_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_UploadSave_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Detect_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_UploadROM_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_comboBox_FlashCartType_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_pushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->pollSerialPorts(); break;
        case 1: _t->on_actionUpdate_Firmware_triggered(); break;
        case 2: _t->on_pushButton_DownloadROM_clicked(); break;
        case 3: _t->handleSerialData(); break;
        case 4: _t->FetchCartData(); break;
        case 5: _t->Init(); break;
        case 6: _t->requestAndReceiveROM((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2]))); break;
        case 7: _t->requestAndReceiveRAM((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2]))); break;
        case 8: _t->sendByte((*reinterpret_cast< std::add_pointer_t<char>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[3]))); break;
        case 9: { QByteArray _r = _t->getBytes((*reinterpret_cast< std::add_pointer_t<char>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 10: { QByteArray _r = _t->swapBits((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 11: { uchar _r = _t->QRY((*reinterpret_cast< std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[2])));
            if (_a[0]) *reinterpret_cast< uchar*>(_a[0]) = std::move(_r); }  break;
        case 12: { QByteArray _r = _t->AppendWord((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->populateComboBoxWithALGFiles((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: { bool _r = _t->readALGFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ALGData&>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->InterrogateCFI(); break;
        case 16: { uint _r = _t->isCFIpresent();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 17: _t->on_actionAbout_triggered(); break;
        case 18: _t->on_actionAbout_QT_triggered(); break;
        case 19: _t->on_pushButton_DownloadSave_clicked(); break;
        case 20: _t->on_pushButton_UploadSave_clicked(); break;
        case 21: _t->on_pushButton_Detect_clicked(); break;
        case 22: _t->on_pushButton_UploadROM_clicked(); break;
        case 23: _t->on_comboBox_FlashCartType_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}
QT_WARNING_POP
