			.text
			.globl			__start
__start:
			jal			_main
			li			$v0, 			10
			syscall
_main:
			li			$t0, 			'>'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_x
			li			$t0, 			'>'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$v0, 			5
			syscall
			move			$t0, 			$v0
			sw			$t0, 			_y
			li			$t0, 			'x'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			'='
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_x
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			'y'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			'='
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_y
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
L4:
			lw			$t0, 			_x
			lw			$t1, 			_y
			beq			$t0, 			$t1, 			L1
			lw			$t2, 			_x
			lw			$t3, 			_y
			bge			$t2, 			$t3, 			L2
			lw			$t4, 			_y
			lw			$t5, 			_x
			sub			$t6, 			$t4, 			$t5
			sw			$t6, 			_y

			j			L3
L2:
			lw			$t0, 			_x
			lw			$t1, 			_y
			sub			$t2, 			$t0, 			$t1
			sw			$t2, 			_x

L3:

			j			L4
L1:
			li			$t0, 			'g'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			li			$t0, 			'='
			li			$v0, 			11
			move			$a0, 			$t0
			syscall
			lw			$t0, 			_x
			li			$v0, 			1
			move			$a0, 			$t0
			syscall
			li			$t0, 			'\n'
			li			$v0, 			11
			move			$a0, 			$t0
			syscall

			jr			$ra
			.data
_x:			.word			0
_y:			.word			0
