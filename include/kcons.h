/*!\file kons.h
 * Kernel console header.
 *
 * This header contains some definitions and global variables and
 * function prototypes necessary for the kernel console.
 */

/*!\struct struct kons
  !\brief The data type that represents the kernel console.

  It keeps track of the location of the video memory, keeps a brief
  terminal history (for Shift-Pageup, if I ever get around to it), and
  the current cursor position and color scheme.
*/

/*!\var kons kc
  !\brief a global instance of the kernel console. 

  There are no plans to have more than one console for the
  kernel. Once the kernel is completely loaded and processes are
  running, multiple consoles will be allowed; they will certainly be
  more fully featured. This console is basically for debugging
  purposes and so that the kernel can reassure us that it is working
  as it should.
*/

/*!\fn void kcinit()
  !\brief Initialize kernel console.
*/

/*!\fn void kputchar(const char c)
  !\param c The character to pu.
  !\brief Writes characters to the kernel console.
*/

/*!\fn void kcscroll(short lines)
  !\param lines The number of lines to scroll.
  !\brief Scrolls the kernel console by 'lines' lines.
*/

/*\fn void kcclrscr()
  !\brief Clears the kernel console.
*/

#include <igx.h>

#ifndef __KCONS__
#define __KCONS__

#define _K_VIDMEM 0xb8000
extern byte* _kc_mem;
extern byte _kc_color;
extern short _kc_cursor;
extern byte page_buf[4][4000];

/* colors / styles */
#define BLACK      0x0;
#define BLUE       0x1;
#define GREEN      0x2;
#define CYAN       0x3;
#define RED        0x4;
#define MAGENTA    0x5;
#define BROWN      0x6;
#define WHITE      0x7;
#define GRAY       0x8;
#define LBLUE      0x9;
#define LGREEN     0xa;
#define LCYAN      0xb;
#define LRED       0xc;
#define LMAGENTA   0xd; 
#define YELLOW     0xe;
#define IWHITE     0xf;

void putchar(const char lc);
void kclrscr();
void kscroll(short lines);
void kprintf(const char *s,...);

#endif
