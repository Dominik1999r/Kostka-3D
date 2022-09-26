#ifndef STRUKTURASZESCIANU_H
#define STRUKTURASZESCIANU_H

#include <vector>
#include <cmath>
using namespace std;

/* Struktura reprezentujaca punkt w przestrzeni */
struct Point {
    double v[4];

    /* Konstruktor punktu */
    Point(double x = 0, double y = 0, double z = 0, double w = 0) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }

    /* Normalizacja punktu */
    void normalization() {
        if (v[3] != 0) {
            for (int i = 0; i < 4; ++i)
                v[i] /= v[3];
        }
    }

    /* Gettery */
    double x() { return v[0]; }
    double y() { return v[1]; }
    double z() { return v[2]; }

    /* Roznica punktow po wspolrzednych (do tworzenia wektorow) */
    Point operator -(Point B) {
        Point A = *this;
        return Point(A.x()-B.x(), A.y()-B.y(), A.z()-B.z(), 1);
    }

    /* Iloczyn skalarny dwoch wektorow */
    double operator *(Point B) {
        Point A = *this;
        return A.x()*B.x() + A.y()*B.y() + A.z()*B.z();
    }

    /* Porownywanie punktow po wspolrzednych */
    bool operator <(Point B) {
        Point A = *this;
        double eps = 0.0001;

        if (abs(A.x() - B.x()) < eps) {
            if (abs(A.y() - B.y()) < eps) {
                return A.z() < B.z();
            }
            else return A.y() < B.y();
        }
        else return A.x() < B.x();
    }
};

struct Operacje
{
    double v[4][4];

    Operacje(int x)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                v[i][j] = x*(i==j);

    }

    Operacje(double tab[4][4]){
        for (int i = 0; i < 16; ++i)
            v[i/4][i%4] = tab[i/4][i%4];
    }

    static Operacje translation(double x, double y, double z)
    {
        double tab[4][4] = { {1,0,0,x},
                             {0,1,0,y},
                             {0,0,1,z},
                             {0,0,0,1} };
        return Operacje(tab);
    }

    static Operacje scale(double x, double y, double z)
    {
        x /= 100; y /= 100; z /= 100;
        double tab[4][4] = { {x,0,0,0},
                             {0,y,0,0},
                             {0,0,z,0},
                             {0,0,0,1} };
        return Operacje(tab);
    }

    static Operacje shearing(double yx, double zx, double xy, double zy, double xz, double yz)
    {
        yx /= 100; zx /= 100; xy /= 100; zy /= 100; xz /= 100; yz /= 100;
        double tab[4][4] = { {1,xy,xz,0},
                             {yx,1,yz,0},
                             {zx,zy,1,0},
                             {0,0,0,1} };
        return Operacje(tab);
    }

    static Operacje rotate(double x, double y, double z)
    {
        x *= 0.0174532925; y *= 0.0174532925; z *= 0.0174532925;
        Operacje result = Operacje(1);

        double tab1[4][4] = { {1,0,0,0},
                              {0,cos(x),-sin(x),0},
                              {0,sin(x),cos(x),0},
                              {0,0,0,1} };
        result = result * Operacje(tab1);

        double tab2[4][4] = { {cos(y),0,sin(y),0},
                              {0,1,0,0},
                              {-sin(y),0,cos(y),0},
                              {0,0,0,1} };
        result = result * Operacje(tab2);

        double tab3[4][4] = { {cos(z),-sin(z),0,0},
                              {sin(z),cos(z),0,0},
                              {0,0,1,0},
                              {0,0,0,1} };
        result = result * Operacje(tab3);
        return result;
    }

    static Operacje projection(double d = 500)
    {
        double tab[4][4] = { {1,0,0,0},
                             {0,1,0,0},
                             {0,0,1,0},
                             {0,0,1/d,1} };
        return Operacje(tab);
    }

    Operacje operator *(Operacje B)
    {
        Operacje A = *this;
        Operacje result = Operacje(0);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    result.v[i][j] += A.v[i][k] * B.v[k][j];
        return result;
    }

    Point operator *(Point B)
    {
        Operacje A = *this;
        Point result = Point();
        for (int i = 0; i < 4; ++i)
            for (int k = 0; k < 4; ++k)
                result.v[i] += A.v[i][k] * B.v[k];
        result.normalization();
        return result;
    }
};



#endif // STRUKTURASZESCIANU_H
