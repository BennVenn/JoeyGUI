#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QRegularExpression>
#include <QThread>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pollTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Connect signals and slots
    // connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(pollTimer, &QTimer::timeout, this, &MainWindow::pollSerialPorts);
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::handleSerialData);

    Init();
    // Load a different image and set it as the new pixmap for the QLabel
    // ui->labelImage->setFixedSize(newPixmap.size());
}


void MainWindow::Init(){

    // Configure and start the timer for polling
    pollTimer->start(1000);  // Poll every 1000 milliseconds (1 second)

    isRxEnabled = false;
    //hide the progress bar
    ui->progressBar->setVisible(false);
    ui->labelMode->setVisible(false);
    ui->labelROMTitle->setVisible(false);
    ui->labelROMCHK8->setVisible(false);
    ui->labelROMCHK16->setVisible(false);
    ui->labelROMMBC->setVisible(false);
    ui->labelROMROM->setVisible(false);
    ui->labelROMRAM->setVisible(false);
    ui->labelROMCPAT->setVisible(false);
    ui->labelGameThumbnail->setVisible(false);
    ui->pushButton_DownloadROM->setVisible(false);
    ui->pushButton_DownloadSave->setVisible(false);
    ui->pushButton_UploadSave->setVisible(false);
    ui->pushButton_UploadROM->setVisible(true);
    ui->labelLFN->setVisible(false);
    // Set the initial image (you can adjust the path to your image file)
    //  ui->labelImage->setPixmap(QPixmap("searching.gif"));
    // Create a QMovie object and set the GIF file path
    QMovie *gifMovie = new QMovie(QCoreApplication::applicationDirPath()+"/Assets/searching.gif");
    ui->labelGif->setMovie(gifMovie);
    ui->labelGif->setVisible(true);
    ui->labelGif->raise();
    gifMovie->start();

    ui->groupBox->lower();
    ui->groupBox_2->lower();
    //ui->groupBox->setVisible(false);
    //ui->checkBoxOverdump->setVisible(false);
    ui->comboBox_Bootleg->addItem("Default Settings");
    ui->comboBox_Bootleg->addItem("Battery + SRAM");
    ui->comboBox_Bootleg->addItem("Battery + A4 + SRAM");
    ui->comboBox_Bootleg->addItem("64K Flash Save");
    ui->comboBox_Bootleg->addItem("128K Flash Save");
    ui->comboBox_Bootleg->addItem("128K (A4) Flash Save");
    ui->comboBox_Bootleg->addItem("ROM Save 64K (Batteryless)");
    ui->comboBox_Bootleg->addItem("ROM Save 128K (Batteryless)");

    ui->comboBox_ROMoffset->addItem("ROM offset 0x18000000");

    ui->comboBox_battery->addItem("No Battery");
    ui->comboBox_battery->addItem("Battery");
    ui->plainTextEdit->setVisible(false);
    ui->comboBox_FlashCartType->setVisible(false);


    //populate Flash Cart list:

    QComboBox comboBox;

    // Specify the folder path
    QString folderPath = (QCoreApplication::applicationDirPath() + "/FlashCartAlgorithms");

    // Populate the combo box with .ALG files from the folder
    populateComboBoxWithALGFiles( folderPath);


    // Show the combo box
    comboBox.show();

}


