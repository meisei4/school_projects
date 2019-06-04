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
			sw			$t0, 			_p
L10:
			lw			$t0, 			_p
			li			$t1, 			1
			ble			$t0, 			$t1, 			L2
			la			$t2, 			L3
			li			$v0, 			4
			move			$a0, 			$t2
			syscall
			lw			$t2, 			_p
			li			$v0, 			1
			move			$a0, 			$t2
			syscall
			la			$t2, 			L4
			li			$v0, 			4
			move			$a0, 			$t2
			syscall
			li			$t2, 			2
			sw			$t2, 			_d
L8:
			lw			$t2, 			_d
			lw			$t3, 			_p
			li			$t4, 			2
			div			$t5, 			$t3, 			$t4
			bge			$t2, 			$t5, 			L5
			lw			$t3, 			_p
			lw			$t4, 			_d
			div			$t6, 			$t3, 			$t4
			sw			$t6, 			_q
			lw			$t3, 			_q
			lw			$t4, 			_d
			mul			$t6, 			$t3, 			$t4
			lw			$t3, 			_p
			bne			$t6, 			$t3, 			L6
			lw			$t4, 			_d
			li			$v0, 			1
			move			$a0, 			$t4
			syscall
			la			$t4, 			L7
			li			$v0, 			4
			move			$a0, 			$t4
			syscall

L6:
			lw			$t0, 			_d
			li			$t1, 			1
			add			$t2, 			$t0, 			$t1
			sw			$t2, 			_d

			j			L8
L5:
			la			$t0, 			L9
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			la			$t0, 			L1
			li			$v0, 			4
			move			$a0, 			$t0
			syscall
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_p

			j			L10
L2:

			jr			$ra
			.data
_d:			.word			0
_p:			.word			0
_q:			.word			0
L7:			.asciiz			" "
L4:			.asciiz			" are "
L1:			.asciiz			"> "
L9:			.asciiz			"\n"
L3:			.asciiz			"factors of "
