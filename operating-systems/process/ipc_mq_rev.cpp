#include <iostream>
#include <mqueue.h> // 消息队列
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#define QUEUE_NAME "/my_queue"  // 消息队列的名称
#define MSG_STOP "exit"        // 用于终止的特殊消息

int main() {
    // 1. 打开消息队列
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0777, NULL);
    if (mq == (mqd_t)-1) {
        std::cerr << "mq_open failed: " << strerror(errno) << std::endl;
        return 1;
    }
    // 获取消息队列的属性
    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        std::cerr << "mq_getattr failed: " << strerror(errno) << std::endl;
        mq_close(mq);
        return 1;
    }

    // 输出消息队列的最大消息大小
    std::cout << "Max message size: " << attr.mq_msgsize << std::endl;
    char* buffer = new char[attr.mq_msgsize];


    while (true) {
        // 2. 从消息队列接收消息
        ssize_t bytes_read = mq_receive(mq, buffer, attr.mq_msgsize, NULL);
        if (bytes_read == -1) {
            std::cerr << "mq_receive failed: " << strerror(errno) << std::endl;
            mq_close(mq);
            return 1;
        }

        // 输出接收到的消息
        std::cout << "Received message: " << buffer << std::endl;

        // 如果接收到退出消息，结束循环
        if (strcmp(buffer, MSG_STOP) == 0) {
            break;
        }
    }

    // 3. 关闭消息队列
    mq_close(mq);

    // 4. 删除消息队列
    mq_unlink(QUEUE_NAME);

    return 0;
}
