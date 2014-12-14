#include <math.h>
#include <unistd.h>
#include <X11/Xlib.h>

#define WINDOW_SIZE 500
#define DEGREES_TO_RADIANS (3.141/180.0)

Display *display;
Window window;
Screen *screen;
GC gc;

double current_x, current_y, current_direction;
int pen_down;

void tortoise_reset()
{
    current_x = current_y = WINDOW_SIZE / 2;
    current_direction = 0.0;
    pen_down = 1;
}


void tortoise_pendown()
{
    pen_down = 1;
}


void tortoise_penup()
{
    pen_down = 0;
}


void tortoise_turn(int degrees)
{
    current_direction += (double)degrees;
}


void tortoise_move(int steps)
{
    double new_x, new_y;
    new_x = current_x + sin(current_direction * DEGREES_TO_RADIANS) * (double)steps;
    new_y = current_y - cos(current_direction * DEGREES_TO_RADIANS) * (double)steps;
    if (pen_down) 
        XDrawLine(display, window, gc, (int)current_x, (int)current_y,
                  (int)new_x, (int)new_y);
    current_x = new_x;
    current_y = new_y;
}


int main(int argc, const char *argv[])
{
    display = XOpenDisplay(NULL);
    XSynchronize(display, True);
    screen = DefaultScreenOfDisplay(display);
    window = XCreateSimpleWindow(display, RootWindowOfScreen(screen),
            0, 0, WINDOW_SIZE, WINDOW_SIZE, 0,
            BlackPixelOfScreen(screen), WhitePixelOfScreen(screen));
    gc = XCreateGC(display, window, 0L, NULL);
    /*XSetForeground(display, gc, BlackPixelOfScreen(screen));*/
    XSetForeground(display, gc, 0x000000L);
    XMapWindow(display, window);

    {
        int i;
        tortoise_pendown();
        for (i = 0; i < 4; i++) {
            tortoise_move(100);
            tortoise_turn(90.0);
        }
        sleep(5);
    }
    return 0;
}
