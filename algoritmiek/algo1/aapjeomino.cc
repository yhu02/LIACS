// Implementatie van klasse AapjeOmino

#include <iostream>
#include <fstream>
#include <vector>
#include "standaard.h"
#include "aapjeomino.h"
#include <unistd.h>


//*************************************************************************

AapjeOmino::AapjeOmino ()
{
  // TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//*************************************************************************

bool AapjeOmino::leesIn (const char* invoernaam)
{
  ifstream invoer;
  char letter;
  //int som = 0;
  invoer.open(invoernaam,ios::in);
  if(invoer.fail())
  {
    cout << "fail";
    usleep(10000);
    return 0;
  }

  //Lees het bestand in, en sla deze op bij de bijbehorende variabelen
  this->hoogte = leesGetal(letter, invoer);
  this->breedte = leesGetal(letter, invoer);
  this->nrStenen = leesGetal(letter, invoer);
  this->beginStenen = leesGetal(letter, invoer);
  this->rij = leesGetal(letter, invoer);
  this->kolom = leesGetal(letter, invoer);

  //Lees iedere steen in en sla deze op
  int som = leesGetal(letter,invoer);
  for(int i = 0; i < this->nrStenen; i++){
    vector<int> steen;
    for(int j = 0; j < SteenZijden; j++)
    {
      steen.push_back(som);
      som = leesGetal(letter,invoer);
    }
    if(invoer.eof())
    {
      break;
    }
    stenen.push_back(steen);
  }
  
  //Print de opgeslagen gegevens
  /*
  cout << ":" << this->hoogte << ":" << this->breedte << "\n";
  cout << ":" << this->nrStenen << ":" << this->beginStenen << "\n";
  cout << ":" << this->rij << ":" << this->kolom << "\n";

  for(long unsigned int i = 0; i < stenen.size(); i++){
    for(int j = 0; j < SteenZijden; j++){
        cout << stenen[i][j] << " ";
    }
    cout << "\n";
  }
  usleep(10000);
  */
  //Conditie checks
  if(
    this->hoogte < MaxDimensie && this->breedte < MaxDimensie &&// Bord dimensies zijn binnen de limieten
    this->nrStenen == int(stenen.size()) &&                     // Aantal stenen komt overeen met de beschrijving
    this->hoogte >= this->rij && this->breedte >= this->kolom &&// Positie van beginsteen is binnen het bord
    (this->nrStenen >= (beginStenen * 2) + 1))                  //Genoeg stenen voor iedere speler en een beginsteen
  {
    //Verspreid de stenen over het bord, de spelers, en de pot
    for(int i = 0; i < nrStenen; i++)
    {
      //Plaats eerste steen op bord
      if(i == 0)
      {
        bord[this->rij][this->kolom].first = 0;
        bord[this->rij][this->kolom].second = 0;
      } else if(i < beginStenen*2)
      {
        //Bitwise AND om de stenen te sorteren op even en oneven indexes
        if(i & 0)
        {
          speler1Stenen.push_back(stenen[i]);
        } else 
        {
          speler2Stenen.push_back(stenen[i]);
        }
      }else
      {
        potStenen.push_back(stenen[i]);
      }
    }
    return true;
  } else
  {
    return false;
  }
}  // leesIn

//*************************************************************************

bool AapjeOmino::eindstand ()
{
  //if(speler1Stenen.size() == 0 || speler2Stenen.size == 0())

  return false;
  return true;

}  // eindstand

//*************************************************************************

void AapjeOmino::drukAf()
{
  for(int i = 0; i < this->hoogte; i++)
  {
    for(int j = 0; j < this->breedte; j++)
  }

}  // drukAf

//*************************************************************************

vector<Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  vector<Zet> zetten;
  for(int i = 0; i < this->hoogte; i++)
  {
    for(int j = 0; j < this->breedte; j++)
    {
      for(int k = 0; k < int(speler1Stenen.size()); k++)
      {
        for(int l = 0; l < SteenZijden; l++)
        {
          //Vergelijk 
          Zet zett;
          if(!(i == 0) && !(speler1Stenen[k][l] == schuif(stenen[bord[i-1][j].first],bord[i-1][j].second)[0]));                       // noord
          else if(!(j == this->breedte - 1) && !(speler1Stenen[k][l] == schuif(stenen[bord[i][j+1].first],bord[i][j+1].second)[1]));  // oost
          else if(!(i == this->hoogte - 1) && !(speler1Stenen[k][l] == schuif(stenen[bord[i+1][j].first],bord[i+1][j].second)[2]));   // zuid
          else if(!(j == 0) && !(speler1Stenen[k][l] == schuif(stenen[bord[i][j-1].first],bord[i][j-1].second)[3]));                  // west
          else
          {
            zett.setWaardes(k,l,i,j);
            zetten.push_back(zett);
          }
        }
      }
    }
  }
  return zetten;

}  // bepaalMogelijkeZetten

//*************************************************************************

int AapjeOmino::haalSteenUitPot ()
{
  // TODO: implementeer deze memberfunctie

  return 0;

}  // haalSteenUitPot

//*************************************************************************

void AapjeOmino::wisselSpeler ()
{
  // TODO: implementeer deze memberfunctie

}  // wisselSpeler

//*************************************************************************

bool AapjeOmino::doeZet (Zet zet)
{
  // TODO: implementeer deze memberfunctie

  return true;

}  // doeZet

//*************************************************************************

vector<Zet> AapjeOmino::bepaalGoedeZetten ()
{ vector<Zet> zetten;

  // TODO: implementeer deze memberfunctie

  return zetten;

}  // bepaalGoedeZetten

//*************************************************************************

int AapjeOmino::besteScore (Zet &besteZet, long long &aantalStanden)
{
  // TODO: implementeer deze memberfunctie

  return 0;

}  // besteScore

//*************************************************************************

bool AapjeOmino::genereerRandomSpel (int hoogte0, int breedte0,
       int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
       int minGetal, int maxGetal)
{
  // TODO: implementeer deze memberfunctie

  return true;

}  // genereerRandomSpel

//*************************************************************************
int AapjeOmino::leesGetal(char& letter, ifstream& invoer){
    int sum = 0;
    letter = invoer.get();
    while(isdigit(letter)){
        if(letter == '\n'){
            break;
        }
        //Indien meer iteraties, vermenigvuldig met 10
        sum *= sum > 0 ? 10 : sum;
        sum += (letter - '0');
        letter = invoer.get();
    }
    return sum;
}

vector<int> AapjeOmino::schuif(vector<int> vec, int schuif)
{
  vector<int> vec2 = vec;
  for(int i = 0; i < SteenZijden; i++){
    vec[i] = vec2[(schuif + i) % SteenZijden];
  }
  return vec;
}