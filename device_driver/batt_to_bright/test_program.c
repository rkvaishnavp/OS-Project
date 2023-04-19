#include <stdio.h>

int main()
{
    FILE *fp;

    fp = fopen("/sys/class/backlight/my_backlight/brightness", "w");
    if (fp == NULL) {
        perror("Failed to open brightness file");
        return 1;
    }

    fprintf(fp, "128\n"); // set brightness to 50%
    fclose(fp);

    return 0;
}
