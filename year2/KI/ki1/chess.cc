//
// C++ program to play simple chess:
// white king (WK) and white queen/rook (WQ) against black king (BK)
// Compile: g++ -Wall -Wextra -O2 -o chess chess2022.cc
// Run:     ./chess <thesize> <simulations> <maxgamelength>
//                  <gametypeW> <gametypeB> [Q|R] <print> <seed>
//          thesize: number of rows = columns of the game board
//          simulations: number of games played
//          maxgamelength: game is tied after this number of moves
//          gametypeW: 0 white random; 1 pure Monte Carlo; 2 clever
//          gametypeB: 0 black random; 1 try to capture queen;
//                     2 seek king mobility
//          [Q|R]: Q/q for white queen, everything else for white rook
//          print: if 1, print game(s)
//          seed: seeds random number generator
// Walter Kosters, January 3, 2022; w.a.kosters@liacs.leidenuniv.nl
//          February 8, 2022: small bug fix, line 522-532
//          February 10, 2022: (very) small bug fix, line 104
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

const int MAX = 30; // maximum board size

class Chess
{
public:
    // top left is (1,1) = (row,column)
    int thesize;         // height = width of game board
    int xBK, yBK;        // position of black king
    int xWK, yWK;        // position of white king
    int xWQ, yWQ;        // position of white queen (or rook)
    bool queencaptured;  // is white queen captured?
    bool whoistomove;    // who is to move? (true: white)
    int countmoves;      // number of moves so far
    bool queenorrook;    // true: white queen; false: white rook
    static int thecalls; // calls to evaluate
    // member functions, see below for comments
    Chess();
    void reset(int somesize, bool qor);
    void printboard();
    bool legalforblackking(int i, int j);
    bool legalforwhiteking(int i, int j);
    bool canwhitequeenreach(int i, int j);
    bool canwhiterookreach(int i, int j);
    bool legalforwhitequeen(int i, int j);
    bool incheck(int i, int j);
    bool checkmate();
    int numberofwhitequeenmoves();
    int numberofwhitekingmoves();
    int numberofwhitemoves();
    int numberofblackmoves();
    int blackmove(int gametypeB);
    void humanwhitemove();
    void humanblackmove();
    void dowhitekingmove(int k);
    void dowhitequeenmove(int k);
    void dowhitemove(int k);
    void doblackkingmove(int k);
    int playthegame(int maxgamelength, bool print,
                    int &nrmoves, int gametypeW, int gametypeB);
    void randomwhitemove();
    //============================================================= CHANGES
    void MCwhitemove(int maxgamelength, int playouts);
    std::vector<std::vector<int>> positionTransitions;
    void cleverwhitemove();
    int calculateScore();
}; // Chess

//=================================================================

// get first non-enter from keyboard
char getfirstchar()
{
    char c = cin.get();
    while (c == '\n')
    {
        c = cin.get();
    } // while
    return c;
} // getfirstchar

// initialize static class variable
int Chess::thecalls = 0;

// default constructor
Chess::Chess()
{
    // nothing
} // Chess::Chess

// reset the whole game
void Chess::reset(int somesize, bool qor)
{
    thesize = somesize;
    queenorrook = qor;
    xWK = rand() % thesize + 1;
    yWK = rand() % thesize + 1;
    do
    {
        xBK = rand() % thesize + 1;
        yBK = rand() % thesize + 1;
    } while (xBK - xWK <= 1 && xWK - xBK <= 1 && yBK - yWK <= 1 && yWK - yBK <= 1);
    do
    {
        xWQ = rand() % thesize + 1;
        yWQ = rand() % thesize + 1;
    } while ((xWQ == xWK && yWQ == yWK) || (xWQ == xBK && yWQ == yBK) || incheck(xBK, yBK) || numberofblackmoves() == 0);
    queencaptured = false;
    whoistomove = true;
    countmoves = 0;
} // Chess::reset

// can BK move from (xBK,yBK) to (i,j)?
bool Chess::legalforblackking(int i, int j)
{
    if (i <= 0 || j <= 0 || i > thesize || j > thesize)
        return false;
    if (i == xBK && j == yBK)
        return false;
    if (i > xBK + 1 || i < xBK - 1 || j > yBK + 1 || j < yBK - 1)
        return false;
    if (i - xWK <= 1 && xWK - i <= 1 && j - yWK <= 1 && yWK - j <= 1)
        return false;
    if (i == xWQ && j == yWQ)
        return true; // king captures queen
    if (incheck(i, j))
        return false;
    return true;
} // legalforblackking

