#include <stdio.h>

int main()
{
    int n=10000,thread_count=4,i;
    int a[n];
#   pragma omp parallel for num_threads(thread_count)\
                              default(none) private(i) shared(a, n)
    for(i = 0;i < n;i++)
        a[i] = i*(i+1)/2;

    printf("%d\n",a[n-1]);
    return 0;
}

