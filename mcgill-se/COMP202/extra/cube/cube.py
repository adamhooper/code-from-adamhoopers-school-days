#!/usr/bin/env python

#Finds the number of occuranes of 'ABCD' in the given cube of integers.

import sys

class Cube:
    def __init__(self, filename):
        self.data = [[]]
        self.dimension = 0
        self.parse_cube_file(filename)

    def parse_cube_file(self, filename):
        """Parses the contents of the file into self.data. Does some rudimentary
           error checking but won't do anything about the errors."""
        depth = 0

        for line in file(filename).readlines():
            if not self.dimension:
                self.dimension = len(line) - 1

            if len(self.data[depth]) == self.dimension:
                if not line[0] == "-":
                    print "Parse error: No '-' found when needed."

                depth += 1

                self.data.append([])

                continue

            if not self.dimension == len(line) - 1:
                print "Parse error: some lines are longer than others."

            self.data[depth].append(list(line[0:-1]))

    def find_strings(self):
        """Returns the number of occurances of "ABCD" in 3D."""
        ret = 0

        for i in range(0, self.dimension):
            for j in range(0, self.dimension):
                for k in range(0, self.dimension):
                    if self.data[i][j][k] == "A":
                        ret += self.find_strings_from(i, j, k)

        return ret

    def find_strings_from(self, i, j, k):
        """Returns the number of "ABCD"s given the co-ordinate for an "A"."""
        ret = 0

        for rel_i in range(-1, 2):
            for rel_j in range(-1, 2):
                for rel_k in range(-1, 2):
		    # Do bounds-checking explicitly -- [-1] won't cause an error
		    if i+3*rel_i < 0 or \
		       i+3*rel_i > self.dimension-1 or \
		       j+3*rel_j < 0 or \
		       j+3*rel_j > self.dimension-1 or \
		       k+3*rel_k < 0 or\
		       k+3*rel_k > self.dimension-1:
			   continue

		    if self.data[i+1*rel_i][j+1*rel_j][k+1*rel_k] == "B" \
		       and \
		       self.data[i+2*rel_i][j+2*rel_j][k+2*rel_k] == "C" \
		       and \
		       self.data[i+3*rel_i][j+3*rel_j][k+3*rel_k] == "D":
		    
			ret += 1

        return ret

if len(sys.argv) != 2:
    print "Usage: %s [cube-file]" % sys.argv[0]
else:
    c = Cube(sys.argv[1])
    print 'Number of occurances of "ABCD": %d' % c.find_strings()
