fibonacci::Integer->Integer
fibonacci n = if n == 0 then 0
              else if n == 1 then 1
              else fibonacci (n-2) * fibonacci (n-1)

sqr::Integer->Integer
sqr n = n * n

makeSquare :: [Integer]->[Integer]
makeSquare l = (map sqr l)


main = do
  print(makeSquare [1, 2, 4 ,5])
  print(fibonacci 4)
