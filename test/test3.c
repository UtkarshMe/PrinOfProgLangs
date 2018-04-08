#include <unistd.h>

int sqr(int a)
{
    int x;
    x = a * a;
    return x;
}

int main(int argc, char *argv[])
{
    int num;

    /* this is a comment here */
    num = sqr(10); // this is a comment too

    int a = 10;
    no_scope = 20; // this will give an error

    int b = 10;
    int b = 20; // this will give an error too

    while (a < 30) {
        a++;
        int num; // this will not give error
        int b; // this will not give error

        int num = 10 // this will give error
    }

    return 0;
}