void MainWindow::populateComboBoxWithALGFiles( const QString &folderPath) {
    // Clear the combo box
    ui->comboBox_FlashCartType->clear();
    ui->comboBox_FlashCartType->addItem("Select Flash Cart Type");

    // Create a QDir object for the specified folder path
    QDir folder(folderPath);

    // Set the name filters to include only files with .ALG extension
    QStringList nameFilters;
    nameFilters << "*.ALG";
    folder.setNameFilters(nameFilters);

    // Get the list of files matching the filters
    QFileInfoList fileInfoList = folder.entryInfoList();

    // Iterate through the list and populate the combo box
    for (const QFileInfo &fileInfo : fileInfoList) {
        // Add file name (without extension) to the combo box
        ui->comboBox_FlashCartType->addItem(fileInfo.baseName());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(bool connected)
{
    if (connected) {
        QString imagePath = (QCoreApplication::applicationDirPath() + "/Assets/found.png");
        QPixmap pixmap(imagePath);
        ui->labelGif->setPixmap(pixmap);
        ui->labelGif->setVisible(true);
        ui->labelGif->lower();
        ui->label_CFI->setVisible(true);
        ui->comboBox_FlashCartType->setVisible(true);
    } else {
        QMovie *gifMovie = new QMovie(QCoreApplication::applicationDirPath()+"/Assets/searching.gif");
        ui->labelGif->setMovie(gifMovie);
        ui->labelGif->setVisible(true);
        ui->labelGif->raise();
        gifMovie->start();
        ui->label_CFI->setVisible(false);
        ui->plainTextEdit->setVisible(false);
        ui->comboBox_FlashCartType->setVisible(false);

    }
}

void MainWindow::FetchCartData(){
    hexString="";
    QString hexStatusString="";
    //Read ROM Header
    QByteArray dataToSend = "\x02\x01"; //0x02 = Get Header, 0x01=rom title, 0x02=chk8, 0x03=chk16
    serialPort.write(dataToSend);
    QByteArray receivedData;

    if (serialPort.waitForReadyRead(10)) {
        // Reading response data
        receivedData = serialPort.readAll();
        //qDebug() << "Received data:" << receivedData;
        //ROM title
        for (int n=0;n<16;n++){
            hexString += QString("%1").arg(static_cast<quint8>(receivedData[n]), 2, 16, QLatin1Char('0'));
        }
        QByteArray CleanText;
        for (int i = 0; i < qMin(15, receivedData.size()); ++i) {
            char ch = receivedData[i];

            // Check if the character is a printable ASCII character
            if (ch >= 0x20 && ch <= 0x7E) {
                CleanText.append(ch);
            }
        }
        ui->labelROMTitle->setText(CleanText);
        ui->labelROMTitle->setVisible(true);
        //Checksum8
        hexString += QString("%1").arg(static_cast<quint8>(receivedData[16]), 2, 16, QLatin1Char('0'));
        //Checksum16
        hexString += QString("%1").arg(static_cast<quint8>(receivedData[17]), 2, 16, QLatin1Char('0'));
        hexString += QString("%1").arg(static_cast<quint8>(receivedData[18]), 2, 16, QLatin1Char('0'));

        //Status bytes Struct [Current Set Mode,MBC type, Joey Cart Voltage, Detected cart type]
        hexStatusString += QString("%1").arg(static_cast<quint8>(receivedData[19]), 2, 16, QLatin1Char('0'));
        hexStatusString += QString("%1").arg(static_cast<quint8>(receivedData[20]), 2, 16, QLatin1Char('0'));
        hexStatusString += QString("%1").arg(static_cast<quint8>(receivedData[21]), 2, 16, QLatin1Char('0'));
        hexStatusString += QString("%1").arg(static_cast<quint8>(receivedData[22]), 2, 16, QLatin1Char('0'));
        //qDebug()<<hexStatusString;
        cartType=receivedData[22];
        if (cartType==0x01) ui->labelMode->setText("Automatic Mode (GBC)");
        else if (cartType==0x02) ui->labelMode->setText("Automatic Mode (GBA)");
        else if (cartType==0x03) ui->labelMode->setText("Automatic Mode (Unknown)");



    }

    if (cartType==GBC){

        ui->labelROMCHK8->setText("CRC8: "+QString("%1").arg(static_cast<quint8>(receivedData[16]), 2, 16, QLatin1Char('0')));
        ui->labelROMCHK8->setVisible(true);
        ui->labelROMCHK8->raise();
        ui->labelROMCHK16->setText("CRC16: "+(QString("%1").arg(static_cast<quint8>(receivedData[17]), 2, 16, QLatin1Char('0')))+ (QString("%1").arg(static_cast<quint8>(receivedData[18]), 2, 16, QLatin1Char('0'))));
        ui->labelROMCHK16->setVisible(true);
        ui->labelROMCHK16->raise();

        dataToSend = "\x02\x04"; //0x04 = Get Header, 0x01=rom title, 0x02=chk8, 0x03=chk16, 0x04=MBC
        serialPort.write(dataToSend);

        if (serialPort.waitForReadyRead(10)) {
            // Reading response data
            QByteArray receivedData = serialPort.readAll();
            QString hexString = receivedData.toHex().toUpper();
            switch(receivedData[0]){
            case 0x00: Human="No Mapper";cartMapperType=0;break;
            case 0x01: Human="MBC1";cartMapperType=1;;break;
            case 0x02: Human="MBC1+RAM";cartMapperType=1;;break;
            case 0x03: Human="MBC1+RAM+BATTERY";cartMapperType=1;;break;
            case 0x05: Human="MBC2";cartMapperType=2;;break;
            case 0x06: Human="MBC2+BATTERY";cartMapperType=2;;break;
            case 0x08: Human="ROM+RAM";cartMapperType=0;;break;
            case 0x09: Human="ROM+RAM+BATTERY";cartMapperType=0;;break;
            case 0x0B: Human="MMM01";cartMapperType=8;;break;
            case 0x0C: Human="MMM01+RAM";cartMapperType=8;;break;
            case 0x0D: Human="MMM01+RAM+BATTERY";cartMapperType=8;;break;
            case 0x0F: Human="MBC3+TIMER+BATTERY";cartMapperType=3;;break;
            case 0x10: Human="MBC3+TIMER+RAM+BATTERY";cartMapperType=3;;break;
            case 0x11: Human="MBC3";cartMapperType=3;;break;
            case 0x12: Human="MBC3+RAM";cartMapperType=3;;break;
            case 0x13: Human="MBC3+RAM+BATTERY";cartMapperType=3;;break;
            case 0x19: Human="MBC5";cartMapperType=5;;break;
            case 0x1A: Human="MBC5+RAM";cartMapperType=5;;break;
            case 0x1B: Human="MBC5+RAM+BATTERY";cartMapperType=5;;break;
            case 0x1C: Human="MBC5+RUMBLE";cartMapperType=5;;break;
            case 0x1D: Human="MBC5+RUMBLE+RAM";cartMapperType=5;;break;
            case 0x1E: Human="MBC5+RUMBLE+RAM+BATTERY";cartMapperType=5;;break;
            case 0x20: Human="MBC6";cartMapperType=6;break;
            case 0x22: Human="MBC7+SENSOR+RUMBLE+RAM+BATTERY";cartMapperType=7;;break;
            case 0xFC: Human="POCKET CAMERA";cartMapperType=9;;break;
            case 0xFD: Human="BANDAI TAMA5";cartMapperType=10;;break;
            case 0xFE: Human="HuC3";cartMapperType=11;;break;
            case 0xFF: Human="HuC1+RAM+BATTERY";cartMapperType=12;;break;
            default: Human="Unknown";cartMapperType=0;
            }
            ui->labelROMMBC->setText("MBC: "+hexString + " ("+Human+")");
            ui->labelROMMBC->setVisible(true);
            ui->labelROMMBC->raise();
        }
        dataToSend = "\x02\x05"; //0x05 = Get Header, 0x01=rom title, 0x02=chk8, 0x03=chk16, 0x04=MBC, 0x05=ROMsize
        serialPort.write(dataToSend);
        if (serialPort.waitForReadyRead(10)) {
            QByteArray receivedData = serialPort.readAll();
            QString hexString = receivedData.toHex().toUpper();
            switch(receivedData[0]){
            case 0x00: Human="32KBytes";cartRomSize=32768;break;
            case 0x01: Human="64Kbytes";cartRomSize=65536;break;
            case 0x02: Human="128KBytes";cartRomSize=131072;break;
            case 0x03: Human="256KBytes";cartRomSize=262144;break;
            case 0x04: Human="512KBytes";cartRomSize=524288;break;
            case 0x05: Human="1MBytes";cartRomSize=1048576;break;
            case 0x06: Human="2MBytes";cartRomSize=2097152;break;
            case 0x07: Human="4MBytes";cartRomSize=4194304;break;
            case 0x08: Human="8MBytes";cartRomSize=8388608;break;
            case 0x52: Human="1.1MBytes";cartRomSize=2097152;break;
            case 0x53: Human="1.2MBytes";cartRomSize=2097152;break;
            case 0x54: Human="1.52MBytes";cartRomSize=2097152;break;
            default: Human="Unknown";
            }
            ui->labelROMROM->setText("ROM: "+hexString + " ("+Human+")");
            ui->labelROMROM->setVisible(true);
            ui->labelROMROM->raise();
        }
        dataToSend = "\x02\x06"; //0x06 = Get Header, 0x01=rom title, 0x02=chk8, 0x03=chk16, 0x04=MBC, 0x05=ROMsize, 0x06=Ramsize
        serialPort.write(dataToSend);
        if (serialPort.waitForReadyRead(10)) {
            // Reading response data
            QByteArray receivedData = serialPort.readAll();
            QString hexString = receivedData.toHex().toUpper();
            switch(receivedData[0]){
            case 0x00: Human="No SRAM";cartRamSize=0;break;
            case 0x01: Human="2KBytes";cartRamSize=2048;break;
            case 0x02: Human="8KBytes";cartRamSize=8096;break;
            case 0x03: Human="32KBytes";cartRamSize=32768;break;
            case 0x04: Human="128KBytes";cartRamSize=131072;break;
            case 0x05: Human="64KBytes";cartRamSize=65536;break;
            default: Human="Unknown";
            }
            if (cartRamSize==0){
                ui->pushButton_DownloadSave->setVisible(false);
                ui->pushButton_UploadSave->setVisible(false);
            }
            else
            {
                ui->pushButton_DownloadSave->setVisible(true);
                ui->pushButton_UploadSave->setVisible(true);
            }
            ui->labelROMRAM->setText("RAM: "+hexString + " ("+Human+")");
            ui->labelROMRAM->setVisible(true);
            ui->labelROMRAM->raise();
        }
        dataToSend = "\x02\x07"; //0x07 = Get Header, 0x01=rom title, 0x02=chk8, 0x03=chk16,, 0x04=MBC, 0x05=ROMsize, 0x06=Ramsize 7=compatibiilty
        serialPort.write(dataToSend);
        if (serialPort.waitForReadyRead(10)) {
            // Reading response data
            QByteArray receivedData = serialPort.readAll();
            QString hexString = receivedData.toHex().toUpper();
            switch (static_cast<unsigned char>(receivedData[0])){
            case 0x00: Human="DMG";break;
            case 0x80: Human="DMG+GBC";break;
            case 0xC0: Human="GBC Exclusive";break;
            default: Human="UNKNOWN";
            }
            ui->labelROMCPAT->setText("Compatibility: "+hexString + " ("+Human+")");
            ui->labelROMCPAT->setVisible(true);
            ui->labelROMCPAT->raise();
        }
        hexString=hexString.toUpper();


        //Code here to lookup chekcsum and rom timtle to get graphic
        if (OldHexString!=hexString){ //only grab the artwork if the cart has changed
            OldHexString=hexString;


            QString filename = QCoreApplication::applicationDirPath()+"/Assets/gbc.csv";
            QMap<QString, QString> csvData = loadCSV(filename);
            if (hexString==""){
                qDebug("ERROR! port disconnected or sumfink");
                serialPort.close();
                ui->labelGameThumbnail->setVisible(false);
                ui->pushButton_DownloadROM->setVisible(false);
                ui->pushButton_DownloadSave->setVisible(false);
                ui->pushButton_UploadSave->setVisible(false);
                Init();
                ui->labelLFN->setVisible(true);
                ui->labelLFN->setText("Joey Disconnected... Searching");
                ui->labelStatus->setText(""); //clear status bar
                ui->labelGif->raise();
                // Update the image based on the connection status
                updateImage(0);
                ui->labelMode->setVisible(true);
                QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
                opacityEffect->setOpacity(1);
                ui->labelGif->setGraphicsEffect(opacityEffect);
                connected=false;
                return;
            }

            // Find ASCII string for the given hex string
            QString asciiString = findAsciiString(csvData, hexString);
            //in the mantime
            ui->labelGameThumbnail->setVisible(true);
            ui->pushButton_DownloadROM->setVisible(true);
            ui->labelLFN->setVisible(true);
            ui->labelLFN->setText(asciiString);
            QString imagePath = QCoreApplication::applicationDirPath() + "/Artwork/" + asciiString + ".png";
            QPixmap pixmap(imagePath);

            if (!pixmap.isNull()) {
                ui->labelGameThumbnail->setPixmap(pixmap);
                ui->labelGameThumbnail->setVisible(true);
                ui->labelGameThumbnail->raise();
            }
            else
            {
                ui->labelGameThumbnail->clear();
                ui->labelGameThumbnail->setVisible(false);

            }

        }

    }
    else if (cartType==GBA){
        ui->labelROMCHK8->setText("Game ID: "+ QString(receivedData[12]) + QString(receivedData[13]) + QString(receivedData[14]) + QString(receivedData[15]));
        ui->labelROMCHK8->setVisible(true);
        ui->labelROMCHK8->raise();
        ui->labelROMCHK16->setVisible(true);
        ui->labelROMMBC->setVisible(true);
        ui->labelROMCPAT->setVisible(false);
        ui->labelROMRAM->setVisible(false);
        ui->labelROMROM->setVisible(false);

        //Parse ROMLIST.RAW here

        QFile file=(QCoreApplication::applicationDirPath()+"/Assets/ROMLIST.RAW");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Error: Unable to open the file.";
        }
        else{
            QString receivedDataROMLIST = QString::fromUtf8(file.readAll());
            QString asciiSubstring = QString(receivedData[12]) + QString(receivedData[13]) + QString(receivedData[14]) + QString(receivedData[15]);
            int asciiIndex = receivedDataROMLIST.indexOf(asciiSubstring);
            if (asciiIndex != -1) {
                QString numericSubstring = receivedDataROMLIST.mid(asciiIndex + asciiSubstring.length());
                QRegularExpression regex("[^0-9]");
                numericSubstring = numericSubstring.split(regex).at(0);
                if (numericSubstring.length() >= 2) {
                    QString resultChars = numericSubstring.left(2);
                    GBAROMdat = resultChars.at(1);
                    GBASAVEdat = resultChars.at(0);
                    switch(GBAROMdat.at(0).toLatin1()){
                    case'0':cartRomSize=4194304;ui->labelROMCHK16->setText("ROM Size 4MBytes (32mbit)");break;
                    case'1':cartRomSize=8388608;ui->labelROMCHK16->setText("ROM Size 8MBytes (64mbit)");break;
                    case'2':cartRomSize=16777216;ui->labelROMCHK16->setText("ROM Size 16MBytes (128mbit)");break;
                    case'3':cartRomSize=33554432;ui->labelROMCHK16->setText("ROM Size 32MBytes (256mbit)");break;
                    case'4':cartRomSize=1048576;ui->labelROMCHK16->setText("ROM Size 1MBytes (8mbit)");break;
                    case'5':cartRomSize=2097152;ui->labelROMCHK16->setText("ROM Size 2MBytes (16mbit)");break;
                    case'6':cartRomSize=262144;ui->labelROMCHK16->setText("ROM Size 256KBytes (2mbit)");break;
                    case'7':cartRomSize=131072;ui->labelROMCHK16->setText("ROM Size 128KBytes (1mbit)");break;
                    case'8':cartRomSize=524288;ui->labelROMCHK16->setText("ROM Size 512KBytes (4mbit)");break;
                    default:cartRomSize=33554432;ui->labelROMCHK16->setText("Unknown");
                    }
                    ui->pushButton_DownloadSave->setVisible(true);ui->pushButton_UploadSave->setVisible(true);
                    GBAsaveType=GBASAVEdat.at(0).toLatin1();
                    switch(GBASAVEdat.at(0).toLatin1()){
                    case'0':cartRamSize=512;ui->labelROMMBC->setText("EEPROM Size 512Bytes (4kbit)");break;
                    case'1':cartRamSize=8192;ui->labelROMMBC->setText("EEPROM Size 8KBytes (64kbit)");break;
                    case'2':cartRamSize=0;ui->labelROMMBC->setText("No Save File");ui->pushButton_DownloadSave->setVisible(false);ui->pushButton_UploadSave->setVisible(false);break;
                    case'3':cartRamSize=32768;ui->labelROMMBC->setText("SRAM Size 32KBytes (256kbit)");break;
                    case'4':cartRamSize=65536;ui->labelROMMBC->setText("Flash Save Size 64KBytes (512kbit)");break;
                    case'5':cartRamSize=131072;ui->labelROMMBC->setText("Flash Save Size 128KBytes (1mbit)");break;
                    case'6':cartRamSize=65536;ui->labelROMMBC->setText("SRAM Size 64KBytes (512kbit)");break;
                    default:cartRamSize=65536;ui->labelROMMBC->setText("Unknown");
                    }
                } else {
                    qDebug() << "Numeric substring has less than two characters.";
                }
            } else {
                qDebug() << "ASCII substring not found in the data.";
            }
            file.close();
        }

        QString filename = QCoreApplication::applicationDirPath()+"/Assets/GBA.csv";
        QMap<QString, QString> csvData = loadCSV(filename);
        // Find ASCII string for the given hex string
        QString asciiString = findAsciiString(csvData, QString(receivedData[12]) + QString(receivedData[13]) + QString(receivedData[14]) + QString(receivedData[15]));
        ui->labelGameThumbnail->setVisible(true);
        ui->pushButton_DownloadROM->setVisible(true);
        ui->labelLFN->setVisible(true);
        ui->labelLFN->setText(asciiString);
        QString imagePath = QCoreApplication::applicationDirPath() + "/Artwork/" + asciiString + ".png";
        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            ui->labelGameThumbnail->setPixmap(pixmap);
            ui->labelGameThumbnail->setVisible(true);
            ui->labelGameThumbnail->raise();
        }
        else
        {
            ui->labelGameThumbnail->clear();
            ui->labelGameThumbnail->setVisible(false);

        }
    }

    if (BypassRxHandler==false){
        ui->plainTextEdit->clear();
        if (isCFIpresent()==0){
            ui->label_CFI->setText("No Flash detected - Cart does not support reflashing");
            ui->pushButton_UploadROM->setVisible(true);
            ui->plainTextEdit->setVisible(false);
        }
        else
        {
            ui->label_CFI->setText("Flash detected - This cart is NOT genuine");
            ui->pushButton_UploadROM->setVisible(true);
            ui->plainTextEdit->setVisible(true);

            InterrogateCFI();


        }

    }
}

