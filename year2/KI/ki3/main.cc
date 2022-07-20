// aster2022.cc, C++-program to assess 15-puzzles with A*, ...
//
// Compile: g++ -Wall -Wextra -O2 -o puzzle aster2022.cc
// Run:     ./puzzle <filename> <maxmoves> <scramble> <heuristic>
//                   <method> <games> <print> <seed>
//          <filename> name inputfile with puzzle size and target
//          <maxparam> extra parameter for some maximum 
//          <scramble> 1 perhaps not reachable; 2 reachable
//          <heuristic> 0/1/2/3 number of heuristic
//          <method> 1 bestfirst; 2 A*; 3 IDA*
//          <games> number of games to play
//          <print> 1 print solution(s) if any
//          <seed> seeds random number generator
// Return value 1000000 means: memory issues, or IDA* bound reached;
// and 2000000 means: really unsolvable.
// Walter Kosters, March 22, 2022, w.a.kosters@liacs.leidenuniv.nl
//

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

class ThePuzzle {
  public:
    static const short MAX = 4;    // maximum height & width of the puzzle
    static const short BLACK = 99; // blocked = BLACK square
    static const short ZERO = 0;   // empty = ZERO square
    static const int MEMORY = 50000; // memory size for A*-frontier
    bool readfile (ifstream & inputfile, int parameter);
    void printtarget ( );
    void printcurrent ( );
    void printsequence (ThePuzzle Frontier[ ], int number);
    bool movepossible (short i, short j, short k);
    bool movepossible (short k);
    void domove (short k);
    short numberofmoves ( );
    void randommove ( );
    int bestfirst (short heu, bool print);
    int Astar (short heu, bool print);
    int mainIDAstar (short heu, bool print);
    int IDAstar (short heu, bool print, int boundaryvalue);
    bool finished ( );
    void scramble1 ( );
    void scramble2 ( );
    short Manhattan (short i, short j, short p, short q);
    int heuristic0 ( );
    int heuristic1 ( );
    int heuristic2 ( );
    int heuristic3 ( );
    int heuristic (short k);
  private:
    short data[MAX][MAX];        // the target puzzle
    short current[MAX][MAX];     // the current puzzle
    short height;                // actual height
    short width;                 // and width
    short izero, jzero;          // position of (only) zero in current
    short blacks;                // count BLACKs
    int nrmoves;                 // number of moves so far
    int maxparam;                // parameter for some maximum
    int parent;                  // number of parent node in A*-tree
    bool equal (short A[ ][MAX], short B[ ][MAX]);
};//ThePuzzle

// are two arrays A and B the same?
bool ThePuzzle::equal (short A[ ][MAX], short B[ ][MAX]) {
  short i, j;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( A[i][j] != B[i][j] ) {
	return false;
      }//if
    }//for
  }//for
  return true;
}//ThePuzzle::equal

// read target data from inputfile; return success
// make sure at least one move is possible
// inputfile starts with line with height and width
// 99 is interpreted as BLACK square, 0 is the ONLY "zero"
// equals are also allowed (apart from 0)
// and a max parameter
bool ThePuzzle::readfile (ifstream & inputfile, int parameter) {
  short i, j;
  inputfile >> height;
  inputfile >> width;
  if ( height > MAX || width > MAX ) {
    cout << "Size too large ..." << endl;
    return false;
  }//if
  maxparam = parameter;
  blacks = 0;
  izero = -1;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      inputfile >> data[i][j];
      current[i][j] = data[i][j];
      if ( data[i][j] == ZERO ) {
	if ( izero != -1 ) {
	  return false;
	}//if
	izero = i;
	jzero = j;
      }//if
      else if ( data[i][j] == BLACK ) {
	blacks++;
      }//if
    }//for
  }//for
  inputfile.close ( );
  return ( izero != -1 && numberofmoves ( ) != 0 );
}//ThePuzzle::readfile

