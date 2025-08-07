#include <iostream>

#include "factory.h"



class FileLogger : public Product{
public:
    virtual void run(const char *msg) override{
        std::cout << "File Logger has logged the msg: " << msg << std::endl;
    }

};


class FileLoggerFactory : public Factory{
public:
    virtual Product *create() const override{
        return new FileLogger;
    }
};