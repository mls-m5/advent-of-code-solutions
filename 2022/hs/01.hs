import System.Environment (getArgs)
import qualified Data.ByteString as BS
import qualified Data.ByteString.Char8 as BSC

import Data.List (sortBy)

import qualified Data.ByteString.Char8 as BSC

main :: IO ()
main = do
    (fileName:_) <- getArgs
    contents <- BS.readFile fileName
    let calories = BSC.lines contents
        sortedCalories = sortBy compare calories
        topThree = take 3 sortedCalories
        sumTopThree = BSC.foldl' (\acc x -> acc + (BSC.readInt x)) 0 topThree
    putStrLn $ "answer to first question: " ++ (last sortedCalories)
    putStrLn $ "answer to second question: " ++ (show sumTopThree)
