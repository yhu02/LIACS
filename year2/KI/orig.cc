
//
// nnskelet.cc
// C++-programma voor neuraal netwerk (NN) met \'e\'en output-knoop
// TODO: 1,2,3,4,5,6,7
// Zie www.liacs.leidenuniv.nl/~kosterswa/AI/nnhelp.pdf
// 11 april 2022
// Compileren: g++ -Wall -O2 -o nn nnskelet.cc
// Gebruik:    ./nn <inputs> <hiddens> <epochs> <type> <seed>
// Voorbeeld:  ./nn 2 3 100000 1 1234
// Let op: de inputs komen in input[1],...,input[inputs] (tot en
// met inputs) en de verborgen knopen worden geindiceerd met
// 0,1,...,hiddens (tot en met hiddens)
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

const int MAX = 20;
const double ALPHA = 0.2;
const double BETA = 1.0;

// activatie-functie (sigmoid)
double sigmoid(double x)
{
  return 1 / (1 + exp(-BETA * x));
} // sigmoid

// afgeleide van sigmoid
double sigmoidprime(double x)
{
  return BETA * sigmoid(x) * (1 - sigmoid(x));
} // sigmoidprime

// activatie-functie (softplus)
double softplus(double x)
{
  return log(1 + exp(x));
} // softplus

// afgeleide van softplus
double softplusprime(double x)
{
  return exp(x) / (1 + exp(x));
} // softplusprime

// activatie-functie (ReLU)
double ReLU(double x)
{
  if (x >= 0)
  {
    return x;
  } // if
  return 0;
} // ReLU

// "afgeleide" van ReLU
double ReLUprime(double x)
{
  if (x >= 0)
  {
    return 1;
  } // if
  return 0;
} // ReLUprime

// activatie-functie van verschillend type
double g(double x, int type)
{
  if (type == 1)
  {
    return sigmoid(x);
  } // if
  else if (type == 2)
  {
    return softplus(x);
  } // if
  else if (type == 3)
  {
    return ReLU(x);
  } // if
  return 0;
} // g

// afgeleide van activatie-functie van verschillend type
double gprime(double x, int type)
{
  if (type == 1)
  {
    return sigmoidprime(x);
  } // if
  else if (type == 2)
  {
    return softplusprime(x);
  } // if
  else if (type == 3)
  {
    return ReLUprime(x);
  } // if
  return 0;
} // gprime

