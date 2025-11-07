/*
 * 实验一：类和对象 - 平面几何图形类实现
 * 功能：实现点、线段、圆、矩形、三角形类及其各种变换操作
 * 使用EGE图形库进行可视化
 */

 #include <graphics.h>
 #include <cmath>
 #include <vector>
 #include <string>
 #include <sstream>
 
 using namespace std;
 
const double MY_PI = 3.14159265358979323846;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

class Point;

class Shape {
protected:
    static void rotatePointAround(Point& p, const Point& center, double angle);
    static void mirrorPointAround(Point& p, const Point& center, bool horizontal);
    static void scalePointAround(Point& p, const Point& center, double factor);

public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual double getArea() const = 0;
    virtual double getPerimeter() const = 0;
    virtual void draw(color_t color) const = 0;
    virtual void rotate(double angle) = 0;
    virtual void mirror(bool horizontal) = 0;
    virtual void scale(double factor) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual string getInfo() const = 0;
};

class Point : public Shape {
 private:
     double x, y;
     bool counted;
     static int instanceCount;

 public:
     Point(double x = 0, double y = 0, bool registerInstance = false)
         : x(x), y(y), counted(registerInstance) {
         if (counted) {
             ++instanceCount;
         }
     }
     Point(const Point& other) : x(other.x), y(other.y), counted(false) {}

     Point& operator=(const Point& other) {
         if (this != &other) {
             x = other.x;
             y = other.y;
         }
         return *this;
     }

     ~Point() override {
         if (counted) {
             --instanceCount;
         }
     }

     static int getInstanceCount() { return instanceCount; }
 
     double getX() const { return x; }
     double getY() const { return y; }
     void setX(double newX) { x = newX; }
     void setY(double newY) { y = newY; }

     double getArea() const override { return 0; }
     double getPerimeter() const override { return 0; }
 
     void draw(color_t color) const override {
         setcolor(color);
         setfillcolor(color);
         fillellipse((int)(x - 2), (int)(y - 2), 5, 5);
     }
 
     void rotate(double angle) override {
         double rad = angle * MY_PI / 180.0;
         double newX = x * cos(rad) - y * sin(rad);
         double newY = x * sin(rad) + y * cos(rad);
         x = newX;
         y = newY;
     }

     void mirror(bool horizontal) override {
         if (horizontal) {
             y = -y;
         } else {
             x = -x;
         }
     }

     void scale(double factor) override {
         x *= factor;
         y *= factor;
     }

     void move(double dx, double dy) override {
         x += dx;
         y += dy;
     }
 
    string getInfo() const override {
        ostringstream oss;
        oss << "点" << " 位于 (" << x << ", " << y << ")";
        oss << " | 面积: 0";
        oss << " | 周长: 0";
        return oss.str();
    }
 
    double distanceTo(const Point& p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
};

void Shape::rotatePointAround(Point& p, const Point& center, double angle) {
    double rad = angle * MY_PI / 180.0;
    double x = p.getX() - center.getX();
    double y = p.getY() - center.getY();
    double newX = x * cos(rad) - y * sin(rad) + center.getX();
    double newY = x * sin(rad) + y * cos(rad) + center.getY();
    p.setX(newX);
    p.setY(newY);
}

void Shape::mirrorPointAround(Point& p, const Point& center, bool horizontal) {
    if (horizontal) {
        p.setY(2 * center.getY() - p.getY());
    } else {
        p.setX(2 * center.getX() - p.getX());
    }
}

void Shape::scalePointAround(Point& p, const Point& center, double factor) {
    p.setX(center.getX() + (p.getX() - center.getX()) * factor);
    p.setY(center.getY() + (p.getY() - center.getY()) * factor);
}

class LineSegment : public Shape {
 private:
     Point p1, p2;
     static int instanceCount;
 
     Point getCenter() const {
         return Point((p1.getX() + p2.getX()) / 2, (p1.getY() + p2.getY()) / 2);
     }
 