// print the target puzzle
void ThePuzzle::printtarget ( ) {
  short i, j;
  cout << "Target position:" << endl;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( data[i][j] == ZERO ) {
	cout << " ZZ ";
      }//if
      else if ( data[i][j] < 10 ) {
	cout << "  " << data[i][j] << " ";
      }//if
      else if ( data[i][j] == BLACK ) {
	cout << " XX ";
      }//if
      else {
	cout << " " << data[i][j] << " ";
      }//else
    }//for
    cout << endl;
  }//for
  cout << endl;
}//ThePuzzle::printtarget

// print the current situation
void ThePuzzle::printcurrent ( ) {
  short i, j;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( current[i][j] == ZERO ) {
	cout << " ZZ ";
      }//if
      else if ( current[i][j] < 10 ) {
	cout << "  " << current[i][j] << " ";
      }//if
      else if ( current[i][j] == BLACK ) {
	cout << " XX ";
      }//if
      else {
	cout << " " << current[i][j] << " ";
      }//else
    }//for
    cout << endl;
  }//for
  cout << "After move " << nrmoves << endl << endl;
}//ThePuzzle::printcurrent

// print ancestors of number from Frontier, starting from the root
void ThePuzzle::printsequence (ThePuzzle Frontier [ ], int number) {
  if ( Frontier[number].nrmoves > 1 ) {
    printsequence (Frontier,Frontier[number].parent);
  }//else
  Frontier[number].printcurrent ( );
}//ThePuzzle::printsequence

// is move of ZERO tile allowed?
// k=0 up; k=1 right; k=2 down; k=3 left
bool ThePuzzle::movepossible (short k) {
  return ( ( k == 0 && izero != 0 && current[izero-1][jzero] != BLACK )
         || ( k == 1 && jzero != width-1 && current[izero][jzero+1] != BLACK )
         || ( k == 2 && izero != height-1 && current[izero+1][jzero] != BLACK )
         || ( k == 3 && jzero != 0 && current[izero][jzero-1] != BLACK ) );
}//ThePuzzle::movepossible

// do allowed move
// k=0 up; k=1 right; k=2 down; k=3 left
void ThePuzzle::domove (short k) {
  if ( k == 0 ) {
    current[izero][jzero] = current[izero-1][jzero];
    current[izero-1][jzero] = ZERO;
    izero--;
  }//if
  else if ( k == 1 ) {
    current[izero][jzero] = current[izero][jzero+1];
    current[izero][jzero+1] = ZERO;
    jzero++;
  }//if
  else if ( k == 2 ) {
    current[izero][jzero] = current[izero+1][jzero];
    current[izero+1][jzero] = ZERO;
    izero++;
  }//if
  else { // k== 3
    current[izero][jzero] = current[izero][jzero-1];
    current[izero][jzero-1] = ZERO;
    jzero--;
  }//else
  nrmoves++;
}//ThePuzzle::domove

// how many moves can be made (0/1/2/3/4)?
short ThePuzzle::numberofmoves ( ) {
  short counter = 0, k;
  for ( k = 0; k < 4; k++ ) {
    if ( movepossible (k) ) {
      counter++;
    }//if
  }//for
  return counter;
}//ThePuzzle::numberofmoves

// do random move
void ThePuzzle::randommove ( ) {
  short k, nr = rand ( ) % numberofmoves ( );
  for ( k = 0; k < 4; k++ ) {
    if ( movepossible (k) ) { 
      if ( nr == 0 ) {
	domove (k);
	return;
      }//if
      nr--;
    }//if
  }//for
}//ThePuzzle::randommove

