#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void update();

    void on_ironButton_released();
    void on_amberButton_released();

    void on_pushButton_released();

    void on_pushButton_2_released();

    void on_maxButton_released();

    void on_centralButton_released();

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QLabel *tempLabel;
    QPushButton *m_button;
};

#endif // MAINWINDOW_H
