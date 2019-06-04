			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			3
			sw			$t0, 			_y
			la			$t0, 			L1
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			li			$t0, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			6
			sw			$t0, 			_x
L3:
			lw			$t0, 			_x
			lw			$t1, 			_y
			ble			$t0, 			$t1, 			L2
			lw			$t3, 			_x
			li			$v0, 			1
			move			$a0, 			$t3
			syscall
			li			$t3, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t3
			syscall

			lw			$t2, 			_x
			sub			$t2, 			$t2, 			1
			sw			$t2, 			_x
			j			L3
L2:
			la			$t0, 			L4
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			li			$t0, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			0
			sw			$t0, 			_x
L6:
			lw			$t0, 			_x
			lw			$t1, 			_y
			bge			$t0, 			$t1, 			L5
			lw			$t3, 			_x
			li			$v0, 			1
			move			$a0, 			$t3
			syscall
			li			$t3, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t3
			syscall

			lw			$t2, 			_x
			add			$t2, 			$t2, 			1
			sw			$t2, 			_x
			j			L6
L5:

			jr			$ra
			.data
_x:			.word			0
_y:			.word			0
L4:			.asciiz			"count up till == 3 for loop from 0"
L1:			.asciiz			"count down till == 3 for loop from 6"