void MainWindow::pollSerialPorts() //called once a second
{

    if (connected==false){ //if not connected, poll ports to find joey
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            serialPort.setPort(info);
            qDebug() << "Trying to open serial port:" << info.portName();
            if (serialPort.open(QIODevice::ReadWrite)) {
                qDebug() << "Serial port opened successfully.";
                QByteArray dataToSend = "\x55\xAA";
                serialPort.write(dataToSend);
                // Wait for data to be written to the serial port
                if (serialPort.waitForBytesWritten(10)) {
                    qDebug() << "Data written successfully.";
                } else {
                    qDebug() << "Error writing data:" << serialPort.errorString();
                }
                if (serialPort.waitForReadyRead(10)) {
                    QByteArray receivedData = serialPort.readAll();
                    receivedData.remove(0, 1);
                    if (receivedData.contains("Joey")) {
                        qDebug() << "Device identified. Setting status to connected.";
                        ui->labelStatus->setText( receivedData);
                        connected = true;  // Set this based on your actual connection status
                        ui->labelGif->lower();
                        updateImage(connected);
                        ui->labelMode->setVisible(true);
                        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
                        opacityEffect->setOpacity(0.1);
                        ui->labelGif->setGraphicsEffect(opacityEffect);
                    }
                    else
                    {
                        serialPort.close();
                        qDebug() << "Serial port closed.";

                    }
                } else {
                    qDebug() << "No response within the timeout.";
                    serialPort.close();
                    qDebug() << "Serial port closed.";
                }
            } else {
                qDebug() << "Failed to open serial port:" << serialPort.errorString();
            }
        }
    }
    else
    { //if connected, and we're not busy, we'll fetch cart data
        if (StateMachine==State_Ready){
            FetchCartData();
        }
        if (StateMachine==State_Erase){
            if (algData.ALG_EraseMode == 0) {
                QByteArray temp = getBytes(cartType, 0);
                //qDebug() << temp;

                if (static_cast<unsigned char>(temp[0]) == 0xFF) {
                    ui->plainTextEdit->appendPlainText("Done!");
                    ui->labelLFN->setText("Erased! Ready to write flash");
                    StateMachine = State_Ready;

                    //Send a packet and the write to flash command here.
                    //We need to set up the configurable preamble too

                }
            }
            else if (algData.ALG_EraseMode == 1) { //sector erase, we'll have a lot more of these to do
                QByteArray temp = getBytes(cartType, CFIEBsize * EBcurrentSector);
                if (static_cast<unsigned char>(temp[0]) != 0x80) {
                    qDebug() << temp; 
                    return;
                }

                StateMachine = State_Bypass;
                ui->plainTextEdit->appendPlainText("Erase in progress...");
                ui->labelLFN->setText("Erase in progress. Please wait (up to 120seconds)");
                for (unsigned int i = 0; i < algData.ALG_EraseLen; i++) {
                    sendByte(cartType, CFIEBsize * EBcurrentSector, algData.EraseCommandDatas[i]);
                }
                EBcurrentSector++;
                StateMachine = State_Erase;

                if ((EBcurrentSector * CFIEBsize) > CFIFlashSize) {
                    ui->plainTextEdit->appendPlainText("Done!");
                    ui->labelLFN->setText("Erased! Ready to write flash");
                    StateMachine == State_Ready;

                }

            }
        }

        if (StateMachine==State_ROM_Wr){ //erase is finished, lets set up the flash ago then start writing
            //Send a packet and the write to flash command here.
            //We need to set up the configurable preamble too



        }


    }

}



