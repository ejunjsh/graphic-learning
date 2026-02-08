// pointx.h
#pragma once

class Point {
public:
    double x, y;
    double h;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    Point(double x, double y, double h) : x(x), y(y), h(h) {}
};

class Pt {
public:
    int x, y;

    Pt(int x = 0, int y = 0) : x(x), y(y) {}
};