// can WK move from (xWK,yWK) to (i,j)?
bool Chess::legalforwhiteking(int i, int j)
{
    if (i <= 0 || j <= 0 || i > thesize || j > thesize)
        return false;
    if (i == xWK && j == yWK)
        return false;
    if (i > xWK + 1 || i < xWK - 1 || j > yWK + 1 || j < yWK - 1)
        return false;
    if (i - xBK <= 1 && xBK - i <= 1 && j - yBK <= 1 && yBK - j <= 1)
        return false;
    if (i == xWQ && j == yWQ)
        return false;
    return true;
} // legalforwhiteking

// can WQ reach (i,j) from (xWQ,yWQ)?
bool Chess::canwhitequeenreach(int i, int j)
{
    if (i <= 0 || j <= 0 || i > thesize || j > thesize)
        return false;
    if (i != xWQ && j != yWQ && i + j != xWQ + yWQ && i - j != xWQ - yWQ)
        return false;
    if (i == xWQ && i == xWK && ((yWQ < yWK && yWK < j) || (j < yWK && yWK < yWQ)))
        return false;
    if (j == yWQ && j == yWK && ((xWQ < xWK && xWK < i) || (i < xWK && xWK < xWQ)))
        return false;
    if (i + j == xWQ + yWQ && i + j == xWK + yWK && ((xWQ - yWQ < xWK - yWK && xWK - yWK < i - j) || (i - j < xWK - yWK && xWK - yWK < xWQ - yWQ)))
        return false;
    if (i - j == xWQ - yWQ && i - j == xWK - yWK && ((xWQ + yWQ < xWK + yWK && xWK + yWK < i + j) || (i + j < xWK + yWK && xWK + yWK < xWQ + yWQ)))
        return false;
    return true;
} // canwhitequeenreach

// can WQ reach (i,j) from (xWQ,yWQ), when being a ROOK?
bool Chess::canwhiterookreach(int i, int j)
{
    if (i <= 0 || j <= 0 || i > thesize || j > thesize)
        return false;
    if (i != xWQ && j != yWQ)
        return false;
    if (i == xWQ && i == xWK && ((yWQ < yWK && yWK < j) || (j < yWK && yWK < yWQ)))
        return false;
    if (j == yWQ && j == yWK && ((xWQ < xWK && xWK < i) || (i < xWK && xWK < xWQ)))
        return false;
    return true;
} // canwhiterookreach

// is BK at (i,j) in check?
bool Chess::incheck(int i, int j)
{
    if (queenorrook)
        return canwhitequeenreach(i, j);
    else
        return canwhiterookreach(i, j);
} // Chess::incheck

// can WQ move from (xWQ,yWQ) to (i,j)?
bool Chess::legalforwhitequeen(int i, int j)
{
    if (i <= 0 || j <= 0 || i > thesize || j > thesize)
        return false;
    if (i == xWQ && j == yWQ)
        return false;
    if (i == xWK && j == yWK)
        return false;
    return incheck(i, j);
} // legalforwhitequeen

// black cannot move and is in check
bool Chess::checkmate()
{
    return (numberofblackmoves() == 0 && incheck(xBK, yBK));
} // checkmate

// how many moves are available for WK?
int Chess::numberofwhitekingmoves()
{
    int howmany = 0;
    int i, j;
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
            howmany += legalforwhiteking(xWK + i, yWK + j);
    return howmany;
} // Chess::numberofwhitekingmoves

// how many moves are available for WQ?
int Chess::numberofwhitequeenmoves()
{
    int howmany = 0;
    int i, j;
    for (i = 1; i <= thesize; i++)
        for (j = 1; j <= thesize; j++)
            howmany += legalforwhitequeen(i, j);
    return howmany;
} // Chess::numberofwhitequeenmoves

// how many moves are available for white?
int Chess::numberofwhitemoves()
{
    return (numberofwhitequeenmoves() + numberofwhitekingmoves());
} // Chess:numberofwhitemoves

