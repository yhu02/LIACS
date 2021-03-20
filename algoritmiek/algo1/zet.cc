// Implementatie van klasse Zet

#include "standaard.h"
#include "zet.h"
#include <iostream>
using namespace std;

//*************************************************************************

Zet::Zet ()
{
  i = 0;
  r = 0;
  rij = 0;
  kolom = 0;
  som = 0;

}  // default constructor

//*************************************************************************

void Zet::setDefaultWaardes ()
{
  i = -1;
  r = -1;
  rij = -1;
  kolom = -1;
  som = -1;

}  // setDefaultWaardes

//*************************************************************************

void Zet::setWaardes (int i0, int r0, int rij0, int kolom0, int som0)
{
  if (i0>=0)
    i = i0;
  else
  { i = 0;
    cout << endl;
    cout << "i0 mag niet negatief zijn, maar is " << i0 << endl;
  }

  if (integerInBereik ("r0", r0, 0, 3))
    r = r0;
  else
    r = 0;

  if (rij0>=0)
    rij = rij0;
  else
  { rij = 0;
    cout << endl;
    cout << "rij0 mag niet negatief zijn, maar is " << rij0 << endl;
  }

  if (kolom0>=0)
    kolom = kolom0;
  else
  { kolom = 0;
    cout << endl;
    cout << "kolom0 mag niet negatief zijn, maar is " << kolom0 << endl;
  }

  if (som0>=0)
    som = som0;
  else
  { som = 0;
    cout << endl;
    cout << "som0 mag niet negatief zijn, maar is " << som0 << endl;
  }

}  // setWaardes

//*************************************************************************

int Zet::getI ()
{
  return i;

}  // getI

//*************************************************************************

int Zet::getR ()
{
  return r;

}  // getR

//*************************************************************************

int Zet::getRij ()
{
  return rij;

}  // getRij

//*************************************************************************

int Zet::getKolom ()
{
  return kolom;

}  // getKolom

//*************************************************************************
int Zet::getSom ()
{
  return som;

}  // getKolom

//*************************************************************************

void Zet::drukAf ()
{
  cout << "(steen " << i << ", rotatie " << r << ", vakje ["
       << rij << "," << kolom << "])" << endl;

}  // drukAf

