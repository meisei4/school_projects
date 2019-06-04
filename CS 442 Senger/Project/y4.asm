			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			la			$t0, 			L1
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_x
			la			$t0, 			L2
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_x
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			la			$t0, 			L3
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			li			$t0, 			1
			sw			$t0, 			_fac
L5:
			lw			$t0, 			_x
			li			$t1, 			0
			ble			$t0, 			$t1, 			L4
			lw			$t2, 			_fac
			lw			$t3, 			_x
			mul			$t4, 			$t2, 			$t3
			sw			$t4, 			_fac
			lw			$t2, 			_x
			li			$t3, 			1
			sub			$t4, 			$t2, 			$t3
			sw			$t4, 			_x

			j			L5
L4:
			la			$t0, 			L6
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_fac
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			la			$t0, 			L3
			li			$v0, 			4
			move			$a0, 			$t0
			syscall

			jr			$ra
			.data
_x:			.word			0
_fac:			.word			0
L2:			.asciiz			"x = "
L6:			.asciiz			"factorial = "
L1:			.asciiz			"> "
L3:			.asciiz			"\n"
