#include <iostream>
#include <queue.h>
#include <whereiam.h>
#include <sstream>

using namespace std;

bool validSpot(string &input);
void convertInput(queue<currentPosition, int> &moves, string inp);
bool addMoves(queue<currentPosition, int> &moves);
void displayMoves(queue<currentPosition, int> &moves);

// GLOBAL DATA

currentPosition newMove;
currentPosition temp;
currentPosition start;
bool couldAdd = true;
bool goodAdd = false;

int main()
{
    // DATA TABLE
    string spot;
    bool program = true;

    // Make a queue of moves
    queue<currentPosition, int> moves = queue<currentPosition, int>(25);

    // Begin Tour Prompting

    //while(program)
    //{
        cout << "KNIGHT'S TOUR. To Exit, type in: 00" << endl << endl;

        // User prompts

        while(!validSpot(spot))
        {
            cout << "Enter a valid spot: " << endl;
            cin >> spot;
            if(!validSpot(spot))
               cout << "Not valid. Try again." << endl;

            if(spot == "00")
            {
                program = false;
                break;
            }
        }

        convertInput(moves, spot);
        newMove = moves.peek();

        // Brute Force
        if(addMoves(moves))
        {
            cout << "Found a solution." << endl << endl;
            displayMoves(moves);
        }

        else
            cout << "A solution could not be found.";

    //}

}

// Makes sure that the user inputs a valid algebraic notation position
bool validSpot(string &input)
{
    // Algebraic notation is given as a through h and then 1 through 8
    if(input.find_first_of("abcdefgABCDEFG") == 0)
        if(input.find_first_of("12345678") == 1)
            return true;

    return false;
}

// Converts the given input and adds it to the queue
void convertInput(queue<currentPosition, int> &moves, string inp)
{
    char row;
    int realRow;
    int col;

    stringstream ss;

    ss << inp;

    ss >> row >> col;

    realRow = int(row) - 96;

    start = currentPosition(col, realRow, 0);

    moves.enqueue(start, 1);
}

// Adding the Moves
// Returns a false if not successful
bool addMoves(queue<currentPosition, int> &moves)
{
    while(!moves.full())
    {
        // Set temp to equal the first index at newMove's whereIcan array
        // and the index to keep track
        temp = currentPosition(newMove.returnAtIndex().row, newMove.returnAtIndex().col, newMove.index);
        couldAdd = true;

        // Check three things:
        // The position is valid
        // Temp is not already in moves
        // temp does not equal the last newMove

            if( (temp.position.isValid()) && (!moves.search(temp)) && (temp != newMove))
            {
                newMove.copy(temp);
                moves.enqueue(newMove, moves.highestPriority());
                newMove.index = 0;
            }

            // If it did not work, then simply increase the index
            // Then the loop return to the top

            else if(newMove.increaseIndex());

            // if newMove.increaseIndex() returns false, the index is set to 0
            // and the queue dequeues

            else
            {
                moves.dequeue();

                // if at the time of dequeueing, if one move remains and its ways out = 0
                // then break loop
                if(moves.size() == 1 && moves.peek().index == 8)
                    break;

                // Else, you can peek
                newMove = moves.dequeue();
            }
    }

    // If moves is full, then it returns true and successful. Else, return false;
    return moves.full();
}

void displayMoves(queue<currentPosition, int> &moves)
{
    currentPosition display;

    while(!moves.empty())
    {
        display = moves.dequeue();

        cout << display << endl;
    }

    cout << "Info." << endl;
    cout << moves.size() << endl;
}
