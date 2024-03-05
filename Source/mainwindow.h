#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QProgressBar>
#include <QMap>
#include <QString>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
#define ROMdownloadID 0x01
#define RAMdownloadID 0x02
#define StatusID 0x03
#define AcknowledgeID 0xFF
#define GBC 0x01
#define GBA 0x02
#define UNKNOWN 0x03

#define State_Ready 0
#define State_ROM_Rd 1
#define State_RAM_Rd 2
#define State_ROM_Wr 3
#define State_RAM_Wr 4
#define State_FW_Wr 5
#define State_Bypass 6
#define State_Erase 7

#define byteWriteBufferSize 32


    unsigned int MBC3000_FCE[12]={0xAAA,0xAA,0x555,0x55,0xAAA,0x80,0xAAA,0xAA,0x555,0x55,0xAAA,0x10};
    unsigned int MBC3000_PRG[6]={0xAAA,0xAA,0x555,0x55,0xAAA,0xA0};

    int StateMachine=State_Ready;

    QString Human="";
    QString hexString = "";
    bool connected=false;
    bool operationInProgress=false;
    QString ComPortNumber="";
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort serialPort;
    QByteArray receivedData;
    QMap<QString, QString> loadCSV(const QString& filename);
    QString findAsciiString(const QMap<QString, QString>& data, const QString& hexString);
    QString CartROMname16;
    QString CHK8;
    QString CHK16;
    QString GBAROMdat;
    QString GBASAVEdat;
    unsigned int cartType;
    unsigned int cartRomSize;
    unsigned int cartRamSize;
    unsigned int cartMapperType;
    unsigned int GBAsaveType;
    unsigned int CFIaddress;
    unsigned int CFIdata;
    unsigned int CFIFlashSize;
    unsigned int CFIEBregions;
    unsigned int CFIEBsize;
    unsigned int CFIBufferSize;
    unsigned int CFIFlashID;
    unsigned int EBcurrentSector;

    struct ALGData {
        unsigned int ALG_Mode;
        unsigned int ALG_BufferSize;
        unsigned int ALG_EraseMode;
        unsigned int ALG_EraseLen;
        unsigned int ALG_PrgAlg;
        unsigned int ALG_PrgLen;
        unsigned int ALG_Voltage;
        unsigned int EraseCommandAddresses[16];
        unsigned int EraseCommandDatas[16];
        unsigned int ProgramCommandAddresses[16];
        unsigned int ProgramCommandDatas[16];
    };
       ALGData algData;
    bool CFIbitsSwapped=false;

    bool deviceBusy=false;
    bool BypassRxHandler=false;
    bool ROMdownloadInProgress=false;
    bool RAMdownloadInProgress=false;
    bool FirmwareUpdateInProgress=false;
    QString OldHexString="";

    QByteArray ROMdataToFlash;
    unsigned int ROMdataToFlashOffset=0;
    unsigned int ROMdataToFlashFileSize=0;


private slots:
  //  void onConnectButtonClicked();
    void pollSerialPorts();
    void on_actionUpdate_Firmware_triggered();
    void on_pushButton_DownloadROM_clicked();
    void handleSerialData();
    void FetchCartData();
    void Init();
    void requestAndReceiveROM(uint8_t MBC, uint32_t offset);
    void requestAndReceiveRAM(uint8_t MBC, uint32_t offset);
    void sendByte(char Mode, unsigned int Address, unsigned int Data);
    QByteArray getBytes(char Mode, unsigned int Address);
    QByteArray swapBits(QByteArray temp);
    unsigned char QRY(unsigned int Address, unsigned int Data);
    QByteArray AppendWord(QByteArray datastring, unsigned int value);
    void populateComboBoxWithALGFiles(const QString &folderPath) ;
    bool readALGFile(const QString& filePath, ALGData& algData) ;


    void InterrogateCFI();
    unsigned int isCFIpresent();
    void on_actionAbout_triggered();

    void on_actionAbout_QT_triggered();

    void on_pushButton_DownloadSave_clicked();

    void on_pushButton_UploadSave_clicked();

    void on_pushButton_Detect_clicked();

    void on_pushButton_UploadROM_clicked();

    void on_pushButton_SRAMTEST_clicked();

    void on_comboBox_FlashCartType_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    bool isRxEnabled;
    Ui::MainWindow *ui;
    QTimer *pollTimer;
    // Function to update the image based on connection status
    void updateImage(bool connected);
    //QSerialPort serialPort;
    QProgressBar *progressBar;
    uint8_t MBC;
    uint32_t offset;
    uint32_t length;
};

#endif // MAINWINDOW_H
