#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#define QUEUE_NAME "/my_queue"  // 消息队列的名称
#define MAX_SIZE 1024          // 消息的最大大小
#define MSG_STOP "exit"        // 用于终止的特殊消息

int main() {
    // 1. 创建或打开消息队列
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, NULL);
    if (mq == (mqd_t)-1) {
        std::cerr << "mq_open failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. 发送消息
    const char* msg1 = "Hello from producer!";
    if (mq_send(mq, msg1, strlen(msg1) + 1, 0) == -1) {
        std::cerr << "mq_send failed: " << strerror(errno) << std::endl;
        mq_close(mq);
        return 1;
    }

    const char* msg2 = "Another message";
    if (mq_send(mq, msg2, strlen(msg2) + 1, 0) == -1) {
        std::cerr << "mq_send failed: " << strerror(errno) << std::endl;
        mq_close(mq);
        return 1;
    }

    // 发送特殊消息来结束消费者
    if (mq_send(mq, MSG_STOP, strlen(MSG_STOP) + 1, 0) == -1) {
        std::cerr << "mq_send failed: " << strerror(errno) << std::endl;
        mq_close(mq);
        return 1;
    }

    std::cout << "Messages sent successfully." << std::endl;

    // 3. 关闭消息队列
    mq_close(mq);
    return 0;
}
