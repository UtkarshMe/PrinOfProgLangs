#include <stdio.h>
#include <unistd.h>

// this is a comment
int j;//fuck
void f(int a, int b)
{
    double x;
    int i = 0;

    /* this
     * is
     * a
     * multiline comment
     */

    while (i < 5)
    {
        int y;
        y = x / 10;
    }
}

void g() {
    int x;
    f(1, 2);
}
