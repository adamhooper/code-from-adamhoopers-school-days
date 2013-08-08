#!/usr/bin/env python

# This is one of my first functional Python programs. It will encode a string
# with the given padding (number of digits per letter) and base (number system).
#
# For example, "trans.py encode 8 2" will return the string passed through stdin
# as an 8-character-per-byte, base-2 number (i.e., binary).
# "trans.py encode 2 16" will return it as hex.
#
# Decoding works too, of course. If you encode and decode with the same options,
# you will receive your original message.

import string
import sys

def base_digits(base):
    """Returns a string of all possible digits of this base"""
    digits = ''
    for i in range(0, base):
	if i < 10:
	    digits += string.digits[i]
	else:
	    digits += string.ascii_uppercase[i-10]
    return digits

def num_to_chr(s, padding, base):
    """Takes a padding-digit number s of base b, returns a character"""
    digits = base_digits(base)

    ret = 0
    for i in range(1, padding + 1):
	index = string.find(digits, s[-i])
	ret += index * base**(i-1)

    return chr(ret)

def chr_to_num(c, padding, base):
    """Takes a character c and encodes it, returning a string of digits"""
    digits = base_digits(base)
    decimal_num = ord(c)

    ret = ''
    for i in range(0, padding):
	for j in range(1, base+1):
	    val = j*base**(padding-i-1)
	    if not (decimal_num / val):
		decimal_num %= base**(padding-i-1)
		break
	j -= 1
	ret += digits[j]
    return ret

def code_to_ascii(s, padding, base):
    """Takes a string of digits, returns a string of characters"""
    ret = ''
    for i in range(0, len(s), padding):
	ret += num_to_chr(s[i:i+padding], padding, base)
    return ret

def ascii_to_code(s, padding, base):
    """Takes a string of ASCII text, returns an encoded string"""
    ret = ''
    for i in range(0, len(s)):
	ret += chr_to_num(s[i], padding, base) + ' '
    return ret[:-1] # Take off the final space

def strip_garbage(s, padding, base):
    """Eliminates extra characters from a string"""
    allowed = base_digits(base)

    ret = ''
    for i in range(0, len(s)):
	if s[i] in allowed:
	    ret += s[i]
    return ret

def decode(s, padding, base):
    """Prints a decoded string"""
    s = strip_garbage(s, padding, base)
    print code_to_ascii(s, padding, base)

def encode(s, padding, base):
    """Encodes a string"""
    print ascii_to_code(s, padding, base)

if not len(sys.argv) == 4:
    raise SystemExit('Usage: ' + sys.argv[0] +
	             ' [decode|encode] [padding] [base]')

decoding = (sys.argv[1] == 'decode')
padding = string.atoi(sys.argv[2])
base = string.atoi(sys.argv[3])

fulltext = ''
for line in sys.stdin.readlines():
    fulltext += line[:-1]

if decoding: decode(fulltext, padding, base)
else: encode(fulltext, padding, base)
