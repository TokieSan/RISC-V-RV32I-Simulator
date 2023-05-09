int F(int N)
{
    int a = 0, b = 1, c, i;
    if (N == 0)
        return a;
    for (i = 2; i <= N; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main()
{
    int N = 5;

    cout << F(N);
    return 0;
}


//retrieved from: https://prepinsta.com/cpp-program/finding-the-nth-term-of-the-fibonacci-series/
