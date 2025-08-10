#include <iostream>
#include <cstring>  // 用于std::strlen和std::strcpy

class String {
private:
    char* data;  // 用于存储字符串，底层是一个动态的字符数组
    size_t length;  // 字符串的长度

public:
    // 默认构造函数
    String() : data(nullptr), length(0) {
        data = new char[1];
        data[0] = '\0';  // 空字符串
    }

    // 构造函数：从C风格字符串构造
    String(const char* str) {
        if (str == nullptr) {
            data = new char[1];
            data[0] = '\0';
            length = 0;
        } else {
            length = std::strlen(str);
            data = new char[length + 1];  // 为字符串分配内存
            std::strcpy(data, str);  // 复制字符串内容
        }
    }

    // 拷贝构造函数
    String(const String& other) {
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);  // 复制字符串内容
    }

    // 赋值运算符重载
    String& operator=(const String& other) {
        if (this == &other)  // 处理自赋值情况
            return *this;

        delete[] data;  // 释放当前的内存

        length = other.length;
        data = new char[length + 1];  // 分配新内存
        std::strcpy(data, other.data);  // 复制字符串内容
        return *this;
    }

    // 析构函数
    ~String() {
        delete[] data;  // 释放内存
    }

    // 获取字符串的长度
    size_t size() const {
        return length;
    }

    // 获取C风格字符串
    const char* c_str() const {
        return data;
    }

    // 字符串拼接
    String operator+(const String& other) const {
        char* new_data = new char[length + other.length + 1];
        std::strcpy(new_data, data);  // 复制当前字符串
        std::strcat(new_data, other.data);  // 拼接其他字符串
        String result(new_data);  // 使用临时字符数组创建新的String
        delete[] new_data;  // 释放临时字符数组
        return result;
    }

    // 字符串访问
    char& operator[](size_t index) {
        if (index >= length) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const char& operator[](size_t index) const {
        if (index >= length) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // 输出字符串
    void print() const {
        std::cout << data << std::endl;
    }
};

int main() {
    String str1("Hello");
    String str2(" World!");

    str1.print();  // 输出: Hello
    str2.print();  // 输出: World!

    String str3 = str1 + str2;
    str3.print();  // 输出: Hello World!

    std::cout << "Length of str3: " << str3.size() << std::endl;  // 输出: 12
    std::cout << "Character at index 6: " << str3[6] << std::endl;  // 输出: W

    return 0;
}
