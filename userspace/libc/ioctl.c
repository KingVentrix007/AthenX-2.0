#include "ioctl.h"

int my_ioctl(int fd, unsigned long request, struct winsize *ws) {
    if (request == TIOCGWINSZ) {
        // Fill in the dummy window size values.
        ws->ws_row = get_screen_height(); // Implement get_screen_height() as needed.
        ws->ws_col = get_screen_width();  // Implement get_screen_width() as needed.
        return 0;
    } else {
        return -1; // Unsupported request.
    }
}

int get_screen_width()
{
    return 135;
}
int get_screen_hight()
{
    return 48;
}