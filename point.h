// point.h
#pragma once

class Point {
public:
    int x, y;
    double h;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    Point(int x, int y, double h) : x(x), y(y), h(h) {}
};