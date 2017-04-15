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

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QPushButton *m_button;
};

#endif // MAINWINDOW_H
