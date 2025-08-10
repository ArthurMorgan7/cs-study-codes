#include <iostream>
#include <cstring>  // for strlen, strcpy

class SimpleString {
private:
    char* data;     // 底层是一个动态的字符数组
    size_t len;     // 维护一个字符长度的len

public:
    // 默认构造函数
    SimpleString() : data(new char[1]), len(0) {
        data[0] = '\0'; // string默认也是有个 \0 的
    }

    // 从 C 字符串构造
    SimpleString(const char* str) {
        len = std::strlen(str);     // strlen不包含末尾 \0
        data = new char[len + 1];   // 多一个用来存放 \0
        std::strcpy(data, str);     // 本质还是两个字符数组的拷贝
    }

    // 拷贝构造
    SimpleString(const SimpleString& other) {
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
    }

    // 拷贝赋值
    SimpleString& operator=(const SimpleString& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            data = new char[len + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    // 析构函数
    ~SimpleString() {
        delete[] data;
    }

    // 索引访问
    char& operator[](size_t index) {
        return data[index];
    }

    const char& operator[](size_t index) const {
        return data[index];
    }

    // 长度
    size_t size() const {
        return len;
    }

    // C 风格字符串访问
    const char* c_str() const {
        return data;
    }

    // 拼接
    void append(const SimpleString& other) {
        char* new_data = new char[len + other.len + 1];
        std::strcpy(new_data, data);
        std::strcat(new_data, other.data);
        delete[] data;
        data = new_data;
        len += other.len;
    }

    // 字符串连接 +
    SimpleString operator+(const SimpleString& other) const {
        SimpleString result;
        result.len = len + other.len;
        result.data = new char[result.len + 1];
        std::strcpy(result.data, data);
        std::strcat(result.data, other.data);
        return result;
    }
};


int main() {
    SimpleString s1("Hello");
    SimpleString s2(" World");

    s1.append(s2);
    std::cout << "After append: " << s1.c_str() << "\n";

    SimpleString s3 = s1 + SimpleString("!");
    std::cout << "After + operator: " << s3.c_str() << "\n";

    s3[0] = 'h';
    std::cout << "After modify: " << s3.c_str() << "\n";

    return 0;
}
