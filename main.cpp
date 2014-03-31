// Jeff DiMarco, March 2014
// Peg jumping game solver
//
// This program runs all combinations of triangular peg solitaire
//  for a given number of total rows.  The indexing starts from the 
//  top single index of the triangle and goes down.

#include <iostream>
#include <vector>

#include "Peg.hpp"

#define TRIANGLE_ROWS 5
#define DEBUG false

void init();
void solvePegProblem(std::vector<Peg>, int);
void printBoard(std::vector<Peg>);
int getRow(int);
int getCol(int);
int getPosition(int, int);
int getLandingIndex(int, int);

std::vector<Peg> pegList;
std::vector<int> solutionsList;
std::vector<int> attemptsList;
int numSolutions = 0;
int numAttempts = 0;

int main(){

    // Initialize Pegs, positions, and neighbors
    init();

    // Run through each starting position
    for(int i = 0; i < pegList.size(); ++i){
        for(int j = 0; j < pegList.size(); ++j){
            pegList[j].hasPeg = true;
        }
        pegList[i].hasPeg = false;
        solutionsList[i] = 0;
        attemptsList[i] = 0;
        solvePegProblem(pegList, i);
    }

    std::cout << "Finished. Number of solutions: " << numSolutions << std::endl;
    std::cout << "Number of attempts           : " << numAttempts << std::endl;
    for(int i = 0; i < pegList.size(); ++i){
        std::cout << "Solutions for hole " << i << ":  " << solutionsList[i] << "  ";
        std::cout << "Attempts  for hole " << i << ":  " << attemptsList[i] << "  Perc: ";
        std::cout << (float)solutionsList[i]/(float)attemptsList[i] << std::endl;
    }
}

// Recursively solves a board state with brute force search
void solvePegProblem(std::vector<Peg> inputPegList, int startingHole){

    // Check for possible jumps
    for(int i = 0; i < inputPegList.size(); ++i){
        // Consider jumping into each missing slot
        if(!inputPegList[i].hasPeg){
            // Check neighbors that could be jumped
            for(int j = 0; j < inputPegList[i].neighborList.size(); ++j){
                int jumpedIndex = inputPegList[i].neighborList[j];
                // The neighbor's neighbors will be considered as the jumper
                for(int k = 0; k < inputPegList[jumpedIndex].neighborList.size(); ++k){
                    int jumperIndex = inputPegList[jumpedIndex].neighborList[k]; 
                    int landingIndex = inputPegList[i].position;
                    // If the empty slot can be jumped into with these conditions, push a new state to solve
                    if(getLandingIndex(jumpedIndex, jumperIndex) == landingIndex && inputPegList[jumpedIndex].hasPeg && inputPegList[jumperIndex].hasPeg && !inputPegList[landingIndex].hasPeg){
                        std::vector<Peg> temp = inputPegList;
                        temp[jumpedIndex].hasPeg = false;
                        temp[jumperIndex].hasPeg = false;
                        temp[landingIndex].hasPeg = true;                        
                        // Recurse with next board using this jump
                        solvePegProblem(temp, startingHole);
                    }
                }
            }
        }
    }

    // No jumps remain
    int hasPegCount = 0;
    for(int i = 0; i < inputPegList.size(); ++i){
        if(inputPegList[i].hasPeg){
            hasPegCount++;
        } 
    }
    // Stats
    numAttempts++;
    attemptsList[startingHole]++;
    if(hasPegCount == 1){
        numSolutions++;
        solutionsList[startingHole] += 1;
    }

    return;
}

// Determines the row of the given position
int getRow(int inputPosition){

    int currRow = 0;
    int runningSum = 1;

    while(currRow < TRIANGLE_ROWS){
        if(inputPosition < runningSum){
            return currRow;
        }else{
            currRow++;
            runningSum += (currRow+1);
        }
    }
    std::cerr << "Error: Couldn't find row for position: " << inputPosition << std::endl;
    return -1;
}

// Determines the col of the given position
int getCol(int inputPosition){
  
    int currRow = 0;
    int currCol = 0;
    int runningSum = 0;
    int prevRunningSum = runningSum;
    
    runningSum += (currRow+1); 
    while(currRow < TRIANGLE_ROWS){
        if(inputPosition < runningSum){
            return inputPosition - prevRunningSum;
        }else{
            currRow++;
            prevRunningSum = runningSum;
            runningSum += (currRow+1);
        }
    }

    std::cerr << "Error: Couldn't find column for position: " << inputPosition << std::endl;
    return -1;
}

