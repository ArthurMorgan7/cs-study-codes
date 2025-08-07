#include <iostream>

// 抽象产品：按钮
class Button {
public:
    virtual void render() = 0;
    virtual void onClick() = 0;
    virtual ~Button() = default;
};

// 抽象产品：文本框
class TextBox {
public:
    virtual void render() = 0;
    virtual void onInput() = 0;
    virtual ~TextBox() = default;
};

// 具体产品：Windows 按钮
class WindowsButton : public Button {
public:
    void render() override {
        std::cout << "Rendering a button in Windows style." << std::endl;
    }

    void onClick() override {
        std::cout << "Windows button clicked!" << std::endl;
    }
};

// 具体产品：Windows 文本框
class WindowsTextBox : public TextBox {
public:
    void render() override {
        std::cout << "Rendering a text box in Windows style." << std::endl;
    }

    void onInput() override {
        std::cout << "Typing in Windows text box." << std::endl;
    }
};

// 具体产品：macOS 按钮
class MacOSButton : public Button {
public:
    void render() override {
        std::cout << "Rendering a button in macOS style." << std::endl;
    }

    void onClick() override {
        std::cout << "macOS button clicked!" << std::endl;
    }
};

// 具体产品：macOS 文本框
class MacOSTextBox : public TextBox {
public:
    void render() override {
        std::cout << "Rendering a text box in macOS style." << std::endl;
    }

    void onInput() override {
        std::cout << "Typing in macOS text box." << std::endl;
    }
};

// 抽象工厂
class GUIFactory {
public:
    virtual Button* createButton() = 0;
    virtual TextBox* createTextBox() = 0;
    virtual ~GUIFactory() = default;
};

// 具体工厂：Windows 工厂
class WindowsFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new WindowsButton();
    }

    TextBox* createTextBox() override {
        return new WindowsTextBox();
    }
};

// 具体工厂：macOS 工厂
class MacOSFactory : public GUIFactory {
public:
    Button* createButton() override {
        return new MacOSButton();
    }

    TextBox* createTextBox() override {
        return new MacOSTextBox();
    }
};

// 客户端函数：使用工厂创建和操作产品
void useFactory(GUIFactory* factory) {
    Button* button = factory->createButton();
    TextBox* textBox = factory->createTextBox();

    button->render();
    button->onClick();

    textBox->render();
    textBox->onInput();

    delete button;
    delete textBox;
}

// 客户端代码
int main() {
    // 假设用户的操作系统是 Windows
    GUIFactory* windowsFactory = new WindowsFactory();
    std::cout << "Using Windows Factory:" << std::endl;
    useFactory(windowsFactory);  // 使用Windows工厂
    delete windowsFactory;

    // 假设用户的操作系统是 macOS
    GUIFactory* macOSFactory = new MacOSFactory();
    std::cout << "\nUsing macOS Factory:" << std::endl;
    useFactory(macOSFactory);  // 使用macOS工厂
    delete macOSFactory;

    return 0;
}