 public:
     LineSegment(const Point& pt1, const Point& pt2) : p1(pt1), p2(pt2) { ++instanceCount; }
     LineSegment(double x1, double y1, double x2, double y2)
         : p1(x1, y1), p2(x2, y2) {
         ++instanceCount;
     }
     ~LineSegment() override { --instanceCount; }

     static int getInstanceCount() { return instanceCount; }
 
     double getArea() const override { return 0; }

     double getPerimeter() const override {
         return p1.distanceTo(p2);
     }
 
     void draw(color_t color) const override {
         setcolor(color);
         setlinewidth(2);
         line((int)p1.getX(), (int)p1.getY(), (int)p2.getX(), (int)p2.getY());
         p1.draw(color);
         p2.draw(color);
     }

    void rotate(double angle) override {
        Point center = getCenter();
        rotatePointAround(p1, center, angle);
        rotatePointAround(p2, center, angle);
    }

    void mirror(bool horizontal) override {
        Point center = getCenter();
        mirrorPointAround(p1, center, horizontal);
        mirrorPointAround(p2, center, horizontal);
    }

    void scale(double factor) override {
        Point center = getCenter();
        scalePointAround(p1, center, factor);
        scalePointAround(p2, center, factor);
    }
 
     void move(double dx, double dy) override {
         p1.move(dx, dy);
         p2.move(dx, dy);
     }
 
    string getInfo() const override {
        ostringstream oss;
        oss << "线段" << " 从 (" << p1.getX() << ", " << p1.getY()
            << ") 到 (" << p2.getX() << ", " << p2.getY() << ")";
        oss << " | 面积: 0";
        oss << " | 周长: " << getPerimeter();
        return oss.str();
    }
 };
 
class Circle : public Shape {
 private:
     Point center;
     double radius;
     static int instanceCount;
 
 public:
     Circle(const Point& c, double r) : center(c), radius(r) { ++instanceCount; }
     Circle(double x, double y, double r) : center(x, y), radius(r) { ++instanceCount; }
     ~Circle() override { --instanceCount; }

     static int getInstanceCount() { return instanceCount; }
 
     double getArea() const override {
         return MY_PI * radius * radius;
     }
 
     double getPerimeter() const override {
         return 2 * MY_PI * radius;
     }
 
     void draw(color_t color) const override {
         setcolor(color);
         setlinewidth(2);
         circle((int)center.getX(), (int)center.getY(), (int)radius);
         center.draw(color);
     }

     void rotate(double angle) override {
     }

     void mirror(bool horizontal) override {
         Point origin(0, 0);
         mirrorPointAround(center, origin, horizontal);
     }
 
     void scale(double factor) override {
         radius *= factor;
     }
 
     void move(double dx, double dy) override {
         center.move(dx, dy);
     }
 
    string getInfo() const override {
        ostringstream oss;
        oss << "圆" << " 位于 (" << center.getX() << ", " << center.getY()
            << ") 半径 " << radius;
        oss << " | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
 };
 
class Rect : public Shape {
 private:
     Point topLeft;
     double width, height;
     static int instanceCount;
 
     Point getCenter() const {
         return Point(topLeft.getX() + width / 2, topLeft.getY() + height / 2);
     }
 
 public:
     Rect(const Point& tl, double w, double h)
         : topLeft(tl), width(w), height(h) {
         ++instanceCount;
     }
     Rect(double x, double y, double w, double h)
         : topLeft(x, y), width(w), height(h) {
         ++instanceCount;
     }
     ~Rect() override { --instanceCount; }

     static int getInstanceCount() { return instanceCount; }
 
     double getArea() const override {
         return width * height;
     }
 
     double getPerimeter() const override {
         return 2 * (width + height);
     }
 
     void draw(color_t color) const override {
         setcolor(color);
         setlinewidth(2);
         rectangle((int)topLeft.getX(), (int)topLeft.getY(),
             (int)(topLeft.getX() + width), (int)(topLeft.getY() + height));
         topLeft.draw(color);
         Point(topLeft.getX() + width, topLeft.getY()).draw(color);
         Point(topLeft.getX(), topLeft.getY() + height).draw(color);
         Point(topLeft.getX() + width, topLeft.getY() + height).draw(color);
     }