// Gives the integer position for a given row and column
int getPosition(int inputRow, int inputCol){

    int currRow = 0;
    int runningSum = 0;

    while(currRow < inputRow){
        currRow++;
        runningSum += currRow;
    }

    runningSum += inputCol;

    return runningSum;

}

// Returns index after jump is completed. -1 if not possible
int getLandingIndex(int inputJumped, int inputJumper){

    int jumpedRow = getRow(inputJumped);
    int jumpedCol = getCol(inputJumped); 
    int jumperRow = getRow(inputJumper);
    int jumperCol = getCol(inputJumper);

    // Case jumped is row above
    if(jumpedRow == jumperRow-1){
        if(jumpedCol == jumperCol){
            // Jumping up and right
            if(jumpedCol <= jumpedRow-1){
                return getPosition(jumpedRow-1, jumpedCol);
            } else {
                // Above and to the right is off board
                return -1;
            }
        }
        if(jumpedCol == jumperCol - 1){
            // Jumping up and left
            if(jumpedCol-1 >= 0){
                return getPosition(jumpedRow-1, jumpedCol-1);
            } else {
                // Above and to the left is off board
                return -1;
            }
        }
    } 
    // Case jumped is same row
    if(jumpedRow == jumperRow){
        if(jumpedCol < jumperCol){
            // Jumping left
            if(jumpedCol > 0){
                return getPosition(jumpedRow, jumpedCol-1);
            } else {
                // Jumped is on left edge
                return -1;
            }
        }
        if(jumpedCol > jumperCol){
            // Jumping right
            if(jumpedCol < jumpedRow){
                return getPosition(jumpedRow, jumpedCol+1);
            } else {
                // Jumped is on right edge
                return -1;
            }
        }
    }
    // Case jumped is row below
    if(jumpedRow == jumperRow+1){
        if(jumpedCol == jumperCol + 1){
            // Jumping down and right
            if(jumpedRow+1 < TRIANGLE_ROWS){
                return getPosition(jumpedRow+1, jumpedCol+1);
            } else {
                // Below is off board
                return -1;
            }
        }
        if(jumpedCol == jumperCol){
            // Jumping down and left
            if(jumpedRow+1 < TRIANGLE_ROWS){
                return getPosition(jumpedRow+1, jumpedCol);
            } else {
                // Below is off board
                return -1;
            }
        }

    }
    return -1;
}

void init(){
    // Set up peg positions
    int currPeg = 0;
    for(int i = 0; i < TRIANGLE_ROWS; ++i){
        for(int j = 0; j <= i; ++j){
            pegList.push_back(Peg(currPeg, i, j));
            // Neighbor(s) above
            if(i-1 >= 0){
                if(j-1 >= 0){
                    // Neighbor above to the left
                    pegList[currPeg].neighborList.push_back(getPosition(i-1, j-1));
                }
                if(j-1 < i-1){
                    // Neighbor above to the right
                    pegList[currPeg].neighborList.push_back(getPosition(i-1, j));
                }
            }
            // Neighbors in same row
            if(j >= 1){
                pegList[currPeg].neighborList.push_back(getPosition(i, j-1));
            }
            if(j+1 < i+1){
                pegList[currPeg].neighborList.push_back(getPosition(i, j+1));
            }
            // Neighbor(s) below
            if(i+1 < TRIANGLE_ROWS){
                pegList[currPeg].neighborList.push_back(getPosition(i+1, j));
                pegList[currPeg].neighborList.push_back(getPosition(i+1, j+1));
            }
            currPeg++;
        }
    }
    solutionsList.resize(pegList.size());
    attemptsList.resize(pegList.size());
    return;
}

// Used for debugging.  Prints the board state
void printBoard(std::vector<Peg> inputPegList){
    std::cout << "Current Board:" << std::endl;
    for(int i = 0; i < TRIANGLE_ROWS; ++i){
        int numSpaces = TRIANGLE_ROWS - i; 
        for(int printSpaces = 0; printSpaces < numSpaces; ++printSpaces)
            std::cout << " ";
        for(int j = 0; j <= i; ++j){
            if(inputPegList[getPosition(i,j)].hasPeg){
                std::cout << 1 << " ";
            }else{
                std::cout << 0 << " ";
            }
        }
        std::cout << std::endl;
    }
}
