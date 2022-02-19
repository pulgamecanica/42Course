#include <stdio.h>

int repeated(int i, int j, int k, int p, int q)
{
    if (i == j || i == k || i == p || i == q)
        return (1);
    if (j == i || j == k || j == p || j == q)
        return (1);
    if (k == j || k == i || k == p || k == q)
        return (1);
    if (p == j || p == k || p == i || p == q)
        return (1);
    if (q == j || q == k || q == p || q == i)
        return (1);
    return (0);
}

int main()
{
    static int i, j, k, p, q;
    
    for ( i = 0; i < 5; i++) {
        for ( j = 0; j < 5; j++) {
            for ( k = 0; k < 5; k++) {
                for ( p = 0; p < 5; p++) {
                    for ( q = 0; q < 5; q++) {
                        if (repeated(i, j, k, p, q))
                            continue ;
                        printf("%d %d %d %d %d\n", i, j, k, p, q);
                    }
                }
            }
        }
    }
    return 0;
}
