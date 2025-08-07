#include <iostream>

// 旧系统接口
class OldSystem {
public:
    void oldRequest() {
        std::cout << "Old System: Handling old request" << std::endl;
    }
};

// 新系统接口
class NewSystem {
public:
    virtual void newRequest() = 0; // 新接口
};

// 适配器类
class Adapter : public NewSystem {
private:
    OldSystem* oldSystem;  // 适配器持有旧系统的实例

public:
    Adapter(OldSystem* system) : oldSystem(system) {}

    // 实现新系统接口，调用旧系统的 oldRequest()
    void newRequest() override {
        std::cout << "Adapter: Adapting old request to new request" << std::endl;
        oldSystem->oldRequest();  // 调用旧接口的方法
    }
};

// 客户端代码
void clientCode(NewSystem* system) {
    system->newRequest();
}

int main() {
    OldSystem* oldSystem = new OldSystem();
    // 通过适配器将旧系统适配为新系统
    Adapter* adapter = new Adapter(oldSystem);

    // 客户端调用新接口，实际由适配器转发到旧接口
    clientCode(adapter);

    delete oldSystem;
    delete adapter;
    return 0;
}
