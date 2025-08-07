
using namespace std;


class SmallInt{
    friend double operator+(const SmallInt& a, const SmallInt& b);
    private:
        int val;
    public:
        explicit SmallInt(int x = 0):val(x){}

        explicit operator int() const{
            return val;
        }
};

double operator+(const SmallInt& a, const SmallInt& b){
    return a.val + b.val;
}

void homework_14_53(){
    SmallInt s1;
    // double d = s1 + 3.14;
    double d = s1 + SmallInt(3.14);

}

int main(){
    homework_14_53();   
}