// best first with heuristic heu, print gives extra output
int ThePuzzle::bestfirst (short heu, bool print) {
  short k, ran;
  int bestk, bestvalue, number;
  ThePuzzle copy;
  int results[4];
  while ( ! finished ( ) ) {
    bestvalue = 10000000;
    number = 0;
    bestk = -1;
    for ( k = 0; k < 4; k++ ) {
      if ( movepossible (k) ) {
        copy = *this;
        copy.domove (k);
        results[k] = copy.heuristic (heu);
        if ( results[k] == bestvalue ) {
	  number++;
        }//if
        else if ( results[k] < bestvalue ) {
  	  bestvalue = results[k];
	  bestk = k;
	  number = 1;
        }//if
      }//if
      else {
        results [k] = 1000000;
      }//else
    }//for
    if ( number > 1 ) {
      ran = rand ( ) % number;
      for ( k = 0; k < 4; k++ ) {
        if ( results[k] == bestvalue ) {
	  if ( ran == 0 ) {
            bestk = k;
	  }//if
	  ran--;
        }//if
      }//for
    }//if
    domove (bestk);
    if ( print ) {
      printcurrent ( );
    }//if
  }//while
  return nrmoves;
}//ThePuzzle::bestfirst

// A* with heuristic heu, print gives extra output
int ThePuzzle::Astar (short heu, bool print) {
  ThePuzzle Frontier[MEMORY]; // can be LARGE
  int heuristics[MEMORY];
  short k;
  int first = 0, last = 0, i, besti, bestf, p, temp, fvalue;
  bool newone;
  Frontier[0] = *this;
  heuristics[0] = heuristic (heu);
  Frontier[0].parent = -1;
  ThePuzzle bestone;
  ThePuzzle copy;
  while ( first <= last ) { // non-empty frontier
    bestf = 10000000;
    besti = 0;
    for ( i = first; i <= last; i++ ) {
      fvalue = Frontier[i].nrmoves + heuristics[i];
      if ( fvalue < bestf ) {
        besti = i;
	bestf = fvalue;
      }//if
      else if ( fvalue == bestf ) {
	if ( Frontier[besti].nrmoves > Frontier[i].nrmoves ) {
          besti = i;
	}//if
      }//if
    }//for
    if ( Frontier[besti].finished ( ) ) {
      if ( print ) {
	//cout << "Solution found" << endl;
        printsequence (Frontier,besti);
      }//if
      return Frontier[besti].nrmoves; 
    }//if
    //cout << "Last " << last << "; best f " << bestf << endl;
    bestone = Frontier[besti];
    Frontier[besti] = Frontier[first];
    Frontier[first] = bestone;
    temp = heuristics[besti];
    heuristics[besti] = heuristics[first];
    heuristics[first] = temp;
    first++;
    for ( k = 0; k < 4; k++ ) {
      if ( bestone.movepossible (k) ) { 
	copy = bestone;
	copy.domove (k);
        newone = true;
	for ( p = 0; newone && p <= last; p++ ) {
          if ( equal (Frontier[p].current,copy.current) ) {
            newone = false;
	  }//if
	}//for
	if ( newone && copy.nrmoves + copy.heuristic (heu) <= maxparam ) {
	  last++;
	  copy.parent = first-1;
	  Frontier[last] = copy; 
	  heuristics[last] = copy.heuristic (heu);
	}//if
      }//if
    }//for
    if ( last > MEMORY - 4 ) {
      if ( print ) {
	cout << "Memory problems" << endl;
      }//if
      return 1000000;
    }//if
  }//while
  if ( print ) {
    cout << "Impossible ..." << endl;
  }//if
  return 2000000;
}//ThePuzzle::Astar

// IDA* with heuristic heu, print gives extra output
int ThePuzzle::mainIDAstar (short heu, bool print) {
  int boundaryvalue = heuristic (heu);
  int result;
  while ( boundaryvalue <= maxparam ) {
    result = IDAstar (heu,print,boundaryvalue);
    if ( result < 1000000 ) {
      return result;
    }//if
    boundaryvalue++;
  }//while
  return 1000000;
}//ThePuzzle::mainIDAstar