    void rotate(double angle) override {
        Point center = getCenter();
        double normalizedAngle = fmod(angle, 360.0);
        if (normalizedAngle < 0) normalizedAngle += 360.0;

        const double ANGLE_90 = 90.0;
        const double ANGLE_180 = 180.0;
        const double ANGLE_270 = 270.0;
        const double ANGLE_TOLERANCE = 1.0;

        if (fabs(normalizedAngle - ANGLE_90) < ANGLE_TOLERANCE || 
            fabs(normalizedAngle - ANGLE_270) < ANGLE_TOLERANCE) {
            double temp = width;
            width = height;
            height = temp;
            topLeft.setX(center.getX() - width / 2);
            topLeft.setY(center.getY() - height / 2);
        }
        else if (fabs(normalizedAngle - ANGLE_180) < ANGLE_TOLERANCE) {
            topLeft.setX(center.getX() - width / 2);
            topLeft.setY(center.getY() - height / 2);
        }
     }

     void mirror(bool horizontal) override {
         Point center = getCenter();
         if (horizontal) {
             double newY = 2 * center.getY() - topLeft.getY() - height;
             topLeft.setY(newY);
         } else {
             double newX = 2 * center.getX() - topLeft.getX() - width;
             topLeft.setX(newX);
         }
     }
 
     void scale(double factor) override {
         Point center = getCenter();
         width *= factor;
         height *= factor;
         topLeft.setX(center.getX() - width / 2);
         topLeft.setY(center.getY() - height / 2);
     }
 
     void move(double dx, double dy) override {
         topLeft.move(dx, dy);
     }
 
    string getInfo() const override {
        ostringstream oss;
        oss << "矩形" << " 位于 (" << topLeft.getX() << ", " << topLeft.getY()
            << ") 宽度 " << width << " 高度 " << height;
        oss << " | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
 };
 
class Triangle : public Shape {
 private:
     Point p1, p2, p3;
     static int instanceCount;
 
     Point getCenter() const {
         return Point((p1.getX() + p2.getX() + p3.getX()) / 3,
                      (p1.getY() + p2.getY() + p3.getY()) / 3);
     }
 
 public:
     Triangle(const Point& pt1, const Point& pt2, const Point& pt3)
         : p1(pt1), p2(pt2), p3(pt3) {
         ++instanceCount;
     }
     Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
         : p1(x1, y1), p2(x2, y2), p3(x3, y3) {
         ++instanceCount;
     }
     ~Triangle() override { --instanceCount; }

     static int getInstanceCount() { return instanceCount; }
 
     double getArea() const override {
         return fabs((p1.getX() * (p2.getY() - p3.getY()) +
             p2.getX() * (p3.getY() - p1.getY()) +
             p3.getX() * (p1.getY() - p2.getY())) / 2.0);
     }
 
     double getPerimeter() const override {
         return p1.distanceTo(p2) + p2.distanceTo(p3) + p3.distanceTo(p1);
     }
 
     void draw(color_t color) const override {
         setcolor(color);
         setlinewidth(2);
         line((int)p1.getX(), (int)p1.getY(), (int)p2.getX(), (int)p2.getY());
         line((int)p2.getX(), (int)p2.getY(), (int)p3.getX(), (int)p3.getY());
         line((int)p3.getX(), (int)p3.getY(), (int)p1.getX(), (int)p1.getY());
         p1.draw(color);
         p2.draw(color);
         p3.draw(color);
     }

    void rotate(double angle) override {
        Point center = getCenter();
        rotatePointAround(p1, center, angle);
        rotatePointAround(p2, center, angle);
        rotatePointAround(p3, center, angle);
    }

    void mirror(bool horizontal) override {
        Point center = getCenter();
        mirrorPointAround(p1, center, horizontal);
        mirrorPointAround(p2, center, horizontal);
        mirrorPointAround(p3, center, horizontal);
    }

    void scale(double factor) override {
        Point center = getCenter();
        scalePointAround(p1, center, factor);
        scalePointAround(p2, center, factor);
        scalePointAround(p3, center, factor);
    }
 
