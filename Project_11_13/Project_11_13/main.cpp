/*
 * 实验二：继承、派生、多态性
 * 功能：实现抽象图形基类Shape，以及Circle, Square, Parallelogram, EquilateralTriangle, RegularHexagon等派生类
 *      并实现其面积、周长、绘制、变换等操作。
 * 使用EGE图形库进行可视化
 */

#include <graphics.h>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

const double MY_PI = 3.14159265358979323846;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// 前向声明
class Point;

//==============================================================================
// 1. 抽象基类 Shape
//==============================================================================
class Shape {
protected:
    // 辅助函数，用于对点进行变换
    static void rotatePointAround(Point& p, const Point& center, double angle);
    static void mirrorPointAround(Point& p, const Point& center, bool horizontal);
    static void scalePointAround(Point& p, const Point& center, double factor);

public:
    Shape() = default;
    virtual ~Shape() = default;

    // 纯虚函数，定义接口
    virtual double getArea() const = 0;
    virtual double getPerimeter() const = 0;
    virtual void draw(color_t color) const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void rotate(double angle) = 0;
    virtual void scale(double factor) = 0;
    virtual string getInfo() const = 0;
};

//==============================================================================
// 2. Point 类 (作为辅助类，不继承Shape)
//==============================================================================
class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }

    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }

    double distanceTo(const Point& p) const {
        return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
    }
};

// Shape类的辅助函数实现
void Shape::rotatePointAround(Point& p, const Point& center, double angle) {
    double rad = angle * MY_PI / 180.0;
    double current_x = p.getX() - center.getX();
    double current_y = p.getY() - center.getY();
    double newX = current_x * cos(rad) - current_y * sin(rad) + center.getX();
    double newY = current_x * sin(rad) + current_y * cos(rad) + center.getY();
    p.setX(newX);
    p.setY(newY);
}

void Shape::mirrorPointAround(Point& p, const Point& center, bool horizontal) {
    if (horizontal) {
        p.setY(2 * center.getY() - p.getY());
    }
    else {
        p.setX(2 * center.getX() - p.getX());
    }
}

void Shape::scalePointAround(Point& p, const Point& center, double factor) {
    p.setX(center.getX() + (p.getX() - center.getX()) * factor);
    p.setY(center.getY() + (p.getY() - center.getY()) * factor);
}

//==============================================================================
// 3. 派生类
//==============================================================================

//------------------------- Circle 圆形 -------------------------
class Circle : public Shape {
private:
    Point center;
    double radius;

public:
    Circle(const Point& c, double r) : center(c), radius(r) {}
    Circle(double x, double y, double r) : center(x, y), radius(r) {}

    double getArea() const override { return MY_PI * radius * radius; }
    double getPerimeter() const override { return 2 * MY_PI * radius; }

    void draw(color_t color) const override {
        setcolor(color);
        setlinewidth(2);
        circle((int)center.getX(), (int)center.getY(), (int)radius);
    }

    void move(double dx, double dy) override { center.move(dx, dy); }
    void rotate(double angle) override { /* 圆绕自身中心旋转不变 */ }
    void scale(double factor) override { radius *= factor; }

    string getInfo() const override {
        ostringstream oss;
        oss << "圆: 中心(" << center.getX() << ", " << center.getY() << "), 半径 " << radius
            << " | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
};

//------------------------- Polygon 多边形 (作为其他多边形的虚基类) -------------------------
class Polygon : public virtual Shape {
protected:
    vector<Point> vertices;

public:
    Polygon(const vector<Point>& v) : vertices(v) {}

    Point getCenter() const {
        double sumX = 0, sumY = 0;
        for (const auto& v : vertices) {
            sumX += v.getX();
            sumY += v.getY();
        }
        return Point(sumX / vertices.size(), sumY / vertices.size());
    }

    void move(double dx, double dy) override {
        for (auto& v : vertices) {
            v.move(dx, dy);
        }
    }

    void rotate(double angle) override {
        Point center = getCenter();
        for (auto& v : vertices) {
            rotatePointAround(v, center, angle);
        }
    }

    void scale(double factor) override {
        Point center = getCenter();
        for (auto& v : vertices) {
            scalePointAround(v, center, factor);
        }
    }

