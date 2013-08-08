#!/usr/bin/env python

import commands

tests = [[12345678900987654321, 92837492036573827837],
	 [1, 1],
	 [99999999999999999999, 0],
	 [1, 99999999999999999999],
	 [55555555554444444444, 44444444445555555555],
	 [55555555554444444444, 44444444445555555556],
	 [99999999999999999999, 99999999999999999999],
	 [15, 4],
	 [1, 1],
	 [67236346282364, 99127361237881],
	]

for test in tests:
	a = test[0]
	b = test[1]

	expected_result = str(a + b)

	cmd = "./q1.exe %d %d" % (a, b)

	# "%s + %s = %s"
	real_result = commands.getoutput(cmd)

	# Change that to a single string
	real_result = real_result.split()[-1]

	if (real_result == expected_result):
		print "Success adding %d and %d" % (a, b)
	else:
		print "FAILURE adding %d and %d" % (a, b)
