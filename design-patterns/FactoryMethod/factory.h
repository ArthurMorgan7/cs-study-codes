#include <string>

class Product{
public:
    virtual void run(const char *msg) = 0;
};

class Factory{
private:

public:
    virtual Product* create() const = 0;
};
