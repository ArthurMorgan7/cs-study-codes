#include "context.h"

int main(){
    // 接口类
    Context context;
    // 接口类中的方法，在此之前要先设置接口类中的功能实体
    // context.pay(100);
    WechatPay wechat_pay;
    context.setStrategy(&wechat_pay);
    context.pay(100);

    Alipay ali_pay;
    context.setStrategy(&ali_pay);
    context.pay(200);
}