// how many moves are available for black?
int Chess::numberofblackmoves()
{
    int howmany = 0;
    int i, j;
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
            howmany += legalforblackking(xBK + i, yBK + j);
    return howmany;
} // Chess::numberofblackmoves

// do k-th move for WK, 0 <= k < numberofwhitekingmoves ( )
void Chess::dowhitekingmove(int k)
{
    int i, j;
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
            if (legalforwhiteking(xWK + i, yWK + j))
            {
                if (k == 0)
                {
                    xWK = xWK + i;
                    yWK = yWK + j;
                    whoistomove = !whoistomove;
                    countmoves++;
                    return;
                } // if
                k--;
            } // if
} // Chess::dowhitekingmove

// do k-th move for WQ, 0 <= k < numberofwhitequeenmoves ( )
void Chess::dowhitequeenmove(int k)
{
    int i, j;
    for (i = 1; i <= thesize; i++)
        for (j = 1; j <= thesize; j++)
            if (legalforwhitequeen(i, j))
            {
                if (k == 0)
                {
                    xWQ = i;
                    yWQ = j;
                    whoistomove = !whoistomove;
                    countmoves++;
                    return;
                } // if
                k--;
            } // if
} // Chess::dowhitequeenmove

// do k-th move for white, 0 <= k < numberofwhitemoves ( )
void Chess::dowhitemove(int k)
{
    int wqms = numberofwhitequeenmoves();
    if (k < wqms)
    {
        dowhitequeenmove(k);
    } // if
    else
    {
        dowhitekingmove(k - wqms);
    } // else
} // Chess::dowhitemove

// do k-th move for BK, 0 <= k < numberofblackkingmoves ( )
void Chess::doblackkingmove(int k)
{
    int i, j;
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
            if (legalforblackking(xBK + i, yBK + j))
            {
                if (k == 0)
                {
                    xBK = xBK + i;
                    yBK = yBK + j;
                    if (xBK == xWQ && yBK == yWQ)
                    {
                        queencaptured = true;
                    } // if
                    whoistomove = !whoistomove;
                    countmoves++;
                    return;
                } // if
                k--;
            } // if
} // Chess::doblackkingmove

// do a random move for White
void Chess::randomwhitemove()
{
    dowhitemove(rand() % numberofwhitemoves());
} // Chess::randomwhitemove

// do a random move for BK
// return 0 if checkmate, 1 if stalemate (in both cases: no move),
// 2 if queen captured, 3 otherwise
// gametypeB = 0: totally random
//           = 1: capture WQ if possible
//           = 2: AND try for largest freedom
int Chess::blackmove(int gametypeB)
{
    int move, i, j;
    int number = numberofblackmoves();
    int freedom[8];
    int nr = 0;
    int equals = 1;
    int largest = -1;
    if (number == 0)
    {
        if (incheck(xBK, yBK))
            return 0;
        else
            return 1;
    } // if
    if (gametypeB > 0)
    { // try to capture WQ
        for (i = -1; i <= 1; i++)
            for (j = -1; j <= 1; j++)
                if (legalforblackking(xBK + i, yBK + j))
                {
                    if (xBK + i == xWQ && yBK + j == yWQ)
                    {
                        xBK = xBK + i;
                        yBK = yBK + j;
                        queencaptured = true;
                        whoistomove = !whoistomove;
                        countmoves++;
                        return 2;
                    } // if
                }     // if
    }                 // if
    if (gametypeB < 2)
    { // random
        move = rand() % number;
        for (i = -1; i <= 1; i++)
            for (j = -1; j <= 1; j++)
                if (legalforblackking(xBK + i, yBK + j))
                {
                    if (move == 0)
                    {
                        xBK = xBK + i;
                        yBK = yBK + j;
                        if (xBK == xWQ && yBK == yWQ)
                        {
                            queencaptured = true;
                            whoistomove = !whoistomove;
                            countmoves++;
                            return 2;
                        } // if
                        whoistomove = !whoistomove;
                        countmoves++;
                        return 3;
                    } // if
                    move--;
                } // if
    }             // if
    for (i = -1; i <= 1; i++)
        for (j = -1; j <= 1; j++)
            if (legalforblackking(xBK + i, yBK + j))
            {
                xBK = xBK + i;
                yBK = yBK + j;
                freedom[nr] = numberofblackmoves();
                if (freedom[nr] > largest)
                {
                    largest = freedom[nr];
                    equals = 1;
                } // if
                else if (freedom[nr] == largest)
                {
                    equals++;
                } // if
                nr++;
                xBK = xBK - i;
                yBK = yBK - j; // undo partial move!
            }                  // if
    move = rand() % equals;
    for (i = 0; i < number; i++)
    {
        if (freedom[i] == largest)
        {
            if (move == 0)
            {
                doblackkingmove(i);
                return 3;
            } // if
            move--;
        } // if
    }     // for
    return 999;
} // Chess::blackmove

