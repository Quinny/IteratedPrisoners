import Data.List (sortBy, groupBy, nubBy, intersperse)
import Data.Function (on)
import Data.Map (fromListWith, toList)
import Control.Monad (guard)
import Control.Monad.Writer

-- | Types for the IPD
data Decision = Defect | Cooperate deriving (Eq)
type Strategy = [Decision] -> Decision
type Name     = String
data Prisoner = Prisoner {
    name    :: Name,
    strat   :: Strategy,
    history :: [Decision]
}

instance Show Prisoner where
    show  = name

instance Ord Prisoner where
    compare = compare `on` name

-- | Compare prisoners based on their names
--   Useful if we want to make a restriction such
--   that players can't play against themselves
instance Eq Prisoner where
    p1 == p2 = name p1 == name p2

-- | Update the history of a prisoner by adding a decision
updateHistory :: Decision -> Prisoner -> Prisoner
updateHistory d (Prisoner n s h) = Prisoner n s (d:h)

-- | Strategies for playing the game

-- | Always defect
allD :: Strategy
allD _ = Defect

-- | Always cooperate
allC :: Strategy
allC _ = Cooperate

-- | Cooperate at first, mirror all other moves
tft :: Strategy
tft []    = Cooperate
tft (x:_) = x

-- | Only defect if the opponent defects two times in a row
tf2t :: Strategy
tf2t (Defect : Defect : xs)  = Defect
tf2t _                       = Cooperate

-- | Cooperate until the opponent defects, then defect unforgivingly
grudger :: Strategy
grudger [] = Cooperate
grudger xs = case Defect `elem` xs of
    True  -> Defect
    False -> Cooperate

-- | Defect until the opponent cooperates, then cooperate like a sucker
sucker :: Strategy
sucker [] = Defect
sucker xs = case Cooperate `elem` xs of
    True  -> Cooperate
    False -> Defect

-- | Same as tit for tat, but start by defecting
susTft :: Strategy
susTft []    = Defect
susTft (x:_) = x

-- | Same as tf2t, start by defecting
susTf2t :: Strategy
susTf2t (Defect : Defect : xs) = Defect
susTf2t [x]                    = Defect
susTf2t _                      = Cooperate

-- | Only cooperate if the oponent cooperates twice in a row
hesitant :: Strategy
hesitant (Cooperate : Cooperate : _) = Cooperate
hesitant _                           = Defect

-- | Helper function to generate a Prisoner
--   based on a name and a strategy
fromStrat :: Name -> Strategy -> Prisoner
fromStrat name = flip (Prisoner name) []

-- | Generate a list of all Prisoners
players :: [Prisoner]
players = map (uncurry fromStrat)
    [("All Defect",      allD),
     ("All Coop",        allC),
     ("Tit for tat",     tft),
     ("Tit for 2 tat",   tf2t),
     ("Grudger",         grudger),
     ("Sus Tit for tat", susTft),
     ("Hesitant",        hesitant),
     ("Sucker",          sucker),
     ("Sus 2 tat",       susTf2t)]

-- | Given two descisions, compute the score for each player
--   The score function here has been altered such that a higher value
--   is more favourable.  Higher value => less time spent in jail
jailTime :: Decision -> Decision -> (Int, Int)
jailTime Cooperate Cooperate = (3, 3)
jailTime Cooperate Defect    = (0, 5)
jailTime Defect Cooperate    = (5, 0)
jailTime Defect Defect       = (1, 1)

-- | Given two Prisoners and a number of rounds to play
--   returns the score recieved at each round
playGame :: Prisoner -> Prisoner -> Int -> [(Int, Int)]
playGame _ _ 0 = []
playGame p1 p2 n = (jailTime d1 d2) :
    playGame (updateHistory d1 p1) (updateHistory d2 p2) (n-1)
        where
           -- Prisoner 1's decision
           d1  = (strat p1) (history p2)
           -- Prisoner 2's decision
           d2  = (strat p2) (history p1)

-- | Plays all Prisoners against each other and records the results
playRound :: [Prisoner] -> Int -> [(Prisoner, Int)]
playRound players rounds = do
    x <- players
    y <- players
    -- uncomment below to not allow prisoners to play
    -- against themselves
    -- guard (x /= y)

    let outcomes = playGame x y rounds
    let scoreX = sum $ map fst outcomes

    return (x, scoreX)

-- | Eliminate lower half of players
eliminateHalf :: [(Prisoner, Int)] -> [Prisoner]
eliminateHalf scores = map fst half
    where
        sorted = reverse $ sortBy (compare `on` snd) scores
        half = take ((length sorted) `div` 2) sorted

tallyScores :: [(Prisoner, Int)] -> [(Prisoner, Int)]
tallyScores xs = concatMap (toList . fromListWith (+)) groups
    where groups = groupBy ((==) `on` fst) xs

allTied :: [(Prisoner, Int)] -> Bool
allTied xs = length (nubBy ((==) `on` snd) xs) == 1

playTourny :: [Prisoner] -> Int -> Writer [[(Prisoner, Int)]] [Prisoner]
playTourny xs rounds = do
    let rankings = (tallyScores . playRound xs) rounds
    tell [rankings]
    if allTied rankings
        then return $ map fst rankings
        else playTourny (eliminateHalf rankings) rounds

main = do
    let (winners, log) = runWriter $ playTourny players 100
    print "Winner(s)"
    mapM_ print winners
    print "===================="
    print "Round seqeunce"
    mapM_ print log
