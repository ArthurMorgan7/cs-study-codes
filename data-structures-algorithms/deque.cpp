#include <iostream>
#include <vector>
#include <stdexcept>

const int BUFFER_SIZE = 4;  // 每个 buffer 的容量

template<typename T>
class SimpleDeque {
private:
    using Buffer = std::vector<T>;
    using BufferPtr = Buffer*;

    std::vector<BufferPtr> map;        // 控制中心：存储指向 buffer 的指针
    std::vector<BufferPtr> freeList;   // 已回收的 buffer，可重用

    int startIndex;    // 起始 buffer 索引
    int startOffset;   // 起始 buffer 内偏移

    int endIndex;      // 末尾 buffer 索引
    int endOffset;     // 末尾 buffer 内偏移（指向下一个空位）

    int totalSize;

    BufferPtr newBuffer() {
        if (!freeList.empty()) {
            BufferPtr buf = freeList.back();
            freeList.pop_back();
            return buf;
        }
        return new Buffer(BUFFER_SIZE, T());
    }

    void recycleBuffer(BufferPtr buf) {
        freeList.push_back(buf);
    }

    void ensureFrontBuffer() {
        if (startOffset == 0) {
            if (startIndex == 0) {
                map.insert(map.begin(), newBuffer());
                ++endIndex;
                ++startIndex;
            }
            --startIndex;
            map[startIndex] = newBuffer();
            startOffset = BUFFER_SIZE;
        }
    }

    void ensureBackBuffer() {
        if (endOffset == BUFFER_SIZE) {
            if (endIndex + 1 >= map.size()) {
                map.push_back(newBuffer());
            } else if (!map[endIndex + 1]) {
                map[endIndex + 1] = newBuffer();
            }
            ++endIndex;
            endOffset = 0;
        }
    }

public:
    SimpleDeque()
        : startIndex(0), startOffset(0),
          endIndex(0), endOffset(0), totalSize(0)
    {
        map.push_back(newBuffer());
    }

    ~SimpleDeque() {
        for (auto buf : map) {
            if (buf) delete buf;
        }
        for (auto buf : freeList) {
            delete buf;
        }
    }

    void push_back(const T& value) {
        ensureBackBuffer();
        (*map[endIndex])[endOffset++] = value;
        ++totalSize;
    }

    void push_front(const T& value) {
        ensureFrontBuffer();
        (*map[startIndex])[--startOffset] = value;
        ++totalSize;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        ++startOffset;
        --totalSize;

        if (startOffset == BUFFER_SIZE) {
            recycleBuffer(map[startIndex]);
            map[startIndex] = nullptr;
            ++startIndex;
            startOffset = 0;
        }
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        --endOffset;
        --totalSize;

        if (endOffset == 0 && endIndex != startIndex) {
            recycleBuffer(map[endIndex]);
            map[endIndex] = nullptr;
            --endIndex;
            endOffset = BUFFER_SIZE;
        }
    }

    T& front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        return (*map[startIndex])[startOffset];
    }

    T& back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        return (*map[endIndex])[endOffset - 1];
    }

    T& operator[](int index) {
        if (index < 0 || index >= totalSize) {
            throw std::out_of_range("Index out of range");
        }
        int offset = startOffset + index;
        int bufIndex = startIndex + offset / BUFFER_SIZE;
        int bufOffset = offset % BUFFER_SIZE;
        return (*map[bufIndex])[bufOffset];
    }

    int size() const { return totalSize; }

    bool empty() const { return totalSize == 0; }

    void clear() {
        for (auto& buf : map) {
            if (buf) recycleBuffer(buf);
        }
        map.clear();
        startIndex = endIndex = 0;
        startOffset = endOffset = totalSize = 0;
        map.push_back(newBuffer());
    }

    void print() const {
        for (int i = 0; i < totalSize; ++i) {
            std::cout << (*this)[i] << " ";
        }
        std::cout << "\n";
    }
};