// do a move for White: human player
void Chess::humanwhitemove()
{
    char choice;
    bool OK = false;
    int i, j;
    do
    {
        printboard();
        cout << "A (white) move please (Q or K) .. ";
        choice = getfirstchar();
        if (choice == 'q' || choice == 'Q')
        {
            cout << "Row number for queen (1..)     .. ";
            i = getfirstchar() - '0';
            cout << "Column number for queen (1..)  .. ";
            j = getfirstchar() - '0';
            if (legalforwhitequeen(i, j))
            {
                xWQ = i;
                yWQ = j;
                OK = true;
            } // if
            else
            {
                cout << "Illegal queen move ..." << endl;
            } // else
        }     // if
        else
        {
            cout << "Row number for king (1..)      .. ";
            i = getfirstchar() - '0';
            cout << "Column number for king (1..)   .. ";
            j = getfirstchar() - '0';
            if (legalforwhiteking(i, j))
            {
                xWK = i;
                yWK = j;
                OK = true;
            } // if
            else
            {
                cout << "Illegal king move ..." << endl;
            } // else
        }     // if
    } while (!OK);
    whoistomove = !whoistomove;
    countmoves++;
} // Chess::humanwhitemove

// do a move for Black: human player
void Chess::humanblackmove()
{
    bool OK = false;
    int i, j;
    do
    {
        printboard();
        cout << "A (black) move please .. " << endl;
        cout << "Row number for king (1..)      .. ";
        i = getfirstchar() - '0';
        cout << "Column number for king (1..)   .. ";
        j = getfirstchar() - '0';
        if (legalforblackking(i, j))
        {
            xBK = i;
            yBK = j;
            OK = true;
        } // if
        else
        {
            cout << "Illegal king move ..." << endl;
        } // else
    } while (!OK);
    whoistomove = !whoistomove;
    countmoves++;
} // Chess::humanblackmove

// print board
void Chess::printboard()
{
    int i, j;
    char A[MAX][MAX];
    for (i = 1; i <= thesize; i++)
    {
        for (j = 1; j <= thesize; j++)
        {
            A[i][j] = '.';
        } // for
    }     // for
    A[xBK][yBK] = 'z';
    A[xWK][yWK] = 'K';
    if (!queencaptured)
    {
        A[xWQ][yWQ] = (queenorrook ? 'Q' : 'R');
    } // if
    cout << "  ";
    for (j = 1; j <= thesize; j++)
    {
        cout << " " << j % 10;
    } // if
    cout << "   " << countmoves;
    if (whoistomove)
    {
        cout << "  WTM" << endl;
    } // if
    else
    {
        cout << "  BTM" << endl;
    } // else
    for (i = 1; i <= thesize; i++)
    {
        cout << (i < 10 ? " " : "") << i << " ";
        for (j = 1; j <= thesize; j++)
        {
            cout << A[i][j] << " ";
        } // for
        cout << endl;
    } // for
    cout << endl;
} // Chess::printboard