int main(int argc, char *argv[])
{

  int inputs, hiddens;            // aantal invoer- en verborgen knopen
                                  // inputs is exclusief de bias-knoop!
  double input[MAX];              // de invoer is input[1]...input[inputs]
  double inputtohidden[MAX][MAX]; // gewichten van invoerknopen 0..inputs
                                  // naar verborgen knopen 1..hiddens
  double hiddentooutput[MAX];     // gewichten van verborgen knopen 0..hiddens
                                  // naar de ene uitvoerknoop
  double inhidden[MAX];           // invoer voor de verborgen knopen 1..hiddens
  double acthidden[MAX];          // en de uitvoer daarvan
  double inoutput;                // invoer voor de ene uitvoerknoop
  double netoutput;               // en de uitvoer daarvan: de net-uitvoer
  double target;                  // gewenste uitvoer
  double error;                   // verschil tussen gewenste en
                                  // geproduceerde uitvoer
  double delta;                   // de delta voor de uitvoerknoop
  double deltahidden[MAX];        // de delta's voor de verborgen
                                  // knopen 1..hiddens
  int epochs;                     // aantal trainingsvoorbeelden
  int type;                       // welke activatie-functie?
  int seed;                       // voor random number generator

  if (argc != 6)
  {
    cout << "Gebruik: " << argv[0] << " <inputs> <hiddens> <epochs> "
         << "<type> <seed>" << endl;
    return 1;
  } // if
  inputs = atoi(argv[1]);
  if (inputs >= MAX)
  {
    cout << "Aantal inputs < " << MAX << "!" << endl;
    return 1;
  } // if
  hiddens = atoi(argv[2]);
  if (hiddens >= MAX)
  {
    cout << "Aantal verborgen knopen < " << MAX << "!" << endl;
    return 1;
  } // if
  epochs = atoi(argv[3]);
  type = atoi(argv[4]);
  seed = atoi(argv[5]);
  input[0] = -1;     // invoer bias-knoop: altijd -1
  acthidden[0] = -1; // verborgen bias-knoop: altijd -1
  srand(seed);

  // LATER TODO-7 lees de gehele Chess dataset:
    ifstream chessdata ("chessdatanodraw.txt",ios::in);
    if ( chessdata.fail ( ) ) {
      cout << "Inputfile bestaat niet!" << endl;
      return 1;
    }//if
    const int SAMPLES = 25260;
    double chess[SAMPLES][7];
    for ( int i = 0; i < SAMPLES; i++ ) {
      for ( int j = 0; j < 7; j++ ) {
        chessdata >> chess[i][j];
        chess[i][j] = chess[i][j] / 16.0;
      }//for
    }//for
  //  // vergeet niet te normaliseren!!!
    chessdata.close ( );

  // TODO-1 initialiseer de gewichten random tussen -1 en 1:
  //  inputtohidden en hiddentooutput
  //  rand ( ) levert geheel randomgetal tussen 0 en RAND_MAX; denk aan casten

  for (int i = 0; i < MAX; i++)
  {
    for (int j = 0; j < MAX; j++)
    {
      inputtohidden[i][j] = double(rand() - RAND_MAX / 2) / RAND_MAX;
    }
    hiddentooutput[i] = double(rand() - RAND_MAX / 2) / RAND_MAX;
  }

  double errorcount = 0;

  for (int cnt = 0; cnt < epochs; cnt++)
  {

    // TODO-2 lees voorbeeld in naar input/target, of genereer ter plekke:
    //  als voorbeeld: de XOR-functie, waarvoor geldt dat inputs = 2
    //  int x = rand ( ) % 2; int y = rand ( ) % 2; int dexor = ( x + y ) % 2;
    //  input[1] = x; input[2] = y; target = dexor;

    //dexor stuk
    if (inputs == 2){
      int x = rand() % 2;
      int y = rand() % 2;
      int dexor = (x + y) % 2;
      input[1] = x;
      input[2] = y;
      target = dexor;
    }

    //chess stuk
    else if (inputs == 6){
      // LATER TODO-7 Chess, met inputs = 6:
      int mysample = rand ( ) % SAMPLES;
      for ( int j = 1; j <= 6; j++ ) {
        input[j] = chess[mysample][j-1];
      }//for
      target = chess[mysample][6];
    }

    // TODO-3 stuur het voorbeeld door het netwerk
    //  reken inhidden's uit, acthidden's, inoutput en netoutput

    for (int n = 1; n <= hiddens; n++)
    {
      inhidden[n] = 0;
      for (int m = 0; m <= inputs; m++)
      {
        inhidden[n] += input[m] * inputtohidden[m][n];
      }
      acthidden[n] = g(inhidden[n], type);
    }

    inoutput = 0;
    for (int m = 0; m < hiddens; m++)
    {
      inoutput += acthidden[m] * hiddentooutput[m];
    }

    netoutput = g(inoutput, type);

    // TODO-4 bereken error, delta, en deltahidden
    error = target - netoutput;

    //voor statistics
    errorcount += error;
    double errorgem = errorcount/cnt;
    double errorgem2 = 0;
    if (cnt >= epochs-1000){
      errorcount += error;
      errorgem2 = errorcount/cnt;
    }


    delta = error * gprime(inoutput, type);

    for (int j = 0; j < hiddens; j++)
    {
      deltahidden[j] = gprime(inhidden[j], type) * hiddentooutput[j] * delta;
    }
    // TODO-5 update gewichten hiddentooutput en inputtohidden
    for (int j = 0; j < hiddens; j++)
    {
      hiddentooutput[j] = hiddentooutput[j] + ALPHA * acthidden[j] * delta;
    }

    for (int j = 0; j <= hiddens; j++)
    {
      for (int k = 0; k <= inputs; k++)
      {
        inputtohidden[k][j] = inputtohidden[k][j] + ALPHA * input[k] * deltahidden[j];
      }
    }

    // TODO-6 beoordeel het netwerk en rapporteer
    //  bijvoorbeeld (totaal)fout bij laatste 1000 voorbeelden

    if (cnt >= epochs - 1000)
    {
      std::cout << "_____________________" << std::endl;
      std::cout << "Iterations: " << cnt << std::endl;
      std::cout << "Error: " << error << std::endl; //error voor de iteratie
      std::cout << "Errortotaal: " << errorgem << std::endl; //gemiddelde error alles
      std::cout << "Error1000: " << errorgem2 << std::endl; //gemiddelde error laatste 1000
      std::cout << "Netoutput: " << netoutput << std::endl;
      std::cout << "Target: " << target << std::endl;
      std::cout << "_____________________" << std::endl;
      // std::cout << "Error 1000: " << errorcountd << std::endl;
    }

    inoutput = 0;
    netoutput = 0;

  } // for

  return 0;
} // main
