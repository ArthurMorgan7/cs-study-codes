#pragma once

#include <iostream>

// 抽象基类
class Strategy{
public:
    virtual void pay(int num) = 0;
};

class WechatPay : public Strategy{
public:
    virtual void pay(int num) override{
        std::cout << "Wechat pay " << num << " money!" << std::endl;
    }
};

class Alipay : public Strategy{
public:
    virtual void pay(int num) override{
        std::cout << "Alipay pay " << num << " money!" << std::endl;
    }
};