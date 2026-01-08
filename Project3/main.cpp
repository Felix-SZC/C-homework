/*
 * 实验三：模板和泛型编程
 * 任务1：堆排序函数模板
 * 任务2：可变参数模板函数
 */

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// ==================== 任务1：堆排序函数模板 ====================

// 辅助函数：维护堆性质（最大堆）
template<typename T>
void heapify(T* A, int n, int i) {
    int largest = i;        // 初始化最大值为根节点
    int left = 2 * i + 1;   // 左子节点
    int right = 2 * i + 2;  // 右子节点

    // 如果左子节点存在且大于根节点
    if (left < n && A[left] > A[largest]) {
        largest = left;
    }

    // 如果右子节点存在且大于当前最大值
    if (right < n && A[right] > A[largest]) {
        largest = right;
    }

    // 如果最大值不是根节点，交换并继续调整
    if (largest != i) {
        swap(A[i], A[largest]);
        heapify(A, n, largest);
    }
}

// 堆排序函数模板
template<typename T>
void sort(T* A, int n) {
    // 步骤1：构建最大堆（从最后一个非叶子节点开始）
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(A, n, i);
    }

    // 步骤2-3：依次将堆顶元素与末尾元素交换，并调整堆
    for (int i = n - 1; i > 0; i--) {
        swap(A[0], A[i]);  // 将最大值移到末尾
        heapify(A, i, 0);  // 调整堆（排除已排序的元素）
    }
}

// 辅助函数：打印数组
template<typename T>
void printArray(T* A, int n) {
    for (int i = 0; i < n; i++) {
        cout << A[i];
        if (i < n - 1) cout << " ";
    }
    cout << endl;
}

// ==================== 任务2：可变参数模板函数 ====================

// 递归终止函数：当参数包为空时调用
void print() {
    cout << endl;
}

// 参数包展开函数：递归展开并输出参数
template<typename T, typename... Args>
void print(T first, Args... args) {
    cout << first;
    if (sizeof...(args) > 0) {
        cout << " ";  // 参数之间用空格分隔
    }
    print(args...);  // 递归处理剩余参数
}

// ==================== 主函数：测试代码 ====================

int main() {
    // ========== 任务1测试：堆排序函数模板 ==========
    cout << "【任务1】堆排序函数模板测试" << endl;
    cout << "----------------------------------------" << endl;

    // 测试1：整数数组
    cout << "\n测试1：整数数组排序" << endl;
    int intArr[] = {64, 34, 25, 12, 22, 11, 90, 5};
    int n1 = sizeof(intArr) / sizeof(intArr[0]);
    cout << "排序前: ";
    printArray(intArr, n1);
    sort(intArr, n1);
    cout << "排序后: ";
    printArray(intArr, n1);

    // 测试2：字符串数组
    cout << "\n测试2：字符串数组排序" << endl;
    string strArr[] = {"banana", "apple", "cherry", "date", "elderberry"};
    int n2 = sizeof(strArr) / sizeof(strArr[0]);
    cout << "排序前: ";
    printArray(strArr, n2);
    sort(strArr, n2);
    cout << "排序后: ";
    printArray(strArr, n2);

    // 测试3：浮点数数组
    cout << "\n测试3：浮点数数组排序" << endl;
    double doubleArr[] = {3.14, 2.71, 1.41, 0.57, 4.67, 1.73};
    int n4 = sizeof(doubleArr) / sizeof(doubleArr[0]);
    cout << "排序前: ";
    printArray(doubleArr, n4);
    sort(doubleArr, n4);
    cout << "排序后: ";
    printArray(doubleArr, n4);

    // 测试4：字符数组
    cout << "\n测试4：字符数组排序" << endl;
    char charArr[] = {'z', 'a', 'm', 'b', 'k', 'd', 'f'};
    int n5 = sizeof(charArr) / sizeof(charArr[0]);
    cout << "排序前: ";
    printArray(charArr, n5);
    sort(charArr, n5);
    cout << "排序后: ";
    printArray(charArr, n5);

    cout << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    // ========== 任务2测试：可变参数模板函数 ==========
    cout << "【任务2】可变参数模板函数测试" << endl;
    cout << "----------------------------------------" << endl;

    // 测试1：整数参数
    cout << "\n测试1：整数参数" << endl;
    cout << "输出: ";
    print(1, 2, 3, 4, 5);

    // 测试2：混合类型参数
    cout << "\n测试2：混合类型参数" << endl;
    cout << "输出: ";
    print("hello", 42, 3.14, 'A', "world");

    // 测试3：字符串参数
    cout << "\n测试3：字符串参数" << endl;
    cout << "输出: ";
    print("C++", "实验", "三", "测试");

    // 测试4：单个参数
    cout << "\n测试4：单个参数" << endl;
    cout << "输出: ";
    print(100);

    // 测试5：无参数（调用终止函数）
    cout << "\n测试5：无参数（终止函数）" << endl;
    cout << "输出: ";
    print();

    cout << endl;
    cout << "----------------------------------------" << endl;
    cout << "========== 实验完成 ==========" << endl;

    return 0;
}

