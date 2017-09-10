#include <unistd.h>

int sqr(int a)
{
    a = a * a;
    return a;
}

int main(int argc, char *argv[])
{
    int num;

    /* this is a comment here */
    num = sqr(10); // this is a comment too

    // int a = 10;
    a = 20; // this will give an error

    return 0;
}
