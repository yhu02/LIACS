// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include <vector>
#include "standaard.h"
#include "rooster.h"
#include "vak.h"

using namespace std;

//*************************************************************************

// Default constructor
Rooster::Rooster ()
{
  // TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//*************************************************************************

bool Rooster::leesIn (const char* invoerNaam)
{
  ifstream invoer;                                                                                                                                      
  int som;                              

  invoer.open(invoerNaam,ios::in);
  if(invoer.fail())
  {
    cout << "fail";
    return false;
  } 
  invoer >> nrDagen >> nrUrenPerDag >> nrZalen >> nrDocenten;

  std::cout << nrDagen << nrUrenPerDag << nrZalen << nrDocenten;

  for(int i = 0; i < nrDocenten; i++)
  {
    int beschikbareTijdSloten;
    std::vector<int>* tijdSloten = new std::vector<int>;
    invoer >> beschikbareTijdSloten;
    for(int j = 0; j < beschikbareTijdSloten; j++)
    {
      int tijdSlot;
      invoer >> tijdSlot;
      tijdSloten->push_back(tijdSlot);
    }
    tijdSlotenDocenten.push_back(*tijdSloten);
    delete tijdSloten;
  }

  invoer >> nrVakken;
  std::cout << nrVakken;
  for(int i = 0; i < nrVakken; i++)
  {
    std::string vakNaam;
    int nrDocent;
    int nrTracks;

    invoer >> vakNaam >> nrDocent >> nrTracks;

    Vak* vak = new Vak(vakNaam, nrDocent, nrTracks);
    for(int j = 0; j < nrTracks; j++)
    {
      int track;
      invoer >> track;
      vak->voegTrackToe(track);
    }
    vakken.push_back(*vak);
    delete vak;
  }


  return true;

}  // leesIn

//*************************************************************************

void Rooster::drukAf ()
{
  // TODO: implementeer deze memberfunctie

}  // drukAf ()

//*************************************************************************

bool Rooster::bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{
  // TODO: implementeer deze memberfunctie

  return true;

}  // bepaalRooster

//*************************************************************************

bool Rooster::bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{
  // TODO: implementeer deze memberfunctie

  return true;

}  // bepaalMinRooster
  
//*************************************************************************

void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  // TODO: implementeer deze memberfunctie

}  // drukAfRooster

//*************************************************************************

void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  // TODO: implementeer deze functie

}  // bepaalRoosterGretig
