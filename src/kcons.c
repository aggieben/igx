#include <igx.h>
#include <kcons.h>
#include <stdarg.h>

byte *_kc_mem;
byte _kc_color=0;
short cursor;
byte page_buf[4][4000];

/*!\brief Kernel console initializer.
 *
 * Initializes the console for the kernel's use until a proper console
 * device can be initialized. 
 */
void _kcons_init()
{
  _kc_mem = (byte*)_K_VIDMEM;
  _kc_color = BLACK;
  _kc_color << 8;
  _kc_color += WHITE;
  kclrscr();
}

/*!\brief Kernel printf function
 *
 * This function is to be used much as a normal printf would be;
 * however, it will certainly not support all the same conversions
 * that the GNU printf supports --- not even close.  It basically just
 * prints one character at a time from a string, and if anything, it
 * is a convenience function to prevent developers from having to put
 * for/while loops in the kernel for printing debug info.
 */
void kprintf(const char *s,...)
{
  va_list ap;
  char lc=0;
  const char *p=s;
  ulong u;
  byte b;
  int j;
  short mask = 0xf;
  int leading=1;
  ulong m=0x100000;

  va_start(ap,s);
  while(*p)
  {
    if(*p=='%')
    {
      switch(*(++p))
      {
      case 'x':
	putchar('0');
	putchar('x');
	u = va_arg(ap,ulong);
	if(0==u)
	{
	  putchar('0');
	  p++;
	  break;
	}
	for(j=sizeof(ulong)*2;j>0;j--)
	{
	  b = (byte)(u>>(j*4-4)) & mask;
	  if(0==b && leading);
	  else{
	    putchar((b>=10)?'a'+b-10:'0'+b);
	    leading=0;
	    p++;
	  }
	}
	break;
      case 'u':
	u = va_arg(ap,ulong);
	if(0==u)
	{
	  putchar('0');
	  p++;
	  break;
	}
	while(m>1)
	{
	  b = (byte)(u/m);
	  u %= m;
	  m /= 10;
	  if(0==b && leading);
	  else{
	    putchar('0'+b);
	    leading=0;
	    p++;
	  }
	}
	putchar('0' + u%10);
	break;
      } /* end switch */
    } else {
      putchar(*p++);
    }
  }
  va_end(ap);
}

/*!\brief Print character to the kernel console
 *
 * This function simply prints a value to the screen, and takes
 * special action for particular characters like '\n'.  The
 * interaction between this function and kprintf aren't totally
 * concrete yet; in the end, there will probably be more
 * responsibility for conversions in the kprintf() function than here.
 */
void putchar(const char lc)
{
  if( (cursor==2000) || (cursor >= 1920 && lc == '\n'))
  {
    kscroll(1);
    cursor = 1920;
  } else {
    switch(lc)
    {
    case '\n':
      cursor += 80 - (cursor%80);
      break;
    default:
      *(_kc_mem+(cursor*2)) = lc;
      //      *(kc.vmem+(kc.c*2)) >>= 4;
      *(_kc_mem+(cursor*2+1)) += _kc_color;
      cursor++;
    }  
  }
}

/*!\brief Scroll screen up by 'lines' lines.
 *
 * Simply copies the ith line to the (i-1)th line, overwriting the top
 * line and blanking the bottom line. A simple way to clear the screen
 * could be to kcscroll(25), however, it would be far more efficient
 * to write a separate clearscreen function that would write zeros
 * directly.
 */
void kscroll(short lines)
{
  short i,j,k;

  for(k=0;k<80;k++){
    *(_kc_mem+3840+(k*2)) = 0;
    *(_kc_mem+3840+(k*2+1)) = _kc_color;
  }

  for(i=0;i<lines;i++)
  {
    for(j=1;j<25;j++)
    {
      bcopy(_kc_mem+(j*160),_kc_mem+((j-1)*160),160);
    }
  }
}

/*!\brief Clears the kernel console.
 *
 * Clears the terminal by using the kcscroll() function.  Needs to be
 * independent of kcscroll() so that it will be more efficient.
 *
 * 1/24/2004: actually, kcscroll() would be better.  It might even be
 * more efficient, since bcopy uses rep/mov, but it would primarily be
 * better because it would make more sense for keeping console
 * history.
 */
void kclrscr()
{
  short i,j;
  for(i=0;i<25;i++)
  {
    for(j=0;j<80;j++)
    {
      *(_kc_mem+(160*i)+(j*2)) = 0;
      *(_kc_mem+(160*i)+(j*2)+1) = 0;
    }
  }
  cursor = 0;
}

/*
char *tostring(ulong i, int r)
{
  short index = 0, mask=0xf;
  ulong m = 1000000000;
  byte c = 0;
  byte leading = 1;
  int j;
  static char buf[20];

  switch(r)
  {
  case 16:
    buf[0] = '0';
    buf[1] = 'x';
    index = 2;

    for(j=sizeof(ulong)*2;j>0;j--)
    {
      cursor = (byte)(i>>(j*4-4)) & mask;
      if(0==cursor && leading);
      else{
	buf[index++] = (cursor>=10)?'a'+cursor-10:'0'+cursor;
	leading = 0;
      }
    }
    break;
  case 10:
    while(m>1)
    {
      cursor = (byte)(i/m);
      i %= m;
      m /= 10;
      if(0==cursor && leading);
      else{
	buf[index++] = '0'+cursor;
	leading = 0;
      }
    }
    buf[index++] = '0' + i%10;
  }

  buf[index] = 0;

  return buf;
}
*/

  