    void draw(color_t color) const override {
        setcolor(color);
        setlinewidth(2);
        if (vertices.size() < 2) return;
        for (size_t i = 0; i < vertices.size(); ++i) {
            const Point& p1 = vertices[i];
            const Point& p2 = vertices[(i + 1) % vertices.size()];
            line((int)p1.getX(), (int)p1.getY(), (int)p2.getX(), (int)p2.getY());
        }
    }
};

//------------------------- Parallelogram 平行四边形 -------------------------
class Parallelogram : public Polygon {
public:
    // 构造函数：p1为左下角，p2为右下角，p3为右上角
    // 第四个点p4（左上角）= p1 + (p3 - p2)
    // 顶点顺序：p1(左下) -> p2(右下) -> p3(右上) -> p4(左上)
    Parallelogram(const Point& p1, const Point& p2, const Point& p3)
        : Polygon({ p1, p2, p3, Point(p1.getX() + (p3.getX() - p2.getX()), p1.getY() + (p3.getY() - p2.getY())) }) {
    }

    double getArea() const override {
        // 使用向量叉积计算平行四边形面积
        const Point& p1 = vertices[0];
        const Point& p2 = vertices[1];
        const Point& p4 = vertices[3];
        return abs((p2.getX() - p1.getX()) * (p4.getY() - p1.getY()) - (p4.getX() - p1.getX()) * (p2.getY() - p1.getY()));
    }

    double getPerimeter() const override {
        return 2 * (vertices[0].distanceTo(vertices[1]) + vertices[1].distanceTo(vertices[2]));
    }

    string getInfo() const override {
        ostringstream oss;
        oss << "平行四边形 | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
};

//------------------------- Square 正方形 (保护继承自Parallelogram，同时公有继承Shape以支持多态) -------------------------
class Square : protected Parallelogram, virtual public Shape {
public:
    Square(const Point& center, double side)
        : Parallelogram(
            Point(center.getX() - side / 2, center.getY() - side / 2),
            Point(center.getX() + side / 2, center.getY() - side / 2),
            Point(center.getX() + side / 2, center.getY() + side / 2)
        ) {
    }

    // 通过 using 声明将基类的公有方法提升为公有，以满足多态性要求
    using Polygon::draw;
    using Polygon::move;
    using Polygon::rotate;
    using Polygon::scale;

    double getArea() const override { return Parallelogram::getArea(); }
    double getPerimeter() const override { return Parallelogram::getPerimeter(); }

    string getInfo() const override {
        ostringstream oss;
        oss << "正方形 | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
};

//------------------------- EquilateralTriangle 正三角形 -------------------------
class EquilateralTriangle : public Polygon {
public:
    EquilateralTriangle(const Point& center, double side) : Polygon({}) {
        double h = side * sqrt(3.0) / 2.0;
        vertices.push_back(Point(center.getX(), center.getY() - 2.0 * h / 3.0));
        vertices.push_back(Point(center.getX() - side / 2.0, center.getY() + h / 3.0));
        vertices.push_back(Point(center.getX() + side / 2.0, center.getY() + h / 3.0));
    }

    double getArea() const override { return pow(vertices[0].distanceTo(vertices[1]), 2) * sqrt(3.0) / 4.0; }
    double getPerimeter() const override { return 3 * vertices[0].distanceTo(vertices[1]); }

    string getInfo() const override {
        ostringstream oss;
        oss << "正三角形 | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
};

//------------------------- RegularHexagon 正六边形 -------------------------
class RegularHexagon : public Polygon {
public:
    RegularHexagon(const Point& center, double side) : Polygon({}) {
        for (int i = 0; i < 6; ++i) {
            double angle_rad = MY_PI / 180.0 * (60 * i);
            vertices.push_back(Point(center.getX() + side * cos(angle_rad), center.getY() + side * sin(angle_rad)));
        }
    }

    double getArea() const override { return 3.0 * sqrt(3.0) / 2.0 * pow(vertices[0].distanceTo(getCenter()), 2); }
    double getPerimeter() const override { return 6 * vertices[0].distanceTo(getCenter()); }

