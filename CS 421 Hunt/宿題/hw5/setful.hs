type Set a = (a -> Bool)

setSuchThat :: (a -> Bool) -> Set a
setSuchThat f = f

unionSet :: Set a -> Set a -> Set a 
unionSet f g x = f x || g x

intersectSet :: Set a -> Set a -> Set a
intersectSet f g x = f x && g x

memberSet :: Set a -> a -> Bool
memberSet f = f

complementSet :: Set a -> Set a
complementSet f = not . f
