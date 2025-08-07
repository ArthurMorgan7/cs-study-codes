#include <iostream>
#include <memory>

// 产品接口
class Logger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~Logger() = default;
};

// 具体产品 1：文件日志记录器
class FileLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << "FileLogger: " << message << std::endl;
    }
};

// 具体产品 2：控制台日志记录器
class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << "ConsoleLogger: " << message << std::endl;
    }
};

// 工厂接口
class LoggerFactory {
public:
    virtual std::unique_ptr<Logger> createLogger() const = 0;
    virtual ~LoggerFactory() = default;
};

// 具体工厂 1：文件日志工厂
class FileLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<Logger> createLogger() const override {
        return std::make_unique<FileLogger>();
    }
};

// 具体工厂 2：控制台日志工厂
class ConsoleLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<Logger> createLogger() const override {
        return std::make_unique<ConsoleLogger>();
    }
};

