#include "mywindow.h"
#include "ui_mywindow.h"

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();

    img = new QImage(szer,wys,QImage::Format_RGB32);
    draw();
    update();
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::paintEvent(QPaintEvent*)
{

    QPainter p(this);

    p.drawImage(poczX,poczY,*img);
}



void MyWindow::clear()
{
    uchar *ptr = img->bits();

    for (int i = 0; i < szerokosc; ++i)
    {
        for (int j = 0; j < wysokosc; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                ptr[4*(i*szerokosc+j)+k] = 255;
            }
        }
    }
}


int MyWindow::getPixel(int x, int y) {
    uchar *ptr = img->bits();

    if (x < 0 || x >= szerokosc || y < 0 || y >= wysokosc) return -1;
    return (ptr[4*(szerokosc*y+x)+2] << 16) + (ptr[4*(szerokosc*y+x)+1] << 8) + (ptr[4*(szerokosc*y+x)+0]);
}

int MyWindow::getPixel(Point P) {
    return getPixel(P.x() + szerokosc/2, P.y() + wysokosc/2);
}

void MyWindow::putPixel(int x, int y, int r, int c)
{
    uchar *ptr = img->bits();

    for (int i = max(0,x-r); i <= min(szerokosc-1, x+r); ++i) {
        for (int j = max(0,y-r); j <= min(wysokosc-1, y+r); ++j) {
            if (getPixel(i, j) == BG) {
                ptr[4*(j*szerokosc+i)+0] = c % 256;
                ptr[4*(j*szerokosc+i)+1] = c / 256 % 256;
                ptr[4*(j*szerokosc+i)+2] = c / 256 / 256;
            }
        }
    };
}

void MyWindow::putPixel(Point p, int r, int c)
{
    putPixel(p.v[0]+szerokosc/2, p.v[1]+wysokosc/2, r, c);
}

void MyWindow::line(int x0, int y0, int x1, int y1)
{
    int dx = x1-x0;
    int dy = y1-y0;
    bool swaped = false;
    int error = 0;

    if (abs(dy) > abs(dx))
    {
        swap(x0, y0);
        swap(x1, y1);
        swap(dx, dy);
        swaped = true;
    }

    if ( x0 > x1 )
    {
        swap( x0, x1 );
        swap( y0, y1 );
        dx = -dx;
        dy = -dy;
    }


    for ( int i = x0; i <= x1; ++i )
    {
        if ( 2*error >= dx )
        {
            error -= dx;
            if (dy >= 0 ) y0++;
            else y0--;
        }
        error += abs(dy);
        if (!swaped ) putPixel(i, y0, 1);
        else putPixel(y0, i, 1);
    }

    update();
}

void MyWindow::line(Point A, Point B)
{
    line(A.v[0]+szerokosc/2, A.v[1]+wysokosc/2, B.v[0]+szerokosc/2, B.v[1]+wysokosc/2);
}

void MyWindow::draw()
{
    clear();

    Operacje M = Operacje(1);

    M = M * Operacje::projection();
    M = M * Operacje::rotate(v[12], v[13], v[14]);
    M = M * Operacje::scale(v[3], v[4], v[5]);
    M = M * Operacje::shearing(v[6], v[7], v[8], v[9], v[10], v[11]);
    M = M * Operacje::translation(v[0], v[1], v[2]);
    szescian.reset(M);

    sort(szescian.sciana.begin(), szescian.sciana.end());
    for (int i = 0; i < 6; ++i)
    {
        texture(szescian.sciana[i], M);
    }
    update();
}

void MyWindow::texture(Sciana sciana, Operacje M) {
    if (sciana.color == -1) return;

    Point A = M * (*sciana.A);
    Point B = M * (*sciana.B);
    Point C = M * (*sciana.C);
    Point D = M * (*sciana.D);

    line(A, B);
    line(B, C);
    line(C, D);
    line(D, A);

    if (textured) {
        queue <Point> que;
        getAxis(que, A, C);
        getAxis(que, B, D);

        while (!que.empty()) {
            Point P = que.front(); que.pop();
            if (getPixel(P) == BG) {
                putPixel(P, 0, sciana.color);
                que.push(Point(P.x(), P.y()-1));
                que.push(Point(P.x(), P.y()+1));
                que.push(Point(P.x()-1, P.y()));
                que.push(Point(P.x()+1, P.y()));
            }
        }
    }

    update();
}

void MyWindow::getAxis(queue<Point> &que, Point A, Point B) {
    int x0 = A.x(), y0 = A.y(), x1 = B.x(), y1 = B.y();
    int dx = x1-x0;
    int dy = y1-y0;
    bool swaped = false;
    int error = 0;

    if (abs(dy) > abs(dx)) {
        swap(x0, y0);
        swap(x1, y1);
        swap(dx, dy);
        swaped = true;
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
        dx = -dx;
        dy = -dy;
    }

    for (int i = x0; i <= x1; ++i){
        if (2*error >= dx) {
            error -= dx;
            if (dy >= 0) y0++;
            else y0--;
        }
        error += abs(dy);
        if (!swaped) que.push(Point(i, y0));
        else que.push(Point(y0, i));
    }
}

void MyWindow::setTextured()
{
    textured = 1-textured;
    draw();
}

void MyWindow::on_horizontalSlider_valueChanged(int value)
{
    v[0] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_2_valueChanged(int value)
{
    v[1] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_3_valueChanged(int value)
{
    v[2] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_4_valueChanged(int value)
{
    v[3] = value;
    update();
    draw();
}
void MyWindow::on_horizontalSlider_5_valueChanged(int value)
{
    v[4] = value;
    update();
    draw();
}
void MyWindow::on_horizontalSlider_6_valueChanged(int value)
{
    v[5] = value;
    update();
    draw();
}
void MyWindow::on_horizontalSlider_7_valueChanged(int value)
{
    v[6] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_8_valueChanged(int value)
{
    v[7] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_9_valueChanged(int value)
{
    v[8] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_10_valueChanged(int value)
{
    v[9] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_11_valueChanged(int value)
{
    v[10] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_12_valueChanged(int value)
{
    v[11] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_13_valueChanged(int value)
{
    v[12] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_14_valueChanged(int value)
{
    v[13] = value;
    update();
    draw();
}

void MyWindow::on_horizontalSlider_15_valueChanged(int value)
{
    v[14] = value;
    update();
    draw();
}

void MyWindow::on_pushButton_clicked()
{
    setTextured();
}
