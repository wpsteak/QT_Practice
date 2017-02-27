#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <thread>
#include "seekware.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QThread>

//#define NUM_CAMS 9
//using namespace std;
//psw pl[NUM_CAMS];

//sudo chmod a+w /dev/bus/usb/001/011

//void test()
//{
//    cout << "bbb" << endl;
//    int numfound = 0;

//    sw_retcode status;
//    status = Seekware_Find(pl, NUM_CAMS, &numfound);
//    cout << numfound << endl;

//    psw dev=pl[0];
//    printf("Seekware_Open\n");
//    status = Seekware_Open(dev);
//    if (SW_RETCODE_NONE != status) {
//       fprintf(stderr, "Could not open PIR Device (%d)\n", status);
//    }
//}

int main(int argc, char *argv[])
{
//    cout << "aaa" << endl;
//    QFuture<void> ta = QtConcurrent::run(test);
//    ta.waitForFinished();
//    cout << "ccc" << endl;


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

