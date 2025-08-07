
#include "fileLoggerFactory.h"



// 客户端代码
void clientCode(const Factory& factory) {
    auto logger = factory.create();

    // 主要任务：记录日志
    logger->run("Hello, Factory Method Pattern!");
}




int main() {
    // 文件记录
    FileLoggerFactory fileLoggerFactory;
    clientCode(fileLoggerFactory);

    // // 控制台记录
    // ConsoleLoggerFactory consoleLoggerFactory;
    // clientCode(consoleLoggerFactory);

    return 0;
}