void MainWindow::on_actionUpdate_Firmware_triggered()
{
    qDebug("Firmware update requested");
    StateMachine=State_FW_Wr;
    QString filePath = QFileDialog::getOpenFileName(this, "Open .JR File", QString(), "JR Files (*.jr)");
    if (!filePath.isEmpty()) {
        // Handle the selected file path, e.g., load and process the file
        qDebug() << "Selected file path: " << filePath;
        QByteArray dataToSend = "\xFE\x01\xFF"; //fe00 init's the routine
        serialPort.write(dataToSend);
        if (serialPort.waitForReadyRead(1000)) {
            // Reading response data
            QByteArray receivedData = serialPort.readAll();
            qDebug() << "Received data:" << receivedData;
        }
        const qint64 chunkSize = 64;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            while (!file.atEnd()) {
                QByteArray chunk = file.read(chunkSize);
                serialPort.write(chunk);
                if (!serialPort.waitForReadyRead(1000)) {
                    qWarning() << "Timeout waiting for response";
                    break;
                }
                // Read and process the response
                QByteArray responseData = serialPort.readAll();
                qDebug() << "Received response:" << responseData;
            }
            Init();
            ui->labelLFN->setVisible(true);
            ui->labelLFN->setText("Joey Disconnected... Searching");
            ui->labelStatus->setVisible(true);
            ui->labelStatus->setText("");
            ui->label_CFI->clear();
            StateMachine=State_Ready;
            connected=false;
            serialPort.close();
        }
    }
}