// IDA* with heuristic heu, print gives extra output
// expand all nodes with f <= boundaryvalue, including the root
int ThePuzzle::IDAstar (short heu, bool print, int boundaryvalue) {
  ThePuzzle copy;
  short k;
  int myvalue, myresult;
  if ( finished ( ) ) {
    return nrmoves;
  }//if
  for ( k = 0; k < 4; k++ ) {
    if ( movepossible (k) ) { 
      copy = *this;
      copy.domove (k);
      myvalue = copy.nrmoves + copy.heuristic (heu);
      if ( myvalue <= boundaryvalue ) {
        myresult = copy.IDAstar (heu,false,boundaryvalue);
	if ( myresult < 1000000 ) {
	  if ( print ) {
	    copy.printcurrent ( );
	    myresult = copy.IDAstar (heu,true,boundaryvalue);
	  }//if
	  return myresult;
	}//if
      }//if
    }//if
  }//for
  return 1000000;
}//ThePuzzle::IDAstar

// is puzzle ready (data == current, or some maxmoves reached)?
// notice that both could be true
bool ThePuzzle::finished ( ) {
  short i, j;
  if ( nrmoves >= 5 * maxparam ) {
    return true;
  }//if
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( data[i][j] != current[i][j] ) {
	return false;
      }//if
    }//for
  }//for
  return true;
}//ThePuzzle::finished

// produce initial random board with same numbers as target
// perhaps target is unreachable from here
void ThePuzzle::scramble1 ( ) {
  short thenumbers[MAX*MAX];
  short i, j, temp, r;
  do {
    r = 0;
    for ( i = 0; i < height; i++ ) {
      for ( j = 0; j < width; j++ ) {
        if ( data[i][j] != BLACK ) {
	  thenumbers[r] = data[i][j];
	  r++;
        }//if
      }//for
    }//for
    for ( j = height * width - blacks - 1; j > 0; j-- ) {
      r = rand ( ) % (j+1);
      temp = thenumbers[j];
      thenumbers[j] = thenumbers[r];
      thenumbers[r] = temp;
    }//for
    r = 0;
    for ( i = 0; i < height; i++ ) {
      for ( j = 0; j < width; j++ ) {
        if ( data[i][j] == BLACK ) {
	  current[i][j] = BLACK;
        }//if
        else {
	  current[i][j] = thenumbers[r];
	  if ( thenumbers[r] == ZERO ) {
            izero = i;
	    jzero = j;
	  }//if
	  r++;
        }//else
      }//for
    }//for
  } while ( numberofmoves ( ) == 0 );
  nrmoves = 0;
}//ThePuzzle::scramble1

// produce initial random board with same numbers as target
// certainly reachable
void ThePuzzle::scramble2 ( ) {
  short i, j;
  int number = 200 + rand ( ) % 100;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      current[i][j] = data[i][j];
      if ( data[i][j] == ZERO ) {
	izero = i;
	jzero = j;
      }//if
    }//for
  }//for
  for ( i = 0; i < number; i++ ) {
    randommove ( );
  }//for
  nrmoves = 0;
}//ThePuzzle::scramble2

// Manhattan distance between (i,j) and (p,q)
short ThePuzzle::Manhattan (short i, short j, short p, short q) {
  if ( i >= p ) {
    if ( j >= q ) {
      return ( i-p + j-q );
    }//if
    else {
      return ( i-p + q-j );
    }//else
  }//if
  else {
    if ( j >= q ) {
      return ( p-i + j-q );
    }//if
    else {
      return ( p-i + q-j );
    }//else
  }//else
}//ThePuzzle::Manhattan

// heuristic 0: always zero
int ThePuzzle::heuristic0 ( ) {
  return 0;
}//ThePuzzle::heuristic0

