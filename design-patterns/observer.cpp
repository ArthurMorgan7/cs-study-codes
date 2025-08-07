#include <iostream>
#include <vector>
#include <string>

/* ---------------------------------- 抽象观察者 --------------------------------- */
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(const std::string& message) = 0;
};



// 具体观察者类（ConcreteObserverA）
class ConcreteObserverA : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << "ConcreteObserverA received message: " << message << std::endl;
    }
};

// 具体观察者类（ConcreteObserverB）
class ConcreteObserverB : public Observer {
public:
    void update(const std::string& message) override {
        std::cout << "ConcreteObserverB received message: " << message << std::endl;
    }
};


/* ----------------------------------- 主题 ----------------------------------- */

// 主题类（Subject）
class Subject {
public:
    virtual ~Subject() {}
    // 添加观察者
    virtual void addObserver(Observer* observer) = 0;
    // 删除观察者
    virtual void removeObserver(Observer* observer) = 0;
    // 通知观察者
    virtual void notifyObservers() = 0;
};

// 具体主题类（ConcreteSubject）
class ConcreteSubject : public Subject {
private:
    std::vector<Observer*> observers;
    std::string state;

public:
    // 改变状态
    void setState(const std::string& newState) {
        state = newState;
        notifyObservers();
    }

    // 获取当前状态
    std::string getState() const {
        return state;
    }

    // 添加观察者（重写）
    void addObserver(Observer* observer) override {
        observers.push_back(observer);
    }

    // 删除观察者（重写）
    void removeObserver(Observer* observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // 通知观察者（重写）
    void notifyObservers() override {
        for (Observer* observer : observers) {
            observer->update(state);
        }
    }
};

/* ---------------------------------- main ---------------------------------- */

int main() {
    // 创建主题对象
    ConcreteSubject subject;

    // 创建观察者对象
    ConcreteObserverA observerA;
    ConcreteObserverB observerB;

    // 注册观察者
    subject.addObserver(&observerA);
    subject.addObserver(&observerB);

    // 改变主题的状态
    subject.setState("New State 1");
    subject.setState("New State 2");

    // 注销观察者
    subject.removeObserver(&observerB);

    // 改变状态，观察者B不会接收到通知
    subject.setState("New State 3");

    return 0;
}