void MainWindow::on_pushButton_UploadSave_clicked()
{
    qDebug("Save Xfer requested");
    StateMachine=State_RAM_Wr;
    QString filePath = QFileDialog::getOpenFileName(this, "Open .SAV File", QString(), "Save File (*.SAV)");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error: Unable to open the file.";
        return;
    }
    ui->progressBar->setVisible(true);
    // Set the initial offset
    int offset = 0;
    // Buffer to store the packet
    QByteArray packet(42, 0);  // 2 fixed bytes (0x05, 0x01) + 4 bytes offset + 32 bytes data
    // Loop until the end of the file
    while (!file.atEnd()) {
        ui->progressBar->setValue(static_cast<int>((static_cast<double>(offset) / cartRamSize) * 100));
        packet[0] = 0x05;
        packet[1] = 0x01;
        // Set the 32-bit offset at position 2 in the byte array
        for (int i = 0; i < 4; ++i) {
            packet[2 + i] = static_cast<char>((offset >> (8 * i)) & 0xFF);
        }
        file.read(packet.data() + 10, 32);
        serialPort.write(packet);
        serialPort.waitForBytesWritten();
        offset += 32;
    }
    ui->progressBar->setVisible(false);
    file.close();
    StateMachine=State_Ready;
}


void MainWindow::on_pushButton_DownloadROM_clicked()
{
    //rom download function
    //we know the rom size, we know the long file name, open a new file with lfn suggested
    //ROM dump mode = 0x03, followed by mapper type, start offset, number of bytes to dump
    ui->progressBar->setVisible(true);
    offset=0;
    MBC=cartMapperType;
    length=cartRomSize;
    receivedData.clear();
    requestAndReceiveROM( MBC, offset);
    ui->labelGif->raise(); //put the image over the buttons so they're inaccessible.

}

void MainWindow::on_pushButton_DownloadSave_clicked()
{
    //rom download function
    //we know the rom size, we know the long file name, open a new file with lfn suggested
    //ROM dump mode = 0x03, followed by mapper type, start offset, number of bytes to dump
    ui->progressBar->setVisible(true);
    offset=0;
    MBC=cartMapperType;
    length=cartRomSize;
    receivedData.clear();
    requestAndReceiveRAM( MBC, offset);
    ui->labelGif->raise(); //put the image over the buttons so they're inaccessible.

}

void MainWindow::requestAndReceiveROM(uint8_t MBC, uint32_t offset)
{
    StateMachine=State_ROM_Rd;
    // Prepare the request data
    QByteArray requestData;
    requestData.append(static_cast<char>(0x03));
    if (cartType==GBC) requestData.append(static_cast<char>(MBC));
    else if (cartType==GBA) requestData.append(static_cast<char>(0x10));
    requestData.append(reinterpret_cast<const char*>(&offset), sizeof(offset));
    // Send the request
    serialPort.write(requestData);
}

void MainWindow::requestAndReceiveRAM(uint8_t MBC, uint32_t offset)
{
    StateMachine=State_RAM_Rd;    // Prepare the request data
    QByteArray requestData;
    requestData.append(static_cast<char>(0x04));
    requestData.append(static_cast<char>(MBC));
    requestData.append(reinterpret_cast<const char*>(&offset), sizeof(offset));
    // Send the request
    serialPort.write(requestData);
}


void MainWindow::handleSerialData() //function is called on every packet received. We'll check the header to see what to do with the packet
{
    if (StateMachine==State_Bypass)return;

    else if (StateMachine==State_RAM_Rd || StateMachine==State_ROM_Rd) {
        QString filePath;
        if (StateMachine==State_ROM_Rd) length=cartRomSize; else length=cartRamSize;
        receivedData += serialPort.readAll();
        offset+=64;
        ui->progressBar->setValue(static_cast<int>((static_cast<double>(offset) / length) * 100));
        if (offset >= length)
        {
            // Save the received data to a file
            if (StateMachine==State_ROM_Rd){
                if (cartType==GBC) filePath = QFileDialog::getSaveFileName(nullptr, "Save File", ui->labelLFN->text()+".GBC", "All Files (*)");
                else if (cartType==GBA) filePath = QFileDialog::getSaveFileName(nullptr, "Save File", ui->labelLFN->text()+".GBA", "All Files (*)");
            }
            else
            {
                filePath = QFileDialog::getSaveFileName(nullptr, "Save File", ui->labelLFN->text()+".SAV", "All Files (*)");
            }
            if (!filePath.isEmpty()) {
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(receivedData);
                    file.close();
                    qDebug() << "File saved successfully.";
                } else {
                    qDebug() << "Error saving file.";
                }
            } else {
                qDebug() << "File save canceled.";
            }
            offset = 0;
            length = 0;
            ui->progressBar->setVisible(false);
            StateMachine=State_Ready;
            ui->labelGif->lower(); //put the image uner the buttons so they're accessible.
        }
        else
        {
            // Request more data if the expected length is not reached
            if (StateMachine==State_ROM_Rd){
                requestAndReceiveROM(MBC, offset);
            }
            else if (StateMachine==State_RAM_Rd)
            {
                requestAndReceiveRAM(MBC, offset);
            }
        }
    }
    else if (StateMachine==State_ROM_Wr){
        if (cartType == GBC) {
            ui->progressBar->setVisible(true);
            if (ROMdataToFlashOffset < ROMdataToFlashFileSize) {
                ui->progressBar->setValue(static_cast<int>((static_cast<double>(ROMdataToFlashOffset) / ROMdataToFlashFileSize) * 100));
                if ((ROMdataToFlashOffset & 0x3FFF) == 0x00) { //new bank, do the bank thing
                    sendByte(cartType, 0x2000, (ROMdataToFlashOffset >> 14));
                    qDebug() << "Bank:" << ROMdataToFlashOffset;
                }
                QByteArray requestData = "";
                QByteArray extractedBytes = ROMdataToFlash.mid(ROMdataToFlashOffset, 32);
                StateMachine = State_ROM_Wr;
                unsigned ROMdataToFlashOffsetBanked = ROMdataToFlashOffset;
                if (ROMdataToFlashOffsetBanked >= 0x8000)ROMdataToFlashOffsetBanked = (ROMdataToFlashOffsetBanked & 0x7FFF) | 0x4000;
                requestData.append(static_cast<char>(0x06)); //write algo byte
                requestData.append(static_cast<char>(0x03)); //call the flash algo
                requestData.append(static_cast<char>(algData.ALG_PrgAlg)); //Algo Type
                requestData = AppendWord(requestData, byteWriteBufferSize); //number of bytes to write (max 4096)
                requestData = AppendWord(requestData, ROMdataToFlashOffsetBanked); //offset in the buffer to put it
                requestData.append(extractedBytes);
                ROMdataToFlashOffset += byteWriteBufferSize;
                serialPort.write(requestData);
               // qDebug() << requestData;
            }
            else
            {
                StateMachine = State_Ready; //all done!
                ui->progressBar->setVisible(false);
            }

        }
        else if (cartType == GBA) {

            ui->progressBar->setVisible(true);
            if (ROMdataToFlashOffset < ROMdataToFlashFileSize) {
                ui->progressBar->setValue(static_cast<int>((static_cast<double>(ROMdataToFlashOffset) / ROMdataToFlashFileSize) * 100));
                QByteArray requestData = "";
                QByteArray extractedBytes = ROMdataToFlash.mid(ROMdataToFlashOffset, 32);
                StateMachine = State_ROM_Wr;
                requestData.append(static_cast<char>(0x06)); //write algo byte
                requestData.append(static_cast<char>(0x03)); //call the flash algo
                requestData.append(static_cast<char>(algData.ALG_PrgAlg)); //Algo Type
                requestData = AppendWord(requestData, byteWriteBufferSize); //number of bytes to write (max 4096)
                requestData = AppendWord(requestData, ROMdataToFlashOffset); //offset in the buffer to put it
                requestData.append(extractedBytes);
                ROMdataToFlashOffset += byteWriteBufferSize;
                serialPort.write(requestData);
                //qDebug() << requestData;
            }
            else
            {
                StateMachine = State_Ready; //all done!
                ui->progressBar->setVisible(false);
            }


        }
    }
}


