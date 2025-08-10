#include <iostream>
#include <vector>
#include <list>
#include <functional>  // std::hash

template <typename T>
class UnorderedSet {
private:
    static const size_t INITIAL_BUCKET_COUNT = 8;
    static constexpr float LOAD_FACTOR_THRESHOLD = 0.75f;

    std::vector<std::list<T>> buckets;
    size_t _size;

    size_t getBucketIndex(const T& value) const {
        return std::hash<T>{}(value) % buckets.size();
    }

    void rehash() {
        size_t newBucketCount = buckets.size() * 2;
        std::vector<std::list<T>> newBuckets(newBucketCount);

        for (const auto& bucket : buckets) {
            for (const auto& val : bucket) {
                size_t newIndex = std::hash<T>{}(val) % newBucketCount;
                newBuckets[newIndex].push_back(val);
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    UnorderedSet() : buckets(INITIAL_BUCKET_COUNT), _size(0) {}

    bool insert(const T& value) {
        if (contains(value)) return false;

        if ((float)(_size + 1) / buckets.size() > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

        size_t index = getBucketIndex(value);
        buckets[index].push_back(value);
        ++_size;
        return true;
    }

    bool contains(const T& value) const {
        size_t index = getBucketIndex(value);
        for (const auto& val : buckets[index]) {
            if (val == value) return true;
        }
        return false;
    }

    bool erase(const T& value) {
        size_t index = getBucketIndex(value);
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (*it == value) {
                bucket.erase(it);
                --_size;
                return true;
            }
        }
        return false;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void debug_print() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& val : buckets[i]) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
};
int main() {
    UnorderedSet<int> s;

    s.insert(5);
    s.insert(10);
    s.insert(20);
    s.insert(15);
    s.insert(25);

    std::cout << "Contains 10? " << s.contains(10) << "\n"; // 1
    std::cout << "Contains 99? " << s.contains(99) << "\n"; // 0

    s.erase(10);
    std::cout << "Contains 10 after erase? " << s.contains(10) << "\n"; // 0

    std::cout << "Size: " << s.size() << "\n";

    s.debug_print();
}
