#include <iostream>

using namespace std;




/* ------------------------------ 18.3 多重继承与虚继承 ----------------------------- */

void homework_18_28(){
    // 在局部作用域内{} 定义的结构体类型，其作用域也会在局部作用域内
    struct Base{
        void bar(int);
    protected:
        int ival;
    };

    struct Derived1 : virtual public Base{
        void bar(char); // 覆盖
        void foo(char);
    protected:
        char cval;
    };

    struct Derived2 : virtual public Base{
        void foo(int);
    protected:
        int ival;
        char cval;
    };

    class VMI : public Derived1, public Derived2 {
    };

}

int main(){
    homework_18_28();
}