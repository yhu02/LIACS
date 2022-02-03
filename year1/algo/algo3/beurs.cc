// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "standaard.h"
#include "beurs.h"

using namespace std;
 
  
//*************************************************************************

// Default constructor
Beurs::Beurs ()
{
  this->tw = 0;
  this->n = 0;
  this->b0 = 0;
  this->a = 0;
}  // default constructor

//****************************************************************************

// Default destructor
Beurs::~Beurs ()
{
}  // default destructor

bool Beurs::leesIn (const char* invoernaam)
{
  std::ifstream invoer;      
  
  invoer.open(invoernaam, std::ios::in);
  if(invoer.fail())
  {
    std::cout << "Bestand bestaat niet";
    return false;
  } 

  invoer >> this->tw >> this->n;
  invoer >> this->p;
  invoer >> this->b0;

  //Check invoer volgens opdracht
  if(!(1 <= this->tw && tw <= 100)   || 
    !(1 <= this->n && this->n <= 8)  ||
    !(0 <= this->p && this->p <= 50) ||
    !(this->b0 >= 0))
  {
    std::cout << "Invoer is ongeldig";
    return false;
  }

  //Vul vector koersen
  
  for(int t = 0; t < (this->tw + 1); t++)
  {
    //Doe alle koersen in een vector
    std::vector<double> koersDag;

    for(int i = 0; i < this->n; i++)
    {
      //Prijs is de waarde van het aandeel van bedrijf i
      double prijs;
      invoer >> prijs;

      //Check invoer volgens opdracht
      if(!(0 <= prijs && prijs <= 100))
      {
        std::cout << "Invoer is ongeldig";
        return false;
      }
      koersDag.push_back(prijs);
    }

    //Doe elke koersen vector in een vector
    this->koers.push_back(koersDag);
  }
  
  //Vul vector rente
  for(int t = 0; t < this->tw; t++)
  {
    double rente;
    invoer >> rente;

    //Check invoer volgens opdracht
    if(!(-10 <= rente && rente <= 100))
    {
      std::cout << "Invoer is ongeldig";
      return false;
    }

    this->rentes.push_back(rente);
  }
  return true;

}  // leesIn

//****************************************************************************