     void move(double dx, double dy) override {
         p1.move(dx, dy);
         p2.move(dx, dy);
         p3.move(dx, dy);
     }
 
    string getInfo() const override {
        ostringstream oss;
        oss << "三角形" << " 顶点: (" << p1.getX() << ", " << p1.getY() << "), ("
            << p2.getX() << ", " << p2.getY() << "), (" << p3.getX() << ", " << p3.getY() << ")";
        oss << " | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
 };
 
int Point::instanceCount = 0;
int LineSegment::instanceCount = 0;
int Circle::instanceCount = 0;
int Rect::instanceCount = 0;
int Triangle::instanceCount = 0;

 void drawText(int x, int y, const string& text, color_t color = WHITE) {
     setcolor(color);
     setfont(16, 0, "宋体");
     outtextxy(x, y, text.c_str());
 }
 
 void drawTitle(const string& title, int y) {
     setcolor(YELLOW);
     setfont(20, 0, "黑体");
     outtextxy(20, y, title.c_str());
 }

 int main() {
     initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
     setcaption("几何图形变换 - 学号: 24061824 姓名: 盛智超   ");
     setbkcolor(BLACK);
 
    vector<Shape*> shapes;
    shapes.push_back(new Point(100, 250, true));
    shapes.push_back(new LineSegment(200, 250, 350, 300));
    shapes.push_back(new Circle(500, 300, 50));
    shapes.push_back(new Rect(650, 250, 100, 80));
    shapes.push_back(new Triangle(900, 230, 950, 350, 850, 350));
 
     cleardevice();
     drawTitle("1: 原始图形", 20);
     int textY = 50;
    ostringstream countInfo;
    countInfo << "对象数量: 点=" << Point::getInstanceCount()
              << " 线段=" << LineSegment::getInstanceCount()
              << " 圆=" << Circle::getInstanceCount()
              << " 矩形=" << Rect::getInstanceCount()
              << " 三角形=" << Triangle::getInstanceCount();
    drawText(20, textY, countInfo.str(), LIGHTGREEN);
    textY += 25;
     for (auto shape : shapes) {
         drawText(20, textY, shape->getInfo(), LIGHTCYAN);
         textY += 25;
     }
     for (auto shape : shapes) {
         shape->draw(WHITE);
     }
     drawText(800, 20, "白：原图", WHITE);
     drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
     getch();

     cleardevice();
     drawTitle("2: 移动操作 (dx=50, dy=100)", 20);
     for (auto shape : shapes) {
         shape->draw(WHITE);
     }
     drawText(800, 20, "白：原图", WHITE);
     for (auto shape : shapes) {
         shape->move(50, 100);
         shape->draw(RED);
     }
     drawText(800, 50, "红：改图", RED);
     drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
     getch();

     cleardevice();
     drawTitle("3: 旋转操作 (45度)", 20);
     for (auto shape : shapes) {
         shape->draw(WHITE);
     }
     drawText(800, 20, "白：原图", WHITE);
     for (auto shape : shapes) {
         shape->rotate(45);
         shape->draw(RED);
     }
     drawText(800, 50, "红：改图", RED);
     drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
     getch();

     cleardevice();
     drawTitle("4: 缩放操作 (1.5倍)", 20);
     for (auto shape : shapes) {
         shape->draw(WHITE);
     }
     drawText(800, 20, "白：原图", WHITE);
     for (auto shape : shapes) {
         shape->scale(1.5);
         shape->draw(RED);
     }
     drawText(800, 50, "红：改图", RED);
     drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
     getch();

     cleardevice();
     drawTitle("5: 水平镜像操作", 20);
     for (auto shape : shapes) {
         shape->draw(WHITE);
     }
     drawText(800, 20, "白：原图", WHITE);
     for (auto shape : shapes) {
         shape->mirror(true);
         shape->draw(RED);
     }
     drawText(800, 50, "红：改图", RED);
     drawText(20, WINDOW_HEIGHT - 40, "按任意键退出程序...", YELLOW);
     getch();

     for (auto shape : shapes) {
         delete shape;
     }
 
     closegraph();
 
     return 0;
 }
 