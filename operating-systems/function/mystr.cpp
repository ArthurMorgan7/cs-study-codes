#include <stdio.h>
#include <stddef.h>  // size_t
#include <string.h>  // for memmove/memcpy in safe example (optional)

size_t myStrlen(const char *s) {
    if (!str) return 0; // 可选：防御性处理（标准strlen对NULL是未定义的）
    size_t len = 0;
    while (*str){
        ++len;
    }
    return len;
}


char* myStrcy(char *dst, const char *src){
    char *p = dst;
    // '/0' 的 ASCII码为0
    while(*src){    
        *p = *src;
        ++p;
        ++src;
    }
    return dst;
}

size_t my_strlcpy(char *dst, const char *src, size_t dstsize) {
    size_t srclen = my_strlen(src);
    if (dstsize == 0) {
        return srclen;
    }
    size_t copylen = (srclen >= dstsize) ? dstsize - 1 : srclen;
    /* 使用简单循环拷贝（避免依赖 memcpy） */
    for (size_t i = 0; i < copylen; ++i) dst[i] = src[i];
    dst[copylen] = '\0';
    return srclen;
}

/* 测试 */
int main(void) {
    const char *s = "Hello, world!";
    printf("my_strlen(\"%s\") = %zu\n", s, my_strlen(s));

    char buf1[32];
    my_strcpy(buf1, s);
    printf("after my_strcpy: \"%s\"\n", buf1);

    char small[6];
    size_t src_len = my_strlcpy(small, s, sizeof(small));
    printf("after my_strlcpy into size=%zu: \"%s\" (src_len=%zu, truncated=%s)\n",
           sizeof(small), small, src_len, (src_len >= sizeof(small) ? "YES" : "NO"));

    return 0;
}
