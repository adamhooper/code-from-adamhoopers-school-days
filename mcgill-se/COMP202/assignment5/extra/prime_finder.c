#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_primes(long long n);
void print_usage(void);

int main(int argc, char **argv)
{
    long long n;

    if (argc != 2) {
        print_usage();
        return 0;
    }

    n = atoll(argv[1]);

    if (n < 2) {
        print_usage();
        return 1;
    }

    print_primes(n);
    return 0;
}

void print_primes(long long n)
{
    long long i, j;
    char *primes;
    long long rt = sqrt(n);

    primes = calloc(sizeof(char) * (n + 1), sizeof(char));

    if (!primes) {
        printf("Could not allocate memory. Aborting.\n");
        exit(1);
    }

    primes[0] = 0;
    primes[1] = 0;
    primes[2] = 1;

    /* Set all odd numbers to 1 (prime) */
    for (i = 3; i <= n; i++) {
        if (i & 1)
            primes[i] = 1;
    }

    /* Loop through, unsetting multiples of primes */
    for (i = 3; i <= rt; i += 2) {
        if (!primes[i])
            continue;

        for (j = i * i; j <= n; j += 2 * i)
            primes[j] = 0;
    }

    /* Print what's left */
    for (i = 0; i <= n; i++)
        if (primes[i])
            printf("%lld\n", i);

    free(primes);
}

void print_usage(void)
{
    printf("Usage: prime_finder [N]\n");
}
