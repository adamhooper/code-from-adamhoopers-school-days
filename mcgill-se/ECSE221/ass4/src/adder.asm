	.text

#####
# addBCD: add two arbitrary-length string representations of decimal numbers
#
# Assigned registers:
#	$a0: Pointer to first null-terminated string
#	$a1: Pointer to second null-terminated string
#	$a3: output
#	$t0: "moving" pointer over $a0
#	$t1: "moving" pointer over $a1
#	$t3: "moving" pointer over $a3
#	$t4: Length of result (possibly overestimated by 1)
#	$t5: carry (either 0 or 1)
#	$t6: sum (between 0 and 18)
#	$t7: scratch
#	$t8: scratch
#####

addBCD:

#####
# Save context of caller
#####

	addi	$sp, $sp, -28		# Allocate space on stack
	sw	$t0, 0($sp)
	sw	$t1, 4($sp)
	sw	$t3, 8($sp)
	sw	$t4, 12($sp)
	sw	$t5, 16($sp)
	sw	$t6, 20($sp)
	sw	$t7, 24($sp)

#####
# Main procedure body
#####

# Move $t0 to end of $a0
	move	$t0, $a0
move_t0:
	addi	$t0, 1			# Move forward a byte
	lb	$t7, 0($t0)		# Load the character we're pointing to
	bnez	$t7, move_t0		# If it's not null, move forward again

# Move $t1 to end of $a1
	move	$t1, $a1
move_t1:
	addi	$t1, 1			# Move forward a byte
	lb	$t7, 0($t1)		# Load the character we're pointing to
	bnez	$t7, move_t1		# If it's not null, move forward again

# Calculate result length in $t4
	sub	$t7, $t0, $a0		# $t7: Length of $a0 string
	sub	$t8, $t1, $a1		# $t8: Length of $a1 string
	move	$t4, $t7		# Assume $t7 is the largest
	bgt	$t7, $t8, len_done	# If it's not...
	move	$t4, $t8		# ... then $t8 is the largest
len_done:
	addi	$t4, $t4, 1		# Add 1: potential carried "1" of result

# Allocate memory for $a3, the result, using sbrk system call
	move	$t7, $a0		# Save $a0, we need it for syscall
	addi	$a0, $t4, 1		# We need length + 1, for NULL character
	li	$v0, 9			# sbrk is 9
	syscall
	move	$a3, $v0		# Store our result right away
	move	$a0, $t7		# Restore $a0

# Set $t3 to end of $a3
	add	$t3, $a3, $t4		# Add length of result to $a3
	sb	$0, 0($t3)		# Set final character to NULL
	addi	$t3, $t3, -1		# Point to previous character

# Loop backwards across $a3 (with $t3), adding $t0/$t1 characters
	li	$t5, 0			# carry = 0
a3_loop_backwards:
	li	$t6, 0			# sum = 0

	beqz	$t5, skip_carry		# if carry != 0 (it'll be 1)
	addi	$t6, $t6, 1		# sum += 1
	li	$t5, 0			# carry = 0
skip_carry:

	addi	$t0, $t0, -1		# Move left along $a0
	blt	$t0, $a0, skip_t0	# If we're done with $a0, skip it
	lb	$t7, 0($t0)		# Load that into scratch
	addi	$t7, $t7, -0x30		# Subtract '0'
	add	$t6, $t6, $t7		# Add it to sum
skip_t0:

	addi	$t1, $t1, -1		# Move left along $a1
	blt	$t1, $a1, skip_t1	# If we're done with $a1, skip it
	lb	$t7, 0($t1)		# Load that into scratch
	addi	$t7, $t7, -0x30		# Subtract '0'
	add	$t6, $t6, $t7		# Add it to sum
skip_t1:

	li	$t7, 10
	blt	$t6, $t7, skip_carry_1	# If sum < 10, don't add to carry
	li	$t5, 1			# carry = 1
	addi	$t6, $t6, -10		# sum -= 10
skip_carry_1:

	addi	$t6, $t6, 0x30		# sum += '0'
	sb	$t6, 0($t3)		# Write that to the digit of $a3

	addi	$t3, $t3, -1		# Decrement return value pointer
	bge	$t3, $a3, a3_loop_backwards
					# Loop until t3 < a3

#####
# Restore context
#####

	lw	$t0, 0($sp)
	lw	$t1, 4($sp)
	lw	$t3, 8($sp)
	lw	$t4, 12($sp)
	lw	$t5, 16($sp)
	lw	$t6, 20($sp)
	lw	$t7, 24($sp)
	addi	$sp, $sp, 28

	jr $ra				# Return

#####
# Test Program
#
# Ask for two decimals, call addBCD, and print the result
#
# Assigned registers:
#	$a0: Pointer to first number
#	$a1: Pointer to second number
#	$a3: Result
#	$t0: scratch
#	$t1: scratch
#	$t2: scratch
#	$t3: temp storage of $a0
#	$t4: '0'
#	$t5: '9'
#	$t6: '\n'
#####
	.globl main

main:
	li	$t4, 0x30		# '0'
	li	$t5, 0x39		# '9'
	li	$t6, 0xa		# '\n'

	# Prepare to load strings

#####
# Load string 1
#####
	li	$v0, 4			# print_string
	la	$a0, strEnter1
	syscall				# Ask for first string

	li	$v0, 8			# read_string
	la	$a0, str1
	li	$a1, 1024
	syscall				# Okay, $a0 is now the string

	move	$t0, $a0		# t0 is a temp pointer to the string

main_loop_a0:
	lb	$t2, 0($t0)		# $t2 = current byte
	beq	$t2, $t6, main_loop_a0_done	# Loop until we hit '\n'
	beqz	$t2, main_loop_a0_done	# though NULL would suffice

	bgt	$t2, $t5, error		# Error if byte > '9'
	blt	$t2, $t4, error		# Error if byte < '0'

	addi	$t0, $t0, 1		# Move to next byte
	b	main_loop_a0

main_loop_a0_done:
	sb	$0, 0($t0)		# Store NULL in last byte

	beq	$t0, $a0, error		# Empty string ==> Error

	move	$t3, $a0		# Store $a0 in $t3 for now

#####
# Load string 2
#####
	li	$v0, 4			# print_string
	la	$a0, strEnter2
	syscall				# Ask for second string

	li	$v0, 8			# read_string
	la	$a0, str2
	li	$a1, 1024
	syscall				# Okay, $a0 is now string 2

	move	$t0, $a0		# t0 is a temp pointer to the string

main_loop_a1:
	lb	$t2, 0($t0)		# $t2 = current byte
	beq	$t2, $t6, main_loop_a1_done	# Loop until we hit '\n'
	beqz	$t2, main_loop_a1_done	# though NULL would suffice

	bgt	$t2, $t5, error		# Error if byte > '9'
	blt	$t2, $t4, error		# Error if byte < '0'

	addi	$t0, $t0, 1		# Move to next byte
	b	main_loop_a1

main_loop_a1_done:
	sb	$0, 0($t0)		# Store NULL in the last byte

	beq	$t0, $a0, error		# Empty string ==> Error

#####
# Call addBCD
#####
	move	$a1, $a0		# Move string 2 to $a1 for addBCD call
	move	$a0, $t3		# Move string 1 back where it belongs

	jal	addBCD

#####
# Generate output
#####
	lb	$t1, 0($a3)		# Load first byte of return value
	addi	$t1, $t1, -0x30		# subtract '0'
	bnez	$t1, main_skip_carry	# If first char is '0'...
	addi	$a3, $a3, 1		# ... there was no carry. Skip it.
main_skip_carry:

	li	$v0, 4

	syscall				# First number
	la	$a0, strPlus
	syscall				# " + "
	move	$a0, $a1
	syscall				# Second number
	la	$a0, strEquals
	syscall				# " = "
	move	$a0, $a3
	syscall				# Result
	la	$a0, strNl
	syscall				# "\n"
exit:
	li	$v0, 10
	syscall				# Exit

error:
	la	$a0, strInvalidInput
	li	$v0, 4
	syscall				# Print an error message
	b	exit

#####
# Data segment
#####
	.data

strEnter1:		.asciiz "Enter first number: "
str1:			.space	1024
strEnter2:		.asciiz "Enter second number: "
str2:			.space	1024
strInvalidInput:	.asciiz	"Invalid input\n"
strPlus:		.asciiz	" + "
strEquals:		.asciiz	" = "
strNl:			.asciiz	"\n"