// play the game, print if asked to do so
// maxgamelength is the maximum length of the entire game
// return 0 if checkmate, 1 if stalemate, 2 if simple tie,
// 3 if stopped
// nrmoves is the final number of moves
// gametypeW 0: random; 1: pure Monte Carlo; 2: clever
// gametypeB 0: random; 1: capture queen if possible; 2: largest freedom
int Chess::playthegame(int maxgamelength, bool print,
                       int &nrmoves, int gametypeW, int gametypeB)
{
    int themove = 3;
    if (print)
    {
        printboard();
    } // if
    while (themove == 3 && countmoves < maxgamelength)
    {
        if (whoistomove)
        {
            switch (gametypeW)
            {
            case 0: // random
                randomwhitemove();
                break;
            case 1: // pure Monte Carlo
                MCwhitemove(maxgamelength, 100);
                break;
            case 2: // Minimax
                cleverwhitemove();
                break;
            } // switch
            if (print)
            {
                printboard();
            } // if
        }     // if
        else
        {
            themove = blackmove(gametypeB);
            if (themove > 1 && print)
            {
                printboard();
            } // if
        }     // else
    }         // while
    if (countmoves == maxgamelength)
    {
        if (queencaptured)
        {
            themove = 2;
        } // if
        else if ( !whoistomove && numberofblackmoves() == 0)
        {
            if (incheck(xBK, yBK))
                themove = 0;
            else
                themove = 1;
        } // if
    }     // if
    nrmoves = countmoves;
    return themove;
} // Chess::playthegame

//=================================================================

/// do one pure Monte Carlo (MC) move for white
// playouts random games (of maxgamelength) per candidate move
void Chess::MCwhitemove(int maxgamelength, int playouts)
{
    // TODO
    int bestscore = 0;
    int score = 0;
    int bestmove = 0;
    for (int i = 0; i < numberofwhitemoves(); i++)
    {
        int teller = 0;
        score = 0;
        for (int j = 0; j < playouts; j++)
        {
            Chess copy;          // object voor copy
            copy = *this;        // kopieer het bord om vooruit te denken
            copy.dowhitemove(i); // doe i'de zet op het copy
            if (copy.playthegame(maxgamelength - 1, false, teller, 0, 1) == 0)
            {                           // 100 keer random spelen
                score += 1000 - teller; // score bepalen
            }
        } // for j
        if (score > bestscore)
        {
            bestscore = score; // er is een betere zet
            bestmove = i;      // beste zet
        }
    }                      // for i
    dowhitemove(bestmove); // uiteindelijk de zet die je speelt
} // Chess::MCwhitemove

class chessPiece
{
public:
    chessPiece(int player, int type) : player(player), type(type) {}

    int getPlayer()
    {
        return player;
    }

    int getType()
    {
        return type;
    }

private:
    int player; // -1 = n/a, 1 = white, 2 = black
    int type;   // type 0 = cover, 1 = king, 2 = queen, 3 = rook, 4 = bishop
};

bool foundChessPiece(std::vector<chessPiece> v, int player)
{
    if (v.empty() ||                                                                   // No chesspiece
        !v.empty() && (v.front().getType() == 0 ||                                     // Chesspiece detected but it's a cover
                       (v.front().getType() == 1 && v.front().getPlayer() != player))) // Chesspiece detected but it's an enemy king
    {
        return false;
    }

    return true;
}

