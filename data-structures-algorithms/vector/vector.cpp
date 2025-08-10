#include <iostream>
#include <stdexcept>
#include <algorithm>  // std::copy

template <typename T>
class SimpleVector {
private:
    T* data;
    size_t _size;
    size_t _capacity;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < _size; ++i)
            new_data[i] = std::move(data[i]);
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
    }

public:
    SimpleVector() : data(nullptr), _size(0), _capacity(0) {}

    ~SimpleVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            size_t new_cap = _capacity == 0 ? 1 : _capacity * 2;
            reallocate(new_cap);
        }
        data[_size++] = value;
    }

    void pop_back() {
        if (_size > 0)
            --_size;
    }

    T& operator[](size_t index) {
        if (index >= _size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    bool empty() const {
        return _size == 0;
    }
};


int main() {
    SimpleVector<int> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Vector size: " << vec.size() << "\n";
    std::cout << "Vector capacity: " << vec.capacity() << "\n";

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";

    vec.pop_back();
    std::cout << "After pop_back, size: " << vec.size() << "\n";
    std::cout << "Last element: " << vec[vec.size() - 1] << "\n";

    return 0;
}
