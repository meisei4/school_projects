type Node = Integer
type Edge = (Integer, Integer)
type Graph = [Edge]
type Path = [Node]

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (x:xs) = 
    let pivot = [x]
        pivotf = [y | y <- xs, y < x]
        pivotb = [z | z <- xs, z > x]
    in quicksort pivotf ++ pivot ++ quicksort pivotb

nodes :: Graph -> [Node] 
nodes [] = []
nodes (x:xs) = quicksort ([fst x, snd x] ++ nodes xs)

neighbors :: Node -> Graph -> [Node]
neighbors _ [] = []
neighbors n g = [snd a | a<-g, fst a == n]

detach :: Node -> Graph -> Graph
detach _ [] = []
detach n g = [x | x<-g, fst x /= n && snd x /= n]

pathHelper :: Graph -> Node -> Node -> [Node] -> Path -> [Path]
pathHelper g n1 n2 [] p = []
pathHelper g n1 n2 (x:xs) p
    |n1 == n2 = [[n1]]
    |x == n2 = (p ++ [x]) : pathHelper g n1 n2 xs p
    |otherwise = pathHelper g x n2 xns newpath ++ pathHelper g n1 n2 xs p
    where
        xns = neighbors x g
        newpath = p ++ [x]

paths :: Node -> Node -> Graph -> [Path]
paths n1 n2 [] = [] 
paths n1 n2 g = 
    let ns = neighbors n1 g
    in pathHelper g n1 n2 ns [n1]
