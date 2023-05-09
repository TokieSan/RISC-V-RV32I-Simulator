#include <stdio.h>

bool stringequal(char x[], char y[], int n) {
    int i, c = 0;
    for (i = 0; i < n; i++)
        if (x[i] == y[i])
            c++;
    if (c == n)
        return true;
    return false;
}

int main()
{
    char x[6] = { 'a','b','d','o','r','e' };
    char y[6] = { 's','a','b','i','n','e' };
    cout << stringequal(x, y, 6);


    return 0;
}