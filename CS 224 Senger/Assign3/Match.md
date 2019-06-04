# Example for Match

Precompute Shift Array

shift[i] = largest j < i such that pattern[0..j-1] is suffix of pattern[0..i-1]

```
       i = 0 1 2 3 4 5 6 7
pattern  = A A G A A G T
shift[i] = 0 0 1 0 1 2 3 0

i = 4
pattern[0..3] = A A G A
	j = 3
	pattern[0..2] = A A G   not suffix
	j = 2
	pattern[0..1] = A A     not suffix
	j = 1
	pattern[0..0] = A       is suffix
shift[4] = 1

i = 6
pattern[0..5] = A A G A A G
	j = 5
	pattern[0..4] = A A G A A not suffix
	j = 4
	pattern[0..3] = A A G A   not suffix
	j = 3
	pattern[0..2] = A A G     is suffix
shift[6] = 3
	
```

Using Shift Array to Search

```
             v v x
			   0 1 2 3 4 5 6 7 8 9 0 1 2
source       A A C A A G A A G A A G T
pattern      A A G . . .
             0 1 2
fails at 2 in pattern, shift[2] = 1

             v v x
			   0 1 2 3 4 5 6 7 8 9 0 1 2
source       A A C A A G A A G A A G T
               A A . . .
               0 1
fails at 1 in pattern, shift[1] = 0

             v v x
			   0 1 2 3 4 5 6 7 8 9 0 1 2
source       A A C A A G A A G A A G T
                 A . . .
                 0
fails at 0 in pattern, shift[0] = 0

             v v v v v v v v v x
			   0 1 2 3 4 5 6 7 8 9 0 1 2
source       A A C A A G A A G A A G T
                   A A G A A G T
                   0 1 2 3 4 5 6 
fails at 6 in patterh, shift[6] = 3

             v v v v v v v v v v v v v
			   0 1 2 3 4 5 6 7 8 9 0 1 2
source       A A C A A G A A G A A G T
                         A A G A A G T
                         0 1 2 3 4 5 6


```
