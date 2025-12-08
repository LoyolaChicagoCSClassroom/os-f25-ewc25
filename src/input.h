#ifndef INPUT_H
#define INPUT_H

char get_char(void);
void get_line(char *buf, int max);
void get_num_line(char *buf, int max);
void flush_keyboard(void);
int to_int(const char *s);

#endif
