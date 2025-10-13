#include <iostream>
#include <windows.h> // 包含 Windows API 头文件

using namespace std;

class Cuboid {
    private:
        double length;
        double width;
        double height;
    
    public:
        void setDimensions(double l, double w, double h) {
            length = l;
            width = w;
            height = h;
        }

        double getVolume() {
            return length * width * height;
        }
};

int main() {
    SetConsoleOutputCP(936); // 设置控制台输出编码为 GBK (936)，兼容 GB2312
    Cuboid cuboids[3];
    double l, w, h;

    for (int i = 0; i < 3; i++) {
        cout << "请输入第" << i + 1 << "个长方柱的长 宽 高：" << endl;
        cin >> l >> w >> h;
        cuboids[i].setDimensions(l, w, h);
    }

    for (int i = 0; i < 3; i++) {
        cout << "第" << i + 1 << "个长方柱的体积是：" << cuboids[i].getVolume() << endl;
    }

    return 0;
}

