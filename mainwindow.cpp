#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QMessageBox>

#include "seekware.h"
#include <iostream>

#include <QTimer>

#define NUM_CAMS 9
#define NUM_COLS 208
#define NUM_ROWS 156
using namespace std;
psw pl[NUM_CAMS];
psw dev;

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

struct TempLocation{
    QPoint tempLoc;
    uint8_t temp;
};

union imgdata image_rgb[156][208];
float image_f[156][208];


TempLocation findMax(){
    float maxTemp = image_f[0][0];
    QPoint maxPoint = QPoint(0,0);

    for(int i = 0; i< NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            if(image_f[i][j] > maxTemp) {
                maxTemp = image_f[i][j];
                maxPoint = QPoint(i,j);
            }
        }
    }

    struct TempLocation info;
    info.tempLoc = maxPoint;
    info.temp = maxTemp;

    return info;
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
    struct TempLocation info = findMax();
    fprintf(stderr, "%d (%d,%d)\n", info.temp,info.tempLoc.x(),info.tempLoc.y());

    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    //float src = image_f[78][104];
    //fprintf(stderr, "%f\n", src);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
        Seekware_SetSetting(dev, SETTING_ACTIVE_LUT, SW_LUT_IRON);

        QTimer *timer = new QTimer(this);
        timer->setInterval(50);

        connect(timer, SIGNAL(timeout()), this, SLOT(update()));

        timer->start();
    }
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