QMap<QString, QString> MainWindow::loadCSV(const QString& filename) {
    QMap<QString, QString> data;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed(); // Trim leading and trailing whitespaces
            QStringList parts = line.split("\t"); // Use \t as the delimiter
            if (parts.length() == 2) {
                data[parts[0]] = parts[1];
            } else {
                qWarning() << "Invalid line format:" << line;
            }
        }
        file.close();
    } else {
        qWarning() << "Could not open file:" << file.errorString();
    }
    return data;
}
QString MainWindow::findAsciiString(const QMap<QString, QString>& data, const QString& hexString) {
    return data.value(hexString, "NotFound");
}

void MainWindow::on_actionAbout_triggered()
{


    // Create a main window
    QDialog aboutWindow;
    aboutWindow.setWindowTitle("About QT");

    // Create a layout for the main window
    QVBoxLayout *layout = new QVBoxLayout(&aboutWindow);

    // Create a label with information about Qt's license
    QLabel *infoLabel = new QLabel(
        "This application is built using the Qt 6.6.1 framework.\n"
        "Qt is available under the LGPL version 3 license.\n"
        "For more information, visit: https://www.qt.io/\n\n"

        "For JoeyGUI support, visit the JoeyGUI channel in our discord server\n"

    );

    // Create an "About" button to show the license details
    QPushButton *aboutButton = new QPushButton("About Qt");
    QObject::connect(aboutButton, &QPushButton::clicked, [&]() {
        QMessageBox::about(
            &aboutWindow,
            "About Qt",
            "This program uses Qt version 6.6.1.\n"
            
            "Qt is a C++ toolkit for cross-platform application development.\n"
            
            "Qt provides single-source portability across all major desktop operating systems. "
            "It is also available for embedded Linux and other embedded and mobile operating systems.\n"

            "Qt is available under multiple licensing options designed to accomodate the needs of our various users.\n"
 
            "Qt licensed under our commercial license agreement is appropreate for development of proprietary/commercial software where you do not want to share any source code with thrid parties or otherwise cannot comply wiht the terms of GNU (L)GPL.\n"

            "Qt licensed under GNU (L)GPL is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the respective licenses.\n"

            "Please see qt.io/licensing for an overview of Qt licensing..\n"

            "Copyright (C) 2023 The Qt Company Ltd and other contributors.\n"

            "Qt and the Qt logo are trademarks of The Qt Company Ltd.\n"

            "Qt is The Qt Company Ltd product developed as an open source project. See qt.io for more information.\n"

            "Version: " QT_VERSION_STR "\n"
            "License: LGPL version 3\n"
            "For more information, visit: https://www.qt.io/"
            );
    });

    // Add the label and button to the layout
    layout->addWidget(infoLabel);
    layout->addWidget(aboutButton);

    // Set the layout for the main window
    aboutWindow.setLayout(layout);

    // Show the main window
    aboutWindow.show();
    aboutWindow.exec();

    // Clean up the memory when the second window is closed


}


void MainWindow::on_actionAbout_QT_triggered()
{
    // Create a main window
    QDialog aboutWindow;
    aboutWindow.setWindowTitle("About QT");

    // Create a layout for the main window
    QVBoxLayout *layout = new QVBoxLayout(&aboutWindow);

    // Create a label with information about Qt's license
    QLabel *infoLabel = new QLabel(
        "This application is using the Qt framework.\n"
        "Qt is available under the LGPL version 3 license.\n"
        "For more information, visit: https://www.qt.io/"
        );

    // Create an "About" button to show the license details
    QPushButton* aboutButton = new QPushButton("About Qt");
    QObject::connect(aboutButton, &QPushButton::clicked, [&]() {
        QMessageBox::about(
            &aboutWindow,
            "About Qt",
            "This program uses Qt version 6.6.1.\n"

            "Qt is a C++ toolkit for cross-platform application development.\n"

            "Qt provides single-source portability across all major desktop operating systems. "
            "It is also available for embedded Linux and other embedded and mobile operating systems.\n"

            "Qt is available under multiple licensing options designed to accomodate the needs of our various users.\n"

            "Qt licensed under our commercial license agreement is appropreate for development of proprietary/commercial software where you do not want to share any source code with thrid parties or otherwise cannot comply wiht the terms of GNU (L)GPL.\n"

            "Qt licensed under GNU (L)GPL is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the respective licenses.\n"

            "Please see qt.io/licensing for an overview of Qt licensing..\n"

            "Copyright (C) 2023 The Qt Company Ltd and other contributors.\n"

            "Qt and the Qt logo are trademarks of The Qt Company Ltd.\n"

            "Qt is The Qt Company Ltd product developed as an open source project. See qt.io for more information.\n"

            "Version: " QT_VERSION_STR "\n"
            "License: LGPL version 3\n"
            "For more information, visit: https://www.qt.io/"
        );
        });

    // Add the label and button to the layout
    layout->addWidget(infoLabel);
    layout->addWidget(aboutButton);

    // Set the layout for the main window
    aboutWindow.setLayout(layout);

    // Show the main window
    aboutWindow.show();
    aboutWindow.exec();

    // Clean up the memory when the second window is closed


}

void MainWindow::on_pushButton_Detect_clicked()
{





}

QByteArray MainWindow::AppendWord(QByteArray datastring, unsigned int value){

    datastring.append(static_cast<char>((value) & 0xFF));
    datastring.append(static_cast<char>((value >> 8) & 0xFF));
    datastring.append(static_cast<char>((value >> 16) & 0xFF));
    datastring.append(static_cast<char>((value >> 24) & 0xFF));
    return datastring;
}

