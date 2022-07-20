
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

double fchess(double input[], double sampledata[])
{
  for (int j = 0; j < 6; j++)
  {
    input[j] = sampledata[j];
  }
  return sampledata[6];
}

double fgatetype(int gatetype, int x, int y)
{
  if (gatetype == 1) // xor
  {
    return x ^ y;
  }
  else if (gatetype == 2) // or
  {
    return x || y;
  }
  else if (gatetype == 3) // and
  {
    return x && y;
  }
  else
  {
    std::cout << "Invalid gatetype";
    exit(1);
  }
}

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

  bool chessflag;            // Verander naar schaakmode
  bool testflag;             // Verander naar testing mode
  int gatetype = 3;          // XOR = 1, OR = 2, AND = 2
  int inputsize = 7;         // chess dataset inputs
  const int SAMPLES = 25260; // chess dataset aantal voorbeelden

  double ratio = 0.9; // ratio (train dataset + test dataset) / chessdataset = 1
  int trainsize = ratio * SAMPLES;
  int testsize = (1 - ratio) * SAMPLES;

  double chesstrain[trainsize][inputsize];
  double chesstest[testsize][inputsize];

  int correct = 0;          // aantal resultaten binnen 5% marge van target
  double errorcount = 0;    // fout totaal
  double errorcountabs = 0; // fout totaal absoluut

  int itrain = 0;
  int itest = 0;

  if (inputs == 6)
  {
    chessflag = true;
  }
  else
  {
    chessflag = false;
  }

  if (chessflag)
  {
    // LATER TODO-7 lees de gehele Chess dataset:
    ifstream chessdata("chessdatanodraw.txt", ios::in);
    if (chessdata.fail())
    {
      cout << "Inputfile bestaat niet!" << endl;
    }

    for (int i = 0; i < SAMPLES; i++)
    {
      if (i % int(1 / (1 - ratio)) == 0)
      {
        for (int j = 0; j < inputsize; j++)
        {
          chessdata >> chesstest[itest][j];
          chesstest[itest][j] = chesstest[itest][j] / 16.0;
        }
        itest++;
      }
      else
      {
        for (int j = 0; j < inputsize; j++)
        {
          chessdata >> chesstrain[itrain][j];
          chesstrain[itrain][j] = chesstrain[itrain][j] / 16.0;
        }
        itrain++;
      }
    }
    chessdata.close();
  }
  // TODO-1 initialiseer de gewichten random tussen -1 en 1:
  for (int i = 0; i < MAX; i++)
  {
    for (int j = 0; j < MAX; j++)
    {
      inputtohidden[i][j] = double(rand() - RAND_MAX / 2) / RAND_MAX;
    }
    hiddentooutput[i] = double(rand() - RAND_MAX / 2) / RAND_MAX;
  }

  for (int d = 0; d < 2; d++) // d = 0 is train dataset, d = 1 is test dataset
  {
    // Training data set
    int nloops;
    if (d == 0)
    {
      testflag = false;
      nloops = epochs;
    }
    else if (d == 1)
    {
      testflag = true;
      nloops = testsize;
    }

    for (int i = 0; i < nloops; i++)
    {
      inoutput = 0;
      netoutput = 0;
      // TODO-2 lees voorbeeld in naar input/target, of genereer ter plekke:
      if (inputs == 2) // xor
      {
        bool x = rand() % 2;
        bool y = rand() % 2;

        input[1] = x;
        input[2] = y;

        target = fgatetype(gatetype, x, y);
      }
      else if (inputs == 6) // chess
      {
        if (testflag)
        {
          int sample = rand() % testsize;
          target = fchess(input, chesstest[sample]);
        }
        else
        {
          int sample = rand() % trainsize;
          target = fchess(input, chesstrain[sample]);
        }
      }
      else
      {
        std::cout << "unsupported input size" << std::endl;
        exit(1);
      }

      // TODO-3 stuur het voorbeeld door het netwerk
      for (int n = 1; n <= hiddens; n++)
      {
        inhidden[n] = 0;
        for (int m = 0; m <= inputs; m++)
        {
          inhidden[n] += input[m] * inputtohidden[m][n];
        }
        acthidden[n] = g(inhidden[n], type);
      }

      for (int j = 0; j < hiddens; j++)
      {
        inoutput += acthidden[j] * hiddentooutput[j];
      }

      netoutput = g(inoutput, type);

      // TODO-4 bereken error, delta, en deltahidden
      error = target - netoutput;

      if (!testflag) // verander gewichten tijdens training
      {
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
      }

      if (testflag) // uitvoer statistieken
      {
        if (std::abs(error) <= 0.05)
        {
          correct++;
        }
        // voor statistics
        errorcount += error;
        errorcountabs += std::abs(error);

        // TODO-6 beoordeel het netwerk en rapporteer
        if (i >= testsize - 1000)
        {
          std::cout << "_____________________" << std::endl;
          std::cout << "Iterations: " << i << std::endl;
          std::cout << "Error: " << error << std::endl; // error voor de iteratie
          std::cout << "Netoutput: " << netoutput << std::endl;
          std::cout << "Target: " << target << std::endl;
          std::cout << "_____________________" << std::endl;
        }
      }
    }
  }

  double erroravg = errorcount / testsize;
  double erroravgabs = errorcountabs / testsize;

  std::cout << "_____________________" << std::endl;
  std::cout << "Percentage correct: " << (double(correct) / testsize) * 100 << "%" << std::endl;
  std::cout << "Error gemiddeld: " << erroravg << std::endl;             // gemiddelde error
  std::cout << "Error gemiddeld absoluut: " << erroravgabs << std::endl; // gemiddelde error absoluut
  std::cout << "_____________________" << std::endl;

  return 0;
} // main
