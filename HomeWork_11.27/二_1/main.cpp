#include <iostream>
using namespace std;

class Matrix {
private:
    int data[2][3];

public:
    Matrix() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                data[i][j] = 0;
            }
        }
    }

    // 重载+运算符
    Matrix operator+(const Matrix& other) {
        Matrix result;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                result.data[i][j] = this->data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    void input() {
        cout << "请输入矩阵数据（2行3列）：" << endl;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                cin >> data[i][j];
            }
        }
    }

    void display() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                cout << data[i][j] << "\t";
            }
            cout << endl;
        }
    }

};

int main() {
    Matrix a, b, c;

    cout << "a：" << endl;
    a.input();

    cout << "\nb：" << endl;
    b.input();

    c = a + b;

    cout << "\na：" << endl;
    a.display();

    cout << "\nb：" << endl;
    b.display();

    cout << "\nc = a + b：" << endl;
    c.display();

    return 0;
}

