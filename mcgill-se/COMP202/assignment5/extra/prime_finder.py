#!/usr/bin/env python

from sys import argv

def print_primes(max):
    """Prints all primes up to the given integer."""
    max = int(max)

    if max < 2: max = 2

    # Build an array all the way to max.
    # That is, isPrime[4] is True if 4 is prime.
    isPrime = [True] * (max + 1)
    isPrime[0] = isPrime[1] = False

    # Cross off non-primes
    for i in range(2, max):
	j = i * 2
	while j <= max:
	    isPrime[j] = False
	    j += i

    # Print primes
    print "Prime numbers from 2 to %d:" % max
    for i in range(2, max + 1):
	if isPrime[i]:
	    print i

try:
    print_primes(argv[1])
except IndexError:
    print "No argument given!"
