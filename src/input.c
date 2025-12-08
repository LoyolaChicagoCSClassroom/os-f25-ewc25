#include "input.h"
#include "rprintf.h"


static const char scancode_table[128] = {
    0,27,'1','2','3','4','5','6',
    '7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u',
    'i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k',
    'l',';','\'','`',0,'\\','z','x',
    'c','v','b','n','m',',','.','/',
    0,'*',0,' '
};

char read_scancode(void) {
    unsigned char sc;
    asm volatile("inb $0x60, %0" : "=a"(sc));
    return sc;
}

void flush_keyboard(void) {
    while (read_scancode() & 0x80) {}
}

char get_char(void)
{
    unsigned char s1 = read_scancode();
    unsigned char s2;

    do {
        s2 = read_scancode();
    } while (s1 == s2);

    if (s2 & 0x80) return 0;

    return scancode_table[s2];
}

void get_line(char *buf, int max)
{
    int i = 0;

    while (i < max - 1) {
        char c = get_char();
        if (!c) continue;

        if (c == '\n') {
            buf[i] = 0;
            kputc('\n');
            return;
        }

        if (c == '\b' && i > 0) {
            i--;
            kputc('\b'); kputc(' '); kputc('\b');
            continue;
        }

        if (c >= 32 && c <= 126) {
            buf[i++] = c;
            kputc(c);
        }
    }
    buf[i] = 0;
}

void get_num_line(char *buf, int max)
{
    int i = 0;

    while (i < max - 1) {
        char c = get_char();
        if (!c) continue;

        if (c == '\n') {
            buf[i] = 0;
            kputc('\n');
            return;
        }

        if (c == '\b' && i > 0) {
            i--;
            kputc('\b'); kputc(' '); kputc('\b');
            continue;
        }

        if (c >= '0' && c <= '9') {
            buf[i++] = c;
            kputc(c);
        }
    }
    buf[i] = 0;
}

int to_int(const char *s)
{
    int x = 0;
    while (*s) {
        x = x * 10 + (*s - '0');
        s++;
    }
    return x;
}
