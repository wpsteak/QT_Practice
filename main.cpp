#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <thread>
#include "seekware.h"
//#include <pthread.h>
#include </usr/include/libusb-1.0/libusb.h>
#define NUM_CAMS 9
using namespace std;

int main(int argc, char *argv[])
{
    cout << "aaa" << endl;

//    psw *pswlist[10];
//    int length;
    int numfound = 0;
    psw pl[NUM_CAMS];
    sw_retcode status;
    status = Seekware_Find(pl, NUM_CAMS, &numfound);
//    printf("oooo %d\n", numfound);
    cout << numfound << endl;
//    if (numfound > 0) {
//      //Seekware_Close(pswlist[0]);
//    printf("I am going to open device la\n");
//      Seekware_Open(pswlist[0]);

//    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

