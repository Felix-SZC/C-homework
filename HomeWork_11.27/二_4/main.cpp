#include <iostream>
#include <cmath>
using namespace std;

class Shape {
public:
    virtual double getArea() const = 0;
    
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}
    
    double getArea() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) : length(l), width(w) {}
    
    double getArea() const override {
        return length * width;
    }
};

class Triangle : public Shape {
private:
    double base;
    double height;

public:
    Triangle(double b, double h) : base(b), height(h) {}
    
    double getArea() const override {
        return 0.5 * base * height;
    }
};

void printArea(Shape* shape) {
    cout << "面积为: " << shape->getArea() << endl;
}

int main() {
    Circle circle(5.0);           // 圆形，半径为5
    Rectangle rectangle(4.0, 6.0); // 矩形，长为4，宽为6
    Triangle triangle(3.0, 4.0);   // 三角形，底为3，高为4
    
    cout << "圆形（半径=5）：" << endl;
    printArea(&circle);
    
    cout << "\n矩形（长=4，宽=6）：" << endl;
    printArea(&rectangle);
    
    cout << "\n三角形（底=3，高=4）：" << endl;
    printArea(&triangle);
    
    return 0;
}

