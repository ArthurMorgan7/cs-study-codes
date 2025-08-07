

#pragma once

#include "strategy.h"

class Context{
private:
    Strategy *strategy_;
public:
    Context() = default;

    void setStrategy(Strategy *strategy){
        strategy_ = strategy;
    }

    void pay(int num){
        if(strategy_){
            strategy_->pay(num);
        }
    }

};