    string getInfo() const override {
        ostringstream oss;
        oss << "正六边形 | 面积: " << getArea() << " | 周长: " << getPerimeter();
        return oss.str();
    }
};


//==============================================================================
// 4. 辅助绘图函数
//==============================================================================
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

//==============================================================================
// 5. 工具函数：创建初始图形 & 释放图形
//==============================================================================
vector<Shape*> createInitialShapes() {
    vector<Shape*> shapes;
    // 整体下移，避免与顶部文字重叠
    shapes.push_back(new Circle(150, 260, 60));
    shapes.push_back(new Square(Point(350, 260), 100));
    shapes.push_back(new Parallelogram(Point(520, 210), Point(620, 210), Point(650, 290)));
    shapes.push_back(new EquilateralTriangle(Point(900, 260), 120));
    shapes.push_back(new RegularHexagon(Point(1050, 260), 60));
    return shapes;
}

void destroyShapes(vector<Shape*>& shapes) {
    for (auto shape : shapes) {
        delete shape;
    }
    shapes.clear();
}

//==============================================================================
// 6. 主函数 main
//==============================================================================
int main() {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setcaption("实验二: 学号: 24061824 姓名: 盛智超");
    setbkcolor(EGERGB(20, 20, 40)); // 深蓝色背景

    // --- 1. 原始图形 ---
    {
        vector<Shape*> shapes = createInitialShapes();

        cleardevice();
        drawTitle("1: 原始图形", 20);
        int textY = 50;
        for (const auto& shape : shapes) {
            drawText(20, textY, shape->getInfo(), LIGHTCYAN);
            textY += 25;
            shape->draw(WHITE); // 原图统一白色
        }
        drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
        getch();

        destroyShapes(shapes);
    }

    // --- 2. 移动操作（原图位置与第一步相同） ---
    {
        vector<Shape*> originalShapes = createInitialShapes();
        vector<Shape*> movedShapes    = createInitialShapes();

        cleardevice();
        drawTitle("2: 移动操作 (dx=50, dy=50)", 20);
        for (size_t i = 0; i < originalShapes.size(); ++i) {
            originalShapes[i]->draw(WHITE); // 原图：白色
            movedShapes[i]->move(50, 50);
            movedShapes[i]->draw(RED);      // 变换图：红色
        }
        drawText(800, 20, "白: 原图", WHITE);
        drawText(800, 50, "红: 移动后", RED);
        drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
        getch();

        destroyShapes(originalShapes);
        destroyShapes(movedShapes);
    }

    // --- 3. 旋转操作（原图位置与第一步相同） ---
    {
        vector<Shape*> originalShapes = createInitialShapes();
        vector<Shape*> rotatedShapes  = createInitialShapes();

        cleardevice();
        drawTitle("3: 旋转操作 (45度)", 20);
        for (size_t i = 0; i < originalShapes.size(); ++i) {
            originalShapes[i]->draw(WHITE); // 原图：白色
            rotatedShapes[i]->rotate(45);
            rotatedShapes[i]->draw(RED);     // 变换图：红色
        }
        drawText(800, 20, "白: 原图", WHITE);
        drawText(800, 50, "红: 旋转后", RED);
        drawText(20, WINDOW_HEIGHT - 40, "按任意键继续...", YELLOW);
        getch();

        destroyShapes(originalShapes);
        destroyShapes(rotatedShapes);
    }

    // --- 4. 缩放操作（原图位置与第一步相同） ---
    {
        vector<Shape*> originalShapes = createInitialShapes();
        vector<Shape*> scaledShapes   = createInitialShapes();

        cleardevice();
        drawTitle("4: 缩放操作 (0.8倍)", 20);
        for (size_t i = 0; i < originalShapes.size(); ++i) {
            originalShapes[i]->draw(WHITE); // 原图：白色
            scaledShapes[i]->scale(0.8);
            scaledShapes[i]->draw(RED);      // 变换图：红色
        }
        drawText(800, 20, "白: 原图", WHITE);
        drawText(800, 50, "红: 缩放后", RED);
        drawText(20, WINDOW_HEIGHT - 40, "按任意键退出...", YELLOW);
        getch();

        destroyShapes(originalShapes);
        destroyShapes(scaledShapes);
    }

    closegraph();
    return 0;
}