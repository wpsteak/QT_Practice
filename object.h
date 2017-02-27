#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>

class Object : public QObject
{
    Q_OBJECT
public:
    explicit Object(QObject *parent = 0);

signals:

public slots:
    //on_button_clicked();
};

#endif // OBJECT_H
