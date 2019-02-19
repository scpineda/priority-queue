#ifndef WHEREIAM_H
#define WHEREIAM_H
#include <cstdlib>
#include <iostream>

struct whereIam
{
    int row, col;

    whereIam(int r = int(), int c = int())
    {
        row = r;
        col = c;
    }

    friend
    bool operator==(const whereIam &a, const whereIam &b)
    {
        return (a.row == b.row) && (a.col == b.col);
    }

    friend
    bool operator!=(const whereIam &a, const whereIam &b)
    {
        return !(a == b);
    }

    friend
    std::ostream &operator<<(std::ostream &out, const whereIam &where)
    {
        char cols[] = {'0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

        out << cols[where.col] << where.row << std::endl;
        return out;
    }

    bool isValid()
    {
        if((row > 0) && (row < 6))
            if((col > 0) && (col < 6))
                return true;

        return false;
    }

    bool isZero()
    {
        return (row == 0 || col == 0);
    }
};

struct chessMove
{
    whereIam position;
    int waysOut;

    chessMove(whereIam wia = whereIam())
    {
        whereIam temp;
        whereIam tempTwo;

        position = wia;

        waysOut = 0;

        // Ways Out:
        for(int i = -2; i < 3; i += 4)
            for(int j = -1; j < 2; j+= 2)
            {
                temp = whereIam(position.row + i, position.col + j);
                tempTwo = whereIam(position.row + j, position.col + i);

                if(temp.isValid() && temp != position && temp != tempTwo)
                    waysOut++;

                if(tempTwo.isValid() && tempTwo != position && temp != tempTwo)
                    waysOut++;
            }
    }

    friend
    bool operator<(const chessMove &x, const chessMove &y)
    {
        return x.waysOut < y.waysOut;
    }
};

struct currentPosition
{
    whereIam position;
    chessMove whereIcanGo[8];
    size_t index;

    currentPosition()
    {
        position = whereIam();
        index = 0;
        createArray();
    }

    currentPosition(int row, int col, size_t newIn)
    {
        position.row = row;
        position.col = col;
        index = newIn;
        createArray();
    }

    void copy(const currentPosition &other)
    {
        position = other.position;
        index = other.index;

        for(size_t i = 0; i < 8; ++i)
            whereIcanGo[i] = other.whereIcanGo[i];
    }

    friend
    std::ostream &operator<<(std::ostream &out, const currentPosition &cur)
    {
        out << "Position: " << cur.position ;
//        out << "WhereICanGo Array: " << std::endl << std::endl;

//        for(int i = 0; i < 8; ++i)
//            out << cur.whereIcanGo[i].position
//                << "  WAYS OUT: " << cur.whereIcanGo[i].waysOut << std::endl;

        return out;
    }

    friend
    bool operator !=(const currentPosition &a, const currentPosition &b)
    {
        return a.position != b.position;
    }

    friend
    bool operator ==(const currentPosition &a, const currentPosition &b)
    {
        return a.position == b.position;
    }

    void order()
    {
        for(size_t i = 0; i < 8; ++i)
            for(size_t j = 0; j < 8; ++j)
                if(whereIcanGo[i] < whereIcanGo[j])
                    std::swap(whereIcanGo[i], whereIcanGo[j]);
    }

    void createArray()
    {
        whereIam temp;
        whereIam tempTwo;

        int index = 0;

        for(int i = -2; i < 3; i += 4)
          for(int j = -1; j < 2; j+=2)
          {
             {
                temp = whereIam(position.row + j, position.col + i);
                tempTwo = whereIam(position.row + i, position.col + j);

                if(temp.isValid())
                {
                    whereIcanGo[index] = temp;
                    index++;
                }

                if(tempTwo.isValid())
                {
                    whereIcanGo[index] = tempTwo;
                    index++;
                }
              }
          }


        for(int k = 0; k < 8; ++k)
            if(whereIcanGo[k].position.isZero())
                whereIcanGo[k].waysOut = 0;

    }

    bool increaseIndex()
    {
        index++;

        if(index == 8)
        {
            index = 0;
            return false;
        }

        return true;
    }

    whereIam returnAtIndex()
    {
        return whereIcanGo[index].position;
    }


};
#endif // WHEREIAM_H