unsigned int MainWindow::isCFIpresent(){

    unsigned int temp;

    temp=QRY(0x0AAA,0x9898);if (temp!=0) return temp;
    temp=QRY(0x0555,0x9898);if (temp!=0) return temp;
    temp=QRY(0x4AAA,0x9898);if (temp!=0) return temp;
    temp=QRY(0x4555,0x9898);if (temp!=0) return temp;


    return 0;
}

void MainWindow::InterrogateCFI(){
    QByteArray temp;
    sendByte(cartType,CFIaddress,CFIdata);
    ui->plainTextEdit->appendPlainText("CFI Interrogation:");

    temp = getBytes(cartType,0x40);
    if (CFIbitsSwapped==true){temp=swapBits(temp);}
    //qDebug()<<temp;
    CFIFlashSize=(1U << temp[0x0E]);
    ui->plainTextEdit->appendPlainText(QString("Flash Capcity: 0x%1 (%2 bytes)").arg(CFIFlashSize, 0, 16).toUpper() .arg(CFIFlashSize));
    CFIBufferSize=(1U << temp[0x14]);
    ui->plainTextEdit->appendPlainText(QString("Buffer Size: 0x%1 (%2 bytes)").arg(CFIBufferSize, 0, 16).toUpper() .arg(CFIBufferSize));
    CFIEBregions=( temp[0x18]);
    ui->plainTextEdit->appendPlainText(QString("Erase Block Regions: %1").arg(CFIEBregions));
    CFIEBsize = ((temp[0x1E]) | ((temp[0x20]) << 8) * 256);
    ui->plainTextEdit->appendPlainText(QString("Erase Block Size: %1").arg(CFIEBsize));


    sendByte(cartType,0,0xFF);
    sendByte(cartType,0,0xF0);
}


unsigned char MainWindow::QRY( unsigned int Address, unsigned int Data){

    QByteArray temp;
    sendByte(cartType,Address,Data);
    temp = getBytes(cartType,0);
    if (temp[0x20]=='Q'){
        if (temp[0x22]=='R'){
            if (temp[0x24]=='Y'){
                sendByte(cartType,0,0xFF);
                sendByte(cartType,0,0xF0);
                ui->plainTextEdit->appendPlainText(QString("Address: 0x%1, Data: 0x%2").arg(Address, 0, 16).toUpper() .arg(Data, 0, 16).toUpper());
                CFIbitsSwapped=false;
                CFIaddress=Address;
                CFIdata=Data;
                return 1;
            }
        }
    }
    temp=swapBits(temp);
    if (temp[0x20]=='Q'){
        if (temp[0x22]=='R'){
            if (temp[0x24]=='Y'){
                sendByte(cartType,0,0xFF);
                sendByte(cartType,0,0xF0);
                ui->plainTextEdit->appendPlainText(QString("Bitswapped, Address: 0x%1, Data: 0x%2").arg(Address, 0, 16).toUpper() .arg(Data, 0, 16).toUpper());
                CFIaddress=Address;
                CFIdata=Data;
                CFIbitsSwapped=true;
                return 2;
            }
        }
    }
    sendByte(cartType,0,0xFF);
    sendByte(cartType,0,0xF0);
    return 0;
}

QByteArray MainWindow::swapBits(QByteArray temp){

    QByteArray resultArray;
    resultArray.resize(temp.size());

    for (int i = 0; i < temp.size(); ++i) {
        char originalByte = temp.at(i);
        char modifiedByte = ((originalByte & 0x01) << 1) | ((originalByte & 0x02) >> 1) | (originalByte & 0xFC);
        resultArray[i] = modifiedByte;
    }

    return resultArray;
}

void MainWindow::sendByte(char Mode, unsigned int Address, unsigned int Data)
{
    int tempstate=StateMachine;
    StateMachine=State_Bypass;
    // Prepare the request data
    QByteArray requestData;
    requestData.append(static_cast<char>(0x0A));
    requestData.append(static_cast<char>(Mode));
    requestData.append(reinterpret_cast<const char*>(&Address), sizeof(Address));
    requestData.append(reinterpret_cast<const char*>(&Data), sizeof(Data));
    serialPort.write(requestData);

    if (serialPort.waitForReadyRead(1000)) {
        // Reading response data
        QByteArray temp = serialPort.readAll();
    }
    StateMachine=tempstate;

}
QByteArray MainWindow::getBytes(char Mode, unsigned int Address)
{
    int tempstate=StateMachine;
    StateMachine=State_Bypass;
    QByteArray temp;
    // Prepare the request data
    QByteArray requestData;
    requestData.append(static_cast<char>(0x0B));
    requestData.append(static_cast<char>(Mode));
    requestData.append(reinterpret_cast<const char*>(&Address), sizeof(Address));
    serialPort.write(requestData);

    if (serialPort.waitForReadyRead(1000)) {
        // Reading response data
        temp = serialPort.readAll();
    }
    StateMachine=tempstate;
    return temp;
}

void MainWindow::on_pushButton_UploadROM_clicked()
{
    QMessageBox msgBox;
    StateMachine=State_Bypass;
    if (ui->comboBox_FlashCartType->currentText()=="Select Flash Cart Type"){
        msgBox.setWindowTitle("Error!");
        msgBox.setText("Please select a Flash Cart in the drop down box!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else{
        QString filePath ;
        if (cartType==GBC) filePath = QFileDialog::getOpenFileName(this, "Open ROM File", QString(), "ROMS (*.GBC *.GB)"); else  filePath = QFileDialog::getOpenFileName(this, "Open ROM File", QString(), "ROMS (*.GBA)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                QFileInfo fileInfo(filePath);
                QString justFileName = fileInfo.fileName();
                ui->labelLFN->setText(justFileName);
                QTextStream in(&file);
                // Read the file content and store it in the global variable
                ROMdataToFlash = file.readAll();
                ROMdataToFlashFileSize = file.size();

                if ( ROMdataToFlashFileSize>CFIFlashSize) {
                    msgBox.setWindowTitle("Error!");
                    msgBox.setText("ROM is too large for the reported flash capacity");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    file.close();
                    BypassRxHandler=false;
                    return;
                }
                file.close();
                //send algo deets
                StateMachine=State_Bypass;


                QByteArray requestData;
                requestData.append(static_cast<char>(0x0C)); //Set cart voltage
                requestData.append(static_cast<char>(algData.ALG_Voltage));
                serialPort.write(requestData);
                if (serialPort.waitForReadyRead(10)) {
                    // Reading response data
                    serialPort.readAll();
                }

                requestData="";
                requestData.append(static_cast<char>(0x06)); //write algo byte
                requestData.append(static_cast<char>(0x00));
                for (unsigned int i=0;i<algData.ALG_PrgLen;i++){ //send the flash ago data
                    requestData=AppendWord(requestData,algData.ProgramCommandAddresses[i]);
                    requestData=AppendWord(requestData,algData.ProgramCommandDatas[i]);
                }
                //qDebug() << requestData;
                StateMachine=State_ROM_Wr;
                //zero the file pointer
                ROMdataToFlashOffset=0;
                serialPort.write(requestData);
                requestData="";
                //now we need to fill the buffer and write the sector. we'll leave it up to the rx handler to do this

            }
        }
    }
}





