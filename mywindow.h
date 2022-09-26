#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <cmath>
#include <vector>
#include "szescian.h"
#include <queue>

namespace Ui {
    class MyWindow;
}

class MyWindow : public QMainWindow
{

    Q_OBJECT

public:

    explicit MyWindow(QWidget *parent = 0);

    ~MyWindow();

private:

    Ui::MyWindow *ui;

    QImage *img;
    Szescian szescian = Szescian();

    int szer;
    int wys;
    int poczX;
    int poczY;

    int v[15] = {0,0,0,100,100,100,0,0,0,0,0,0,0,0,0};
    bool textured = false;
    int szerokosc = 600, wysokosc = 600;
    int guiWidth = 300, guiHeight = wysokosc;
    int d = 25;

    int getPixel(int x, int y);
    int getPixel(Point p);
    void putPixel(int x, int y, int r = 0, int c = 0);
    void putPixel(Point p, int r = 2, int c = 0);
    void line(int x0, int y0, int x1, int y1);
    void line(Point A, Point B);
    void getAxis(queue <Point> &que, Point A, Point B);
    void texture(Sciana sciana, Operacje M);
private slots:

    void paintEvent(QPaintEvent*);
    //void reset();
    void draw();
    void clear();
    void setTextured();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_horizontalSlider_7_valueChanged(int value);

    void on_horizontalSlider_8_valueChanged(int value);
    void on_horizontalSlider_9_valueChanged(int value);
    void on_horizontalSlider_10_valueChanged(int value);
    void on_horizontalSlider_11_valueChanged(int value);
    void on_horizontalSlider_12_valueChanged(int value);
    void on_horizontalSlider_13_valueChanged(int value);
    void on_horizontalSlider_14_valueChanged(int value);
    void on_horizontalSlider_15_valueChanged(int value);
    void on_pushButton_clicked();
};

#endif
