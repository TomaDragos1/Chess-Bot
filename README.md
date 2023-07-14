# Chess Bot

# **Stage 1** 
## Compiling instructions

> Use the build rule in the makefile to compile the target.  
> make run is used to start the engine  
> In order to delete the object files use make clear

---  

 ### *This is a C++ program that allows you to play chess with a basic command-line interface. The program supports the variation of Crazyhouse chess.*

### *This program is using the following classes: BoardPiece (a class that holds the data for a piece), Board (a matrix that holds the chess board), Bot (simulates all possible moves and sends back a legal move) and Main.*
--- 
### **BoardPiece**

> Holds the data for a single square on the chess table.  
> A single BoardPiece can hold an enum of type Piece, which represents either a chess piece or an empty square.


### **Board**

> Holds the chess table data itself.  
> Is composed of a 8x8 matrix of boardpieces


### **Bot**

> This class represents the majority of the program. It holds the data of the current status of the game in a Board object and computes the legal moves that are possible from this point.  
> We compute all possible moves for every piece using specific functions for each piece type such as ***nextQueenPositions*** and then select the legal ones using the ***generateLegalMoves*** function.  
> Each function that generates moves for each type uses a copy of the current board on which it simulates all possible moves.  
> For the current state of the board we check if our king is in check and then filter out the remaining possible moves.  Out of the remaining moves we also check if the new move would result in a check and also select valid ones.  
> As mentioned in the project requirements we made the bot prioritize castling before any other move.  
> Out of all legal moves returned by the ***generateLegalMoves*** function we randomly select one to pass to calculateNextMove().  
> In the recordMove() function the data of the move is parsed from the string format to our local format.

---

## Sources

> We mostly looked at wikipedia articles to learn about more particular moves such as castling or en passant.  
> Because this was the first time we used C++'s object oriented features we heavily relied on various cppreference pages and many stack overflow threads.

---
## Project shares

> Andrei wrote part of the generating moves functions, a part of the castling implementation, a part of the en passant implementation and the documentation.  
> Yannick wrote the rest of the generating moves functions, a part of the functions that check if the moves are valid and the rest of the en passant implementation.
> Dragos wrote the structure of the BoardPiece, Board and Bot classes, a part of the functions that check if the moves are valid and the code for the recordMove function that we added, part of the castling and en passant implementations. He also did most of the debugging with Andrei and mostly Yannick helping him.

---

# **Stage 2**

> For this part of the project we implemented the **minimax** algorithm in order for the bot to make some sensible moves.   


## Implemented functions: 

### **generateOneMove**

> This function takes in a move parameter, makes the move on the global board and then returns the captured piece (if any).  

### **removeOneMove**

> Undoes the move made by generateOneMove and returns the global board to the state before calling generateOneMove


### **returnSafeMoves**

> Returns a vector of safe moves that can be made for the side given as parameter.  

### **minimax**

> The brain of the bot, it works by returning the value of a move made by the side thats given as parameter.  
> Under the hood it works by evaluating the best possible move for the opposing side and then selecting the best move that would be possible given the opponents' last best move. This process is repeated for **depth** times. value given as a parameter.   
> In order to speed up the evaluations we used alpha-beta pruning in order to skip the evaluations of the nodes in the tree that would yield a lesser result.   
> We call this function for each possible move in calculateNextMove and then select the one that has the gretest value.  


### **evaluateBoard**

> This function is used by the **minimax** function to compute the value of a move made by either players.  
> In order to make the evaluation more efficient and effective, we used multiple heuristics to compute the value of the move.  
> The heuristics in cause are: 
> - table scores for each piece:  It works by rewarding a move to certain position on the board while punishing moves to other positions. For each piece we use a matrix of values that the piece takes when it moves on that position.  
> - pawnStructure: We use this heuristic to penalize and reward pawns being in certain situations. A pawns that is isolated is penalized while a pawn that has passed another is being rewarded.  
> - kingSafety: We use this in order to prioritize the safety of the king instead of making a risky move. It is pretty straigtforward:  moves that shield the king from a check are rewarded while ones that expose the king are penalised. Example: castling is highly rewarded while moving the only shileding piece from a diagonal is penalized.  
> - pieceDevelopment: This heuristic is pretty extra for the scope of this project but we wanted to include it anyway. It works by rewarding moves towards the center of the board and moves that do not leave the pieces in their starting positions.  

### **returnCaptures**

> Out of the safe moves for a side it returns the moves that capture a piece.  

### **evaluateMove**
> Returns the integer value of a certain move.  

---
## Sources

> Our sources are still the same. We checked out a few sites like **https://www.chessprogramming.org/** and a few youtube videos like this one: **https://www.youtube.com/watch?v=l-hh51ncgDI** to get a feeling for how the code should look like,  what performance to expect and what optimisations to make.  

---
## Project Shares

> The minimax implementation was done by all of us.   
> The evaluation function was done by all of us.  
> The generateOneMove and removeOneMove were done mainly by Yannick and Dragoș.  
> The returnSafeMoves function was done by Andrei.   
> Debugging and optimisations was done by Dragoș and Yannick.  
> Research and the readme were done by Andrei.  
