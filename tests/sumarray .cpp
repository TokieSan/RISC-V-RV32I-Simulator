#include <stdio.h>

int sum(int a[], int l) {
    int sum = 0;
    int i = 0;
    while (i < l)
    {
        sum += a[i];
        i = i + 1;
    }
    return sum;
}

int main()
{
    int a[5] = { 6,8,9,3,2 };
    sum(a, 5);

    return 0;
}