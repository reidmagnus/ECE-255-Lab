.macro	memorygenerate	destination, size
	mov		r0, #1			// Initialize counter
loop_begin\@:
	cmp 	r0, \size			// If r0 is greater than size, end the loop
	bgt 	loop_end\@
	str 	r0, [\destination]		// Store r0 in the address
	add 	\destination, #4		// Increment the address
	add 	r0, #1				// Increment r0 by 1
	b	loop_begin\@			// Repeat the loop
loop_end\@:
	.endm

.data
	.align 4
	myArray: .space 600
	sumOfarray: .word 0x0010


	.text
	.global main
main:
	mov	r5, #0				//Ensuring that the contents at the sumOfarray address is 0
	ldr r7, =sumOfarray
	str	r5, [r7]
	
	ldr	r3, =myArray			//Generate myArray, an array from 1-150 using memorygenerate using parameters r3 and r4
	mov	r4, #150
	memorygenerate r3, r4
	
	mov	r6, #0				//Initialize loop counter to 0
	ldr	r0, =myArray			//Load r0 with the address of myArray (the first element)

loop:
	cmp	r6, r4				//If 150 numbers have been added, go to stop
	bge	stop
	
	add	r6, #1				//Increment r6 (the loop counter)
	bl	addsubroutine			//Go to addsubroutine
	b	loop				//Repeat loop

addsubroutine:
    	ldr     r1, [r0]			// Loads r1 with the value of the element in myArray that is pointed to by r0
	ldr 	r7, =sumOfarray			// Loads r7 with the address of sumOfarray
	ldr	r5, [r7]			// Load the current value for sum (what is currently contained in the address of sumOfarray)
	add	r5, r1				// Add r1 to r5 (add the value of the current element of myArray to the sum)
    	str     r5, [r7]			// Store the sum back in sumOfarray
    	add     r0, #4				// Increments r0 to point to the next element of myArray
    	bx      lr				// Return from subroutine

stop:	nop
		b 		stop
		.end

