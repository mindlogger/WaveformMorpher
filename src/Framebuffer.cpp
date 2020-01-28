#include "Framebuffer.hpp"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

char *buffer = NULL;

void setPixel(int x, int y)//480 x 320
{
    x = x * 958/480;
    y = y - 1;
    *(buffer + x + y * 960) = 0xFF;
    *(buffer + x + 1 + y * 960) = 0xFF;
}

void setPixelOff(int x, int y)//480 x 320
{
    x = x * 958/480;
    y = y - 1;
    *(buffer + x + y * 960) = 0x00;
    *(buffer + x + 1 + y * 960) = 0x00;
}

void initFramebuffer(int *wave_table)
{
    struct fb_var_screeninfo screen_info;
    struct fb_fix_screeninfo fixed_info;

    size_t buflen;
    int fd = -1;
    int r = 1;

    fd = open("/dev/fb1", O_RDWR);
    system("xinput disable stmpe-ts");

    if (fd >= 0)
    {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info) &&
                !ioctl(fd, FBIOGET_FSCREENINFO, &fixed_info))
        {
            buflen = screen_info.yres_virtual * fixed_info.line_length;
            buffer = (char*) mmap(NULL,
                          buflen,
                          PROT_READ|PROT_WRITE,
                          MAP_SHARED,
                          fd,
                          0);
            if (buffer != MAP_FAILED)
            {

                for(size_t i = 0;i < buflen-1;i++)//SET ARRAY TO ZERO //MAX VAL FOR i = //307199
                {
                    buffer[i] = 0x00;
                }

                /*for(size_t i = 0;i < 480;i++)//FILL ARRAY WITH LINE AT 160
                {
                    setPixel(i,160);
                    wave_table[i] = 160;
                }*/

                //setPixel(480,320,buffer); //THIS SHOULD BE THE BOTTOM RIGHT CORNER PIXEL

                for(size_t i = 1;i<480;i++)//DRAW SINE
                {
                int x = (145+(int)(140*sin(2*3.14*i/480)));
                setPixel(i,x);
                wave_table[i] = x;
                }

                /*
                 * TODO: something interesting here.
                 * "buffer" now points to screen pixels.
                 * Each individual pixel might be at:
                 *    buffer + x * screen_info.bits_per_pixel/8
                 *           + y * fixed_info.line_length
                 * Then you can write pixels at locations such as that.
                 */

                r = 0;   /* Indicate success */
            }
            else
            {
                perror("mmap");
            }
        }
        else
        {
            perror("ioctl");
        }
    }
    else
    {
        perror("open");
    }
    if (buffer && buffer != MAP_FAILED)
        //munmap(buffer, buflen); //THIS LINE DESTROYS BASICALLY EVERYTHING
    if (fd >= 0)
        close(fd);

    //return r;
}
