
#include <stdint.h>
#include "rprintf.h"

#define MULTIBOOT2_HEADER_MAGIC         0xe85250d6

const unsigned int multiboot_header[]  __attribute__((section(".multiboot"))) = {MULTIBOOT2_HEADER_MAGIC, 0, 16, -(16+MULTIBOOT2_HEADER_MAGIC), 0, 12};

uint8_t inb (uint16_t _port) {
    uint8_t rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}
#define WIDTH 80
#define HEIGHT 25

struct termbuf{
 char ascii;
 char color;
};

int x = 0;
int y = 0;

void scroller(void) {
   struct termbuf *vram = (struct termbuf*)0xB8000;
   for ( int row = 1; row < HEIGHT; row++){
      for (int col = 0; col < WIDTH; col++){
          vram[(row - 1) * WIDTH + col] = vram[row * WIDTH + col];
      }
   }
  for (int col = 0; col < WIDTH; col++) {
       vram[(HEIGHT -1) * WIDTH + col].ascii = ' ';
       vram[(HEIGHT -1) * WIDTH + col].color = 7;
   }
 y = HEIGHT -1; 
 x = 0;
}
static void print_char(char c){
 struct termbuf *vram = (struct termbuf*)0xB8000;
 if (c == '\n') {
    x = 0;
    y++;
} else {
   vram[y * WIDTH + x].ascii = c;
   vram[y * WIDTH + x].color = 7;
   x++;
   if (x >= WIDTH){
    x = 0;
    y++;
  }
}
 if (y >= HEIGHT){
   scroller();
  }
}
static int kputc(int c) {
  print_char((char)c);
  return c;
}
void main(void) {
 for (int i = 0; i < 27; i++) {
    esp_printf(kputc, "Ethan\n", i);
 }
 while(1);
}