// heuristic 1: number of misplaced tiles (apart from ZERO)
int ThePuzzle::heuristic1 ( ) {
  short i, j, counter = 0;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( data[i][j] != current[i][j] && current[i][j] != ZERO ) {
	counter++;
      }//if
    }//for
  }//for
  return counter;
}//ThePuzzle::heuristic1

// heuristic 2: total misplaced Manhattan distances (apart from ZERO)
int ThePuzzle::heuristic2 ( ) {
  short i, j, p, q, dist;
  int counter = 0, shortest;
  for ( i = 0; i < height; i++ ) {
    for ( j = 0; j < width; j++ ) {
      if ( current[i][j] != ZERO ) {
        shortest = 1000000;
	// notice that the same number can occur more than once
        for ( p = 0; p < height; p++ ) {
	  for ( q = 0; q < width; q++ ) {
	    if ( current[i][j] == data[p][q] ) {
	      dist = Manhattan (i,j,p,q);
	      if ( dist < shortest ) {
	        shortest = dist;
	      }//if
	    }//if
	  }//for
        }//for
        counter += shortest;
      }//if
    }//for
  }//for
  return counter;
}//ThePuzzle::heuristic2

// heuristic 3: TODO
int ThePuzzle::heuristic3 ( ) {
  return 0;
}//ThePuzzle::heuristic3

// select heuristic 0/1/2 or even 3
int ThePuzzle::heuristic (short k) {
  if ( k == 0 ) {
    return heuristic0 ( );
  }//if
  else if ( k == 1 ) {
    return heuristic1 ( );
  }//
  else if ( k == 2 ) {
    return heuristic2 ( );
  }//else
  else {
    return heuristic3 ( );
  }//else
}//ThePuzzle::heuristic

// main program
int main (int argc, char* argv[ ]) {
  if ( argc != 9 ) {
    cout << "Usage: " << argv[0] 
	 << " <filename> <maxparam> <scramble> <heuristic> "
	 << "<method> <games> <print> <seed>" << endl;
    return 1;
  }//if
  string filename = argv[1];
  int maxparameter = atoi (argv[2]);
  short scrambletype = atoi (argv[3]);
  short heu = atoi (argv[4]);
  short method = atoi (argv[5]);
  int games = atoi (argv[6]);
  bool print ( atoi (argv[7]) == 1 );
  int seed = atoi (argv[8]);
  srand (seed);
  ifstream thefile;
  thefile.open (filename.c_str ( ),ios::in);
  if ( thefile.fail ( ) ) {
    cout << "Could not open file " << filename << endl;
    return 1;
  }//if
  cout << endl;
  ThePuzzle mypuzzle;
  int i;
  int result;
  int stats[100] = {0};
  int statslarge = 0;
  if ( ! mypuzzle.readfile (thefile,maxparameter) ) {
    cout << "Something wrong with the puzzle" << endl;
    return 1;
  }//if

  for ( i = 0; i < games; i++ ) {
    if ( print ) {
      mypuzzle.printtarget ( );
    }//if
    if ( scrambletype == 1 ) {
      mypuzzle.scramble1 ( );
    }//if
    else {
      mypuzzle.scramble2 ( );
    }//else
    if ( print ) {
      cout << "Starting position:" << endl;
      mypuzzle.printcurrent ( );
      cout << "==================" << endl << endl;
    }//if
    if ( method == 1 ) {
      result = mypuzzle.bestfirst (heu,print);
    }//if
    else if ( method == 2 ) {
      result = mypuzzle.Astar (heu,print);
    }//if
    else {
      result = mypuzzle.mainIDAstar (heu,print);
    }//else
    cout << i << " " << result << endl;
    if ( result < 100 ) {
      stats[result]++;
    }//if
    else {
      statslarge++;
    }//else
  }//for

  cout << endl;
  for ( i = 10; i < 32; i++ ) {
    cout << i << " " << stats[i] << endl;
  }//for
  cout << "Large: " << statslarge << endl;
  return 0;
}//main
