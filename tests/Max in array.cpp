#include <stdio.h>

int maximum(int a[], int l) {
    int max = a[0];
    for (int i = 0; i < l; i++)
    {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int main()
{
    int a[5] = { 5,3,-1,8,2 };
    maximum(a, 5);

    return 0;
}