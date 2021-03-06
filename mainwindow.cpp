#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"

#include "seekware.h"
#include <iostream>

#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QAction>
#include <QFile>
#include <QString>
#include <QDateTime>

#define NUM_CAMS 9
#define NUM_COLS 206
#define NUM_ROWS 156
using namespace std;
psw pl[NUM_CAMS];
psw dev;
bool central;

//sudo chmod a+w /dev/bus/usb/001/004

/// Pixel format in the RGB image buffer
union imgdata {
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
    uint32_t raw;
} __attribute__((packed));

struct TempInfo {
    QPoint tempLoc;
    float temp;
};

struct PoleTemperatures {
    TempInfo maxInfo;
    TempInfo minInfo;
};

union imgdata image_rgb[156][206];
float image_f[156][206];


PoleTemperatures findPoleTemps(){
    float maxTemp = image_f[0][0];
    float minTemp = image_f[0][0];
    QPoint maxPoint = QPoint(0,0);
    QPoint minPoint = QPoint(0,0);

    for(int i = 0; i< NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            if(image_f[i][j] > maxTemp) {
                maxTemp = image_f[i][j];
                maxPoint = QPoint(j,i);
            }
            if(image_f[i][j] < minTemp) {
                minTemp = image_f[i][j];
                minPoint = QPoint(j,i);
            }
        }
    }

    struct TempInfo maxInfo;
    maxInfo.tempLoc = maxPoint;
    maxInfo.temp = maxTemp;

    struct TempInfo minInfo;
    minInfo.tempLoc = minPoint;
    minInfo.temp = minTemp;

    struct PoleTemperatures poleTempInfos;
    poleTempInfos.maxInfo = maxInfo;
    poleTempInfos.minInfo = minInfo;

    return poleTempInfos;
}


QImage test()
{
    QImage myImage = QImage(206, 156, QImage::Format_ARGB32_Premultiplied);
//    QImage myImage = QImage(206, 156, QImage::Format_RGBA8888_Premultiplied);
//    QImage myImage = QImage(206, 156, QImage::Format_RGB32);

    psw dev=pl[0];

    const union imgdata * pixel = image_rgb[0];
    int x_offset = 0;
    int y_offset = 0;
    int x, y;

    if (Seekware_GetImage(dev, NULL, &image_f[0][0], &image_rgb[0][0].raw) != SW_RETCODE_NONE){
        fprintf(stderr, "Get Image error!\n");
        //return -1;
    }

    for (y=0; y<156; ++y) {
        int dy = y + y_offset;
        for (x=0; x<206; ++x, ++pixel) {
            int dx = x + x_offset;
            union imgdata * p = (union imgdata *)pixel;
            //printf("%d %d %d %d\n", p->r, p->g, p->b, p->a);
//            myImage.setPixel(dx,dy,qRgb(p->r,p->g,p->b));
            myImage.setPixel(dx,dy,qRgba(p->r,p->g,p->b,p->a));
        }
    }

    return myImage;
}

void MainWindow::update()
{
    QImage image = test();

    if(central){

        ui->tempLabel->setText(QString::number(image_f[78][103]));
        ui->tempLabel->setGeometry(QRect(QPoint(103,78),QSize(68,21)));
    }
    else {
        struct PoleTemperatures poleTempInfos = findPoleTemps();
        struct TempInfo maxTempInfo = poleTempInfos.maxInfo;
        struct TempInfo minTempInfo = poleTempInfos.minInfo;

        ui->tempLabel->setText(QString::number(maxTempInfo.temp));
        ui->tempLabel->setGeometry(QRect(maxTempInfo.tempLoc,QSize(68,21)));

        ui->minTempLabel->setText(QString::number(minTempInfo.temp));
        ui->minTempLabel->setGeometry(QRect(minTempInfo.tempLoc,QSize(68,21)));

        ui->monitorLabel->setText(QString::number(maxTempInfo.temp));
    }

    //fprintf(stderr, "%d (%d,%d)\n", info.temp,info.tempLoc.x(),info.tempLoc.y());
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));


    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString(QLatin1String("yyyyMMdd-hhmmsszzz"));
    //QString filename = QString::fromLatin1("image-%1.png").arg(timestamp);
    QString filename = "aaa.png";
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
//    QPixmap::fromImage(image).save(&file, "PNG");

//    QPixmap pixmap = QPixmap::grabWidget(ui->displayWidget); //deprecated, use QWidget::grab() instead
    QPixmap pixmap = ui->displayWidget->grab();
    pixmap.save(&file, "PNG");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    central = false;

    ui->setupUi(this);

    createMenus();

    sw_retcode status;
    int numfound = 0;
    status = Seekware_Find(pl, NUM_CAMS, &numfound);
    cout << numfound << endl;

    dev=pl[0];

    printf("Seekware_Open\n");
    status = Seekware_Open(dev);
    if (SW_RETCODE_NONE != status) {
       fprintf(stderr, "Could not open PIR Device (%d)\n", status);
    }
    else {
            //printf("%d %d\n", dev->frame_cols, dev->frame_rows);
        Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_IRON);

        QTimer *timer = new QTimer(this);
        timer->setInterval(50);

        connect(timer, SIGNAL(timeout()), this, SLOT(update()));

        timer->start();
    }
}


void MainWindow::optionsWindow() {
    QWidget *widget = new OptionsDialog;
    widget->show();

}

void MainWindow::createMenus() {
    QMenu *toolsMenu = menuBar()->addMenu("Tools");
    QAction *configAction = new QAction(tr("Configuration"),this);
    QAction *optionsAction = new QAction(tr("Options"),this);
    connect(optionsAction, &QAction::triggered, this, &MainWindow::optionsWindow);
    toolsMenu->addAction(configAction);
    toolsMenu->addAction(optionsAction);

//    QAction *exitAction = new QAction(tr("Exit"),this);
    //connect(exitAction, SIGNAL(trigg))
//    QMenu *exitMenu = menuBar()->addMenu("Exit");
    //exitMenu->addAction()

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_amberButton_released()
{
    /*
        SW_LUT_WHITE,
        SW_LUT_BLACK,
        SW_LUT_IRON,
        SW_LUT_COOL,
        SW_LUT_AMBER,
        SW_LUT_INDIGO,
        SW_LUT_TYRIAN,
        SW_LUT_GLORY,
        SW_LUT_ENVY
    */
    Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_AMBER);
}

void MainWindow::on_ironButton_released()
{
    Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_IRON);
}

void MainWindow::on_pushButton_released()
{
    Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_COOL);
}

void MainWindow::on_pushButton_2_released()
{
    Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_WHITE);
}

void MainWindow::on_maxButton_released()
{
    central = false;
}

void MainWindow::on_centralButton_released()
{
    central = true;
}