bool foundEnemyCover(std::vector<chessPiece> v, int player)
{
    for (chessPiece c : v)
    {

        if (c.getPlayer() != player && c.getType() == 0) // enemy cover found
        {
            return true;
        }
    }

    return false;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

int Chess::calculateScore()
{
    Chess copy;
    int score = 0;
    std::vector<chessPiece> matrix[thesize][thesize] = {std::vector<chessPiece>()};

    //Fill matrix with chesspieces and covered cells ////////////////////////////////////////////////////////////////////////////////////////
    matrix[xBK - 1][yBK - 1].push_back(chessPiece(2, 1));
    matrix[xWK - 1][yWK - 1].push_back(chessPiece(1, 1));
    matrix[xWQ - 1][yWQ - 1].push_back(chessPiece(1, queenorrook ? 2 : 3));

    for (int x = 0; x < thesize; x++)
    {
        for (int y = 0; y < thesize; y++)
        {
            if (!matrix[x][y].empty() && matrix[x][y].front().getType() != 0)
            {
                int player = matrix[x][y].front().getPlayer();
                int type = matrix[x][y].front().getType();

                //King coverage
                if (type == 1)
                {
                    // adjacent to x,y
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (!(i == 0 && j == 0) &&
                                (x + i >= 0 && x + i < thesize) &&
                                (y + j >= 0 && y + j < thesize))
                            {
                                matrix[x + i][y + j].push_back(chessPiece(player, 0));
                            }
                        }
                    }
                }

                if (type == 2 || type == 3) //Queen and rook coverage
                {
                    // horizontal
                    for (int i = x + 1; i < thesize; i++)
                    {
                        matrix[i][y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i][y], player))
                            break;
                    }
                    for (int i = x - 1; i >= 0; i--)
                    {
                        matrix[i][y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i][y], player))
                            break;
                    }
                    // vertical
                    for (int j = y + 1; j < thesize; j++)
                    {
                        matrix[x][j].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[x][j], player))
                            break;
                    }
                    for (int j = y - 1; j >= 0; j--)
                    {
                        matrix[x][j].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[x][j], player))
                            break;
                    }
                }

                if (type == 2 || type == 4)  //Queen and bishop coverage
                {
                    // primary diagonal
                    for (int i = -1; ((i + x) >= 0) && ((i + y) >= 0); i--)
                    {
                        matrix[i + x][i + y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i + x][i + y], player))
                            break;
                    }

                    for (int i = 1; ((i + x) < thesize) && ((i + y) < thesize); i++)
                    {
                        matrix[i + x][i + y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i + x][i + y], player))
                            break;
                    }

                    // secondary diagonal
                    for (int i = -1; ((i + x) >= 0) && ((-i + y) < thesize); i--)
                    {
                        matrix[i + x][-i + y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i + x][-i + y], player))
                            break;
                    }
                    for (int i = 1; ((i + x) < thesize) && ((-i + y) >= 0); i++)
                    {
                        matrix[i + x][-i + y].push_back(chessPiece(player, 0));
                        if (foundChessPiece(matrix[i + x][-i + y], player))
                            break;
                    }
                }
            }
        }
    }//End fill matrix//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Calculate score //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int blackPlayer = 2;
    int xBK_ = xBK - 1;
    int yBK_ = yBK - 1;

    // deduct points if the black king is able to move in a bigger square delimited by the squares the queen covers.
    // horizontal
    for (int i = xBK_ + 1; i < thesize; i++)
    {
        if (foundEnemyCover(matrix[i][yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    for (int i = xBK_ - 1; i >= 0; i--)
    {
        if (foundEnemyCover(matrix[i][yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    // vertical
    for (int j = yBK_ + 1; j < thesize; j++)
    {
        if (foundEnemyCover(matrix[xBK_][j], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    for (int j = yBK_ - 1; j >= 0; j--)
    {
        if (foundEnemyCover(matrix[xBK_][j], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    // primary diagonal
    for (int i = -1; ((i + xBK_) >= 0) && ((i + yBK_) >= 0); i--)
    {
        if (foundEnemyCover(matrix[i + xBK_][i + yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    for (int i = 1; ((i + xBK_) < thesize) && ((i + yBK_) < thesize); i++)
    {
        if (foundEnemyCover(matrix[i + xBK_][i + yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    // secondary diagonal
    for (int i = -1; ((i + xBK_) >= 0) && ((-i + yBK_) < thesize); i--)
    {
        if (foundEnemyCover(matrix[i + xBK_][-i + yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    for (int i = 1; ((i + xBK_) < thesize) && ((-i + yBK_) >= 0); i++)
    {
        if (foundEnemyCover(matrix[i + xBK_][-i + yBK_], blackPlayer))
        {
            break;
        }
        else
        {
            score -= thesize;
        }
    }

    //When the WQ has cornered the BK, the WK must come near the BK.
    score -= max(std::abs(xBK - xWK), std::abs(yBK - yWK)) * thesize;

    // the queen must box in the black king, but may not check it.
    // Otherwise black king can simply step to the side and move to whichever 'box' is biggest
    // slightly prevents queen following the king around
    if (incheck(xBK, yBK))
    {
        score -= thesize * thesize;
    }

    // When the black king is sufficently boxed in, a checkmate can be placed
    if (checkmate())
    {
        score = 1000000;
    }

    // simple tie or stalemate
    if (legalforblackking(xWQ, yWQ) ||
        numberofblackmoves() == 0 && !legalforwhitequeen(xBK, yBK))
    {
        score = -1000000;
    }
    //End calculate score///////////////////////////////////////////////////////////////////////////////////////////////////////////

    return score;
}

// do one clever move for white
void Chess::cleverwhitemove()
{
    int depth = 1;
    int score;
    int bestscore = -999999;
    int bestmove = 0;
    Chess copy;
    std::vector<std::vector<int>> pairs;

    for (int k = 0; k < numberofwhitemoves(); k++)
    {
        copy = *this;
        copy.dowhitemove(k);

        score = copy.calculateScore();

        // Dont bother evaluating this move because it results in a loss
        if (score == -1000000)
        {
            continue;
        }
        std::vector<int> pair;
        size_t i = 0;

        //Insert the move at the correct index depending on score
        for (; i < pairs.size(); i++)
        {
            if (score <= pairs[i][0])
            {
                break;
            }
        }

        pair.push_back(score);
        pair.push_back(k);
        pairs.insert(pairs.begin() + i, pair);
    }

    //Calculate best non repeating move
    bool flag;
    do
    {
        std::vector<int> newPosition;
        flag = false;
        copy = *this;
        copy.dowhitemove(pairs.back()[1]); // get best move from top

        // Add all coordinates of pieces to vector
        newPosition.push_back(copy.xBK);
        newPosition.push_back(copy.yBK);
        newPosition.push_back(copy.xWK);
        newPosition.push_back(copy.yWK);
        newPosition.push_back(copy.xWQ);
        newPosition.push_back(copy.yWQ);

        // Check if the new position hasnt already occurred
        for (std::vector<int> oldPosition : positionTransitions)
        {
            bool flag2 = true;
            for (size_t i = 0; i < newPosition.size(); i++)
            {
                // New position is not the same as 1 of the old positions
                if (newPosition[i] != oldPosition[i])
                {
                    flag2 = false;
                    break;
                }
            }
            // 1 of the old positions is the same as the new position
            if (flag2 == true)
            {
                flag = true;
                break;
            }
        }

        //If the position has not yet occurred, do the best move, else do the second best move etc
        if (flag == false)
        {
            // Add a new position to previous positions
            positionTransitions.push_back(newPosition);
        }
        else
        {
            // Remove the best move. The second best move will now be the best move
            pairs.pop_back();
        }
    } while (flag == true);

    dowhitemove(pairs.back()[1]);

} // Chess::cleverwhitemove

// main program
int main(int argc, char *argv[])
{
    int i;
    int stats[5];
    int somesize;
    int simulations;
    int maxgamelength;
    int gametypeW, gametypeB;
    int nrmoves;
    bool print;
    bool queenorrook;
    Chess theboard;
    if (argc == 9)
    {
        somesize = atoi(argv[1]);
        if (somesize > MAX - 2)
        {
            cout << "Size too large!" << endl;
            somesize = MAX - 2;
        } // if
        if (somesize <= 3)
        {
            cout << "Size too small!" << endl;
            somesize = 4;
        } // if
        simulations = atoi(argv[2]);
        maxgamelength = atoi(argv[3]);
        gametypeW = atoi(argv[4]);
        gametypeB = atoi(argv[5]);
        queenorrook = (argv[6][0] == 'Q' || argv[6][0] == 'q');
        print = (atoi(argv[7]) == 1);
        srand(atoi(argv[8]));
    } // if
    else
    {
        cout << "Usage " << argv[0] << " <thesize> <simulations> "
             << "<maxgamelength> <gametypeW> <gametypeB> "
             << "[Q/R] <print> <seed>" << endl;
        return 1;
    } // else
    stats[0] = stats[1] = stats[2] = stats[3] = stats[4] = 0;
    for (i = 0; i < simulations; i++)
    {
        theboard.reset(somesize, queenorrook);
        stats[theboard.playthegame(maxgamelength, print, nrmoves,
                                   gametypeW, gametypeB)]++;
        stats[4] += nrmoves;
    } // for
    cout << "Board size:          " << somesize
         << (queenorrook ? " with Queen" : " with Rook") << endl
         << "Game type white:     " << gametypeW << endl
         << "Game type black:     " << gametypeB << endl
         << "Max game length:     " << maxgamelength << endl
         << "Wins:                " << stats[0] << endl
         << "Stalemates:          " << stats[1] << endl
         << "Simple ties:         " << stats[2] << endl
         << "Stopped:             " << stats[3] << endl
         << "Average game length: " << stats[4] / (double)simulations << endl
         << "Total:               " << simulations << endl
         << endl;
    return 0;
} // main