void Beurs::drukAfInvoer ()
{
  std::cout << "tw:" << this->tw << std::endl;
  std::cout << "n:"  << this->n  << std::endl;
  std::cout << "provisie:" << this->p << std::endl;
  std::cout << "b0:" << this->b0 << std::endl;
  std::cout << std::endl;

  for(int t = 0; t < (this->tw + 1); t++)
  {
    std::cout << "dag:" << t << std::endl;
    for(int i = 0; i < this->n; i++)
    {
      std::cout << this->koers[t][i] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "rentes:" << std::endl;
  for(int t = 0; t < this->tw; t++)
  {
    std::cout << this->rentes[t] << " ";
  }
  std::cout << std::endl << std::endl;
}  // drukAfInvoer

//****************************************************************************

double Beurs::bepaalMaxBedragBU
(vector <vector <pair <bool,int> > > &transacties)
{
  for(int t = 0; t < this->tw; t++)
  {
    double maxb0= -1;                         //Max bedrag na dag t
    int maxa = -1;                            //Bitstring aandelenbezit lopend van rechts naar links aan het eind van dag t

    this->berekenMaxb0(t, maxa, maxb0);
    this->voegToeDagTransacties(maxa, transacties);

    //Wijs de beste combinatie aandelen en het bijbehorende bedrag toe 
    this->a = maxa;
    this->b0 = maxb0;
  }
  
  double eindb0 = this->b0;

  //Reset
  this->b0 = this->bedrag[0][0];
  this->a = 0;

  return eindb0;
}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRec (bool memo)
{
  //Eerste aanroep recursieve functie
  hulpRecFunctie(this->tw - 1);
  
  double eindb0 = this->b0;
  
  //Reset
  this->b0 = this->bedrag[0][0];
  this->a = 0;
  this->bedrag.clear();

  return eindb0;
}  // bepaalMaxBedragRec (memo)

void Beurs::hulpRecFunctie (int t)
{
  double maxb0 = -1;                         //Max bedrag na dag t
  int maxa = -1;                            //Bitstring aandelenbezit lopend van rechts naar links aan het eind van dag t
  
  //Recursie tot t = 0
  if(t != 0)
  {
    hulpRecFunctie(t-1);
  }
  
  //Bereken max b0 op dag t
  this->berekenMaxb0(t, maxa, maxb0);

  //Wijs de beste combinatie aandelen en het bijbehorende bedrag toe 
  this->a = maxa;
  this->b0 = maxb0;

  return;;
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
(vector <vector <pair <bool,int> > > &transacties)
{
  double b0 = this->bedrag[0][0];
  double pMult = this->p / 100;
  int a = 0;
  std::cout << std::endl;
  
  for(size_t t = 0; t < transacties.size(); t++)
  {
    double renteMult = this->rentes[t] / 100;
  
    std::cout << "==================================" << std::endl;
    std::cout << "Dag:" << t << std::endl;
    std::cout << "Startbedrag: " << b0 << std::endl;
    std::cout << "Startbezit: ";
    for(int aandeel = 0; aandeel < this->n; aandeel++)
    {
      int aandeelBit =  std::pow(2,aandeel);
      if(a & aandeelBit)
      {
        std::cout << "[" << aandeel << "]";
      }
    }
    std::cout << std::endl << std::endl;
    for(size_t j = 0; j < transacties[t].size(); j++)
    {
      int aandeel = transacties[t][j].second;
      if(transacties[t][j].first == false)
      {
        std::cout << "Verkoop: [" << aandeel << "]";
        a  -= std::pow(2,aandeel);
        b0 += this->koers[t][aandeel];
        b0 -= this->koers[t][aandeel] * pMult;

      }else if(transacties[t][j].first == true){
        std::cout << "Koop: [" << aandeel << "]";
        a += std::pow(2,aandeel);
        b0 -= this->koers[t][aandeel];
        b0 -= this->koers[t][aandeel] * pMult;
      }
      std::cout << std::endl;
    }

    std::cout << std::endl << "Eindbedrag: " << b0 << std::endl;
    b0 = b0 * (1 + renteMult);
  }
  transacties.clear();
  this->bedrag.clear();
}  // drukAfTransacties

//****************************************************************************

void Beurs::voegToeDagTransacties
(int a, std::vector <std::vector <std::pair <bool,int> > > &transacties)
{
  std::vector<std::pair<bool, int>> dagTransacties;    //Vector met alle transacties van de dag

  //Check of een aandeel is gekocht of verkocht
  for(int aandeel = 0; aandeel < this->n; aandeel++)
  {
    int aandeelBit = std::pow(2,aandeel);              //Bitwaarde van een aandeel
    std::pair<bool, int> pair1;                        //Transactie pair

    pair1.second = aandeel;

    //Check of er een aandeel is gekocht
    if(((a & aandeelBit) == aandeelBit) &&
      ((this->a & aandeelBit) == 0)){
      pair1.first = 1;
      dagTransacties.push_back(pair1);
    //Check of er een aandeel is verkocht
    }else if(((a & aandeelBit) == 0) &&
            ((this->a & aandeelBit) == aandeelBit)){
      pair1.first = 0;
      dagTransacties.push_back(pair1);
    }       
  }
  transacties.push_back(dagTransacties);
}

//****************************************************************************

void Beurs::berekenMaxb0
(int t, int &maxa, double &maxb0)
{
  double renteMult = this->rentes[t] / 100;           //Rente op dag t
  double pMult = this->p / 100;                       //Provisie multiplier
  std::vector<double> dagb0;

  this->vulTabel(t, dagb0);

  for(size_t bitstring = 0; bitstring < dagb0.size(); bitstring++)
  {
    double tempMaxb0 = bedrag[t][bitstring] * (1 + renteMult);
    //Doorloop alle bitstrings waarvan het bedrag in kas >= 0, want anders is het onmogelijk om te kopen
    if(bedrag[t][bitstring] >= 0)
    {        
      for(int aandeel = 0; aandeel < this->n; aandeel++)
      {
        int aandeelBit = std::pow(2, aandeel);
        
        //Winst als men een aandeel koopt en de volgende dag verkoopt
        double koopWinst = (this->koers[t+1][aandeel] * (1 - pMult)) - (this->koers[t][aandeel] * pMult); 

        //Rente als men niets koopt
        double renteWinst = (this->koers[t][aandeel]) * (1 + renteMult);

        //Check of een aandeel kopen winstgevend is
        bool winstGevend = (renteWinst >= koopWinst) ? false : true;

        //Check of een aandeel in de bitstring zit
        if(bitstring & aandeelBit)
        {
          if(winstGevend)
          {
            double verkoopWaarde = this->koers[t+1][aandeel] * (1 - pMult); 
            tempMaxb0 += verkoopWaarde;
          }else{
            //Breek het programma vroegtijdig af als een niet winstgevend aandeel in de bitstring zit
            goto end;
          }
          
          //Stort provisie + misgelopen rente terug als het aandeel al in bezit was aan het begin van de dag
          //Dus verkoop en herkoop is onnodig
          if(this->a & aandeelBit)
          {
            double provisie =  this->koers[t][aandeel] * 2 * pMult * (1 + renteMult);
            tempMaxb0 += provisie;
          }
        }
      }
      
      //Check of een combinatie aandelen meer oplevert dan de vorige beste
      if(tempMaxb0 > maxb0 || (maxb0 == -1))
      {
        //Wijs de huidige beste combinatie aandelen en het bijbehorende bedrag toe 
        maxa = bitstring;
        maxb0 = tempMaxb0;
      }
    }
    end:;
  }
}

//****************************************************************************

void Beurs::vulTabel(int t, std::vector<double> &dagb0)
{
  int maxBezit = std::pow(2, this->n) - 1;                //Maximale bitwaarde voor de aandelen bitstring       
  double pMult = this->p / 100;                           //Provisie multiplier

  //Doorloop elke combinatie van aandelen in bitstring
  for(int bitstring = 0; bitstring <= maxBezit; bitstring++)
  {
    double tempb0 = this->b0;                             //Reset de  waarde van kas naar het begin van de dag

    for(int aandeel = 0; aandeel < this->n; aandeel++)
    {
      int aandeelBit = std::pow(2, aandeel);              //Bit waarde van aandeel

      //Check of aandeel in bitstring zit
      if(bitstring & aandeelBit)
      {
        tempb0 -= (1 + pMult) * this->koers[t][aandeel];  //Verwijder de waarde van aandeel + provisie uit kas
      }
    }
    
    dagb0.push_back(tempb0);                              //Doe de waarde van b0 in de vector voor elke combinatie bitstring         
  }
  
  this->bedrag.push_back(dagb0);                          //Doe elke vector dagb0 voor elke dag;
}

//****************************************************************************