seal :: (Ord a) => [a] -> [a] -> [a]
seal xs [] = xs
seal [] ys = ys
seal (x:xs) (y:ys) = if x > y then y : seal (x:xs) ys else  x : seal xs (y:ys)

subHelp :: (Eq a) => [a] -> [a] -> [a] -> Bool
subHelp op [] [] = True
subHelp op (x:xs) [] = False
subHelp op [] ys = True
subHelp op (x:xs) (y:ys) 
    |x == y = subHelp op xs ys
    |otherwise = subHelp op op ys

--ETA reduced from pattern with two params
isSublist :: (Eq a) => [a] -> [a] -> Bool
isSublist xs = subHelp xs xs

combinator :: [a] -> [a] -> [[a]]
combinator [] ys = []
combinator xs ys = [[x,y] | x<-xs, y<-ys]

--takes a K where a <= b <= c <= K
--in (a, b, c)

mightOfPythagorus :: Int -> [(Int, Int, Int)]
mightOfPythagorus k
    | k < 5 = []
    | otherwise = tripletBuilder k (combinator' [1..k] [1..k])

--takes k and a list of tuples (m,n) where 
--a = m^2 - n^2 
--b = 2mn
--c = m^2 + n^2

tripletBuilder :: Int -> [(Int, Int)] -> [(Int, Int, Int)]
tripletBuilder k (mn:ps)
    | c > k = []
    | a > b && a < c && b < c = scaleTuple (b, a, c) 1 k ++ tripletBuilder k ps
    | a > b = tripletBuilder k ps
    | otherwise = scaleTuple tup 1 k ++ tripletBuilder k ps
    where 
        tup = pythagTuple mn
        a = fst' tup
        b = snd' tup
        c = thd' tup

--takes tuple of (m,n) returns a pythagorean tuple
pythagTuple :: (Int, Int) -> (Int, Int, Int)
pythagTuple mn =
    let m = fst mn
        n = snd mn
        msqrd = m^2 
        nsqrd = n^2
    in (msqrd - nsqrd, 2*m*n, msqrd + nsqrd)

--tuple processing
fst' :: (a,b,c) -> a
fst' (x,_,_) = x

snd' :: (a,b,c) -> b
snd' (_,x,_) = x

thd' :: (a,b,c) -> c
thd' (_,_,x)  = x

--takes tuple and creates list of scalar tuples that 
scaleTuple :: (Int, Int, Int) -> Int -> Int -> [(Int, Int, Int)]
scaleTuple tup s k
    | thd' scaledtup > k = []
    | otherwise = scaledtup : scaleTuple tup (s + 1) k
    where 
        a = fst' tup
        b = snd' tup
        c = thd' tup    
        scaledtup = (a*s, b*s, c*s)

--combinator function for generating (m,n) tuples that follow the predicate 
combinator' :: [Int] -> [Int] -> [(Int, Int)]
combinator' [] ys = []
combinator' xs ys = [(x,y) | x<-xs, y<-ys, predicate x y]

--pythagorean triplet (m,n) predicate
predicate :: Int -> Int -> Bool
predicate m n 
    | (odd m && odd n) || m - n <= 0 = False
    | otherwise = True


