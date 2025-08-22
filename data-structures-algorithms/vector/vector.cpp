#include <stdlib.h>
#include <iostream>
#include <cmath>


using namespace std;

template <typename T>
class Vector{
private:
    T* ptr_;
    size_t size_ = 0;
    size_t capacity_ = 1;
    
public:
    Vector(){
        ptr_ = new T(capacity_);
    }

    Vector(size_t n){
        capacity_ = n;
        ptr_ = new T(capacity_);
    }

    Vector(size_t n, T val){
        capacity_ = n;
        size_ = n;
        ptr_ = new T(capacity_);
        for(int i = 0; i < n; i++){
            *(ptr_ + i) = val;
        }
    }

    size_t size() const{ return size_; }

    size_t capacity() const { return capacity_; }

    T& operator[](size_t i){
        if(i >= capacity_){
            throw out_of_range("访问越界");
        }
        return *(ptr_ + i);     
    }

    void push_back(T val){
        if(size_ == capacity_){
            resize(ceil(1.5 * (float)capacity_));
        }
        *(ptr_ + size_) = val;
        ++size_;
    }


    void resize(size_t n){
        if(n > capacity_){
            T* ptr_new = new T(n);
            capacity_ = n;
            for(int i = 0; i < size_; i++){
                *(ptr_new + i) = *(ptr_ + i);
            }
            free(ptr_);
            ptr_ = ptr_new;
        }
        else{
            size_ = n;
        }
    }
};


int main(){
    // Vector<int> vec(10);
    // cout << vec.size() << endl;
    // cout << vec.capacity() << endl;

    // for(int i = 0; i < 10; i++){
    //     vec[i] = i;
    //     cout << vec[i];
    // }


    Vector<int> vec;
    for(int i = 0; i < 10; i++){
        vec.push_back(i);
        cout << vec[i];
    }

    cout << endl;
    return 0;
}

