#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QMessageBox>

#include "seekware.h"
#include <iostream>

#include <QTimer>

#define NUM_CAMS 9
using namespace std;
psw pl[NUM_CAMS];

//sudo chmod a+w /dev/bus/usb/001/011

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

union imgdata image_f[156][208];
QImage test()
{
    QImage myImage = QImage(206, 156, QImage::Format_ARGB32_Premultiplied);
//    QImage myImage = QImage(206, 156, QImage::Format_RGBA8888_Premultiplied);
//    QImage myImage = QImage(206, 156, QImage::Format_RGB32);

    psw dev=pl[0];

    const union imgdata * pixel = image_f[0];
    int x_offset = 0;
    int y_offset = 0;
    int x, y;

    if (Seekware_GetImage(dev, NULL,NULL, &image_f[0][0].raw) != SW_RETCODE_NONE){
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
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
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

    psw dev=pl[0];

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
