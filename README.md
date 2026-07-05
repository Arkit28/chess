# chess
Basic PvP and PvComputer is available. 

Compile with g++/gcc and run the executable in cmd to play.

---

## Gameplay Inputs

### Piece Inputs

Move input syntax follows a very simple representation: 

- two characters for current square
- two characters for target square

The notation is piece agnostic, so there is no need for separate notation for different pieces. Together this forms a complete move input. Examples from the start position (White's POV):

- `e2e4`  is a valid move because it specifies the current square `e2` and a target square `e4`
(White King's pawn moves two squares forward)

- `g1f3`  is a valid move because it specifies the current square `g1` and a target square `f3`
(White knight moves from forward 2 squares and left one square)

### General Inputs

Commands for offering a draw and resgination: 

- `quit`  : Forfeit the game
- `draw?`  : Offer the opponent a draw

---

Currently working through nextSteps.md plan outline.