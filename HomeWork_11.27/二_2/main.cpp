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

    // 重载流运算符
    friend istream& operator>>(istream& is, Matrix& m);
    friend ostream& operator<<(ostream& os, const Matrix& m);
};

// 重载流提取运算符 >>
istream& operator>>(istream& is, Matrix& m) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            is >> m.data[i][j];
        }
    }
    return is;
}

// 重载流插入运算符 <<
ostream& operator<<(ostream& os, const Matrix& m) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            os << m.data[i][j] << "\t";
        }
        os << endl;
    }
    return os;
}

int main() {
    Matrix a, b, c;

    cout << "a：" << endl;
    cin >> a;

    cout << "b：" << endl;
    cin >> b;

    c = a + b;

    cout << "\na：" << endl;
    cout << a;

    cout << "\nb：" << endl;
    cout << b;

    cout << "\nc = a + b：" << endl;
    cout << c;

    return 0;
}