void MainWindow::on_comboBox_FlashCartType_currentIndexChanged(int index)
{
    QString selectedText = ui->comboBox_FlashCartType->currentText();
    QString fullPath = (QCoreApplication::applicationDirPath() + "/FlashCartAlgorithms/" + selectedText + ".ALG");


    if (readALGFile(fullPath, algData)) {
        qDebug() << "ALG Mode:" << algData.ALG_Mode;
        qDebug() << "ALG Buffer Size:" << algData.ALG_BufferSize;
        qDebug() << "ALG Erase Mode:" << algData.ALG_EraseMode;
        qDebug() << "ALG Erase Length:" << algData.ALG_EraseLen;
        qDebug() << "ALG Program Algorithm:" << algData.ALG_PrgAlg;
        qDebug() << "ALG Program Length:" << algData.ALG_PrgLen;
        qDebug() << "ALG Voltage:" << algData.ALG_Voltage;

        // Print Erase Commands
        qDebug() << "Erase Commands:";
        for (unsigned int i = 0; i < algData.ALG_EraseLen; ++i) {
            qDebug() << "Address:" << algData.EraseCommandAddresses[i] << "Data:" << algData.EraseCommandDatas[i];
        }

        // Print Program Commands
        qDebug() << "Program Commands:";
        for (unsigned int i = 0; i < algData.ALG_PrgLen; ++i) {
            qDebug() << "Address:" << algData.ProgramCommandAddresses[i] << "Data:" << algData.ProgramCommandDatas[i];
        }
    }

}

bool MainWindow::readALGFile(const QString& filePath, ALGData& algData) {
    int ECcount=0;
    int PCcount=0;
    int numOfEraseEntries;
    int numOfProgEntries;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file" << filePath;
        return false;
    }

    QTextStream in(&file);
    QVector<unsigned int> eraseAddresses;
    QVector<unsigned int> eraseDatas;
    QVector<unsigned int> programAddresses;
    QVector<unsigned int> programDatas;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        qDebug() << "Line:" << line;

        QStringList parts = line.split("=");

        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();

            if (key == "Mode") {
                algData.ALG_Mode = value.toUInt(nullptr, 16);
            } else if (key == "Buffer Size") {
                algData.ALG_BufferSize = value.toUInt(nullptr, 16);
            } else if (key == "Erase mode") {
                algData.ALG_EraseMode = value.toUInt(nullptr, 16);
            } else if (key == "Erase Command Length") {
                algData.ALG_EraseLen = value.toUInt(nullptr, 16);
                numOfEraseEntries=algData.ALG_EraseLen;
            }
            else if (key == "Program Algorithm") {
                algData.ALG_PrgAlg = value.toUInt(nullptr, 16);
            }
            else if (key == "Voltage") {
                algData.ALG_Voltage = value.toUInt(nullptr, 16);
            }
            else if (key == "Program Command Length") {
                algData.ALG_PrgLen = value.toUInt(nullptr, 16);
                numOfProgEntries=algData.ALG_PrgLen;
            }
        }





        // Check if the line contains "0x" for Erase or Program commands
        if (line.contains("0x")) {
            QStringList hexValues = line.split(",");
            QVector<unsigned int> intValues;

            for (const QString& hexValue : hexValues) {
                unsigned int intValue = hexValue.trimmed().toUInt(nullptr, 16);
                intValues.append(intValue);
                //               qDebug() << "Hex Value:" << hexValue.trimmed() << "Integer Value:" << intValue;
            }
            //qDebug() << "Hex Data Line:" << line << "Integer Values:" << intValues;

            if (numOfEraseEntries > 0) {
                numOfEraseEntries--;

                // Check if the line is not empty before accessing intValues
                if (!intValues.isEmpty()) {
                    algData.EraseCommandAddresses[ECcount] = intValues.at(0);
                    algData.EraseCommandDatas[ECcount] = intValues.at(1);
                    //   qDebug() << intValues.at(0);
                    //   qDebug() << intValues.at(1);
                    //    qDebug() << "Address:" << algData.EraseCommandAddresses[ECcount] << "Data:" << algData.EraseCommandDatas[ECcount];

                    ECcount++;
                }
            } else if (numOfProgEntries > 0) {
                numOfProgEntries--;

                // Check if the line is not empty before accessing intValues
                if (!intValues.isEmpty()) {
                    algData.ProgramCommandAddresses[PCcount] = intValues.at(0);
                    algData.ProgramCommandDatas[PCcount] = intValues.at(1);
             //       qDebug() << intValues.at(0);
             //       qDebug() << intValues.at(1);
             //       qDebug() << "Address:" << algData.EraseCommandAddresses[PCcount] << "Data:" << algData.EraseCommandDatas[PCcount];
                    PCcount++;
                }
            }
        }
    }
    file.close();
    //qDebug() << "Address:" << algData.ProgramCommandAddresses[0] << "Data:" << algData.ProgramCommandDatas[0];



    return true;
}

void MainWindow::on_pushButton_clicked() //erase button
{
    QMessageBox msgBox;
    if (ui->comboBox_FlashCartType->currentText() == "Select Flash Cart Type") {
        msgBox.setWindowTitle("Error!");
        msgBox.setText("Please select a Flash Cart in the drop down box!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else {
        
        StateMachine = State_Bypass;
        QByteArray requestData;
        requestData.append(static_cast<char>(0x0C)); //Set cart voltage
        requestData.append(static_cast<char>(algData.ALG_Voltage));
        serialPort.write(requestData);
        if (serialPort.waitForReadyRead(10)) {
            // Reading response data
            serialPort.readAll();
        }

        if (algData.ALG_EraseMode == 0) {
            StateMachine = State_Bypass;
            ui->plainTextEdit->appendPlainText("Erase in progress...");
            ui->labelLFN->setText("Erase in progress. Please wait (up to 120seconds)");
            for (unsigned int i = 0; i < algData.ALG_EraseLen; i++) {
                sendByte(cartType, algData.EraseCommandAddresses[i], algData.EraseCommandDatas[i]);
                qDebug() << "Add:" << QString::number(algData.EraseCommandAddresses[i], 16) << "Data:" << QString::number(algData.EraseCommandDatas[i], 16);

            }
            StateMachine = State_Erase;
        }
        else if (algData.ALG_EraseMode == 1) { //FCE not supported, erase via sectors

            StateMachine = State_Bypass;
            ui->plainTextEdit->appendPlainText("Erase in progress...");
            ui->labelLFN->setText("Erase in progress. Please wait (up to 120seconds)");
            for (unsigned int i = 0; i < algData.ALG_EraseLen; i++) {
                sendByte(cartType, algData.EraseCommandAddresses[i], algData.EraseCommandDatas[i]);
                qDebug() << algData.EraseCommandAddresses[i] << algData.EraseCommandDatas[i];
            }
            EBcurrentSector++;
            StateMachine = State_Erase;

        }
    }

}
