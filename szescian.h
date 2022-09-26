#ifndef SZESCIAN_H
#define SZESCIAN_H
#include <strukturaszescianu.h>

using namespace std;

/* Definicje kolorow RGB */
#define RED 0xFF0000
#define WHITE 0xFFFFFE
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define ORANGE 0xFF5800
#define BLACK 0x000000
#define BG 0xFFFFFF

/* Struktura reprezentujaca krawedz */
struct Edge {
    Point A, B; // wierzcholki tworzace sciane

    /* Konstruktor krawedzi */
    Edge() {}
    Edge(Point A, Point B) {
        this->A = A;
        this->B = B;
    }

    /* Srodek krawedzi */
    Point middle() {
        return Point((A.x()+B.x())/2, (A.y()+B.y())/2, (A.z()+B.z())/2, 1);
    }
};

/* Struktura reprezentujaca sciane szescianu */
struct Sciana {
    Point *A, *B, *C, *D; // wierzcholki tworzace sciane
    Operacje *M; // macierz obrotu kostki
    int color; // kolor sciany

    /* Konstruktor sciany */
    Sciana() {}
    Sciana(Point &A, Point &B, Point &C, Point &D, Operacje &M, int color) {
        this->A = &A;
        this->B = &B;
        this->C = &C;
        this->D = &D;
        this->M = &M;
        this->color = color;
    }

    /* Kat miedzy wektorem normalnym sciany a wektorem w strone kamery */
    double alpha() {
        return Edge((*M)*(*A), (*M)*(*C)).middle().z();
    }

    /* Porownywanie scian po wartosci alpha */
    bool operator < (Sciana f) {
        return this->alpha() < f.alpha();
    }
};

/* Struktura reprezentujaca pojedynczy szescian */
struct Szescian {
    vector <Point> wierzcholek = vector <Point>(8); // 8 wierzcholkow
    vector <Sciana> sciana = vector <Sciana>(6); // 6 scian

    /* Ustawienie domyslnych parametrów sześcianu */
    void reset(Operacje &M) {
        /* Ustawienie wspolrzednych bazowych wszystkich wierzcholkow */
        wierzcholek[0] = Point(-100, -100, -100, 1);
        wierzcholek[1] = Point(-100, -100,  100, 1);
        wierzcholek[2] = Point(-100,  100,  100, 1);
        wierzcholek[3] = Point(-100,  100, -100, 1);
        wierzcholek[4] = Point( 100, -100, -100, 1);
        wierzcholek[5] = Point( 100, -100,  100, 1);
        wierzcholek[6] = Point( 100,  100,  100, 1);
        wierzcholek[7] = Point( 100,  100, -100, 1);

        /* Utworzenie scian szescianu */
        sciana[0] = {wierzcholek[0], wierzcholek[1], wierzcholek[2], wierzcholek[3], M, WHITE};
        sciana[1] = {wierzcholek[0], wierzcholek[1], wierzcholek[5], wierzcholek[4], M, RED};
        sciana[2] = {wierzcholek[1], wierzcholek[2], wierzcholek[6], wierzcholek[5], M, BLUE};
        sciana[3] = {wierzcholek[2], wierzcholek[3], wierzcholek[7], wierzcholek[6], M, YELLOW};
        sciana[4] = {wierzcholek[3], wierzcholek[0], wierzcholek[4], wierzcholek[7], M, GREEN};
        sciana[5] = {wierzcholek[4], wierzcholek[5], wierzcholek[6], wierzcholek[7], M, ORANGE};
    }
};



#endif // SZESCIAN_H
