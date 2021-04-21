// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "standaard.h"
#include "rooster.h"
#include "vak.h"

//*************************************************************************

// Default constructor
Rooster::Rooster ()
{
  // TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//*************************************************************************

bool Rooster::leesIn (const char* invoerNaam)
{
  std::ifstream invoer;                                                                                                                                      

  invoer.open(invoerNaam,std::ios::in);
  if(invoer.fail())
  {
    std::cout << "fail";
    return false;
  } 
  invoer >> nrDagen >> nrUrenPerDag >> nrZalen >> nrDocenten;

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
  std::cout << "Dagen:" << this->nrDagen << std::endl;
  std::cout << "Uren:" << this->nrUrenPerDag << std::endl;
  std::cout << "Zalen:" << this->nrZalen << std::endl;
  std::cout << "Docenten:" << this->nrDocenten << std::endl;
  std::cout << "Vakken:" << this->nrVakken << std::endl;
  std::cout << "================================================" << std::endl;
  for(int i = 0; i < this->nrDocenten; i++)
  {
    std::cout << std::endl;
    std::cout << "Docent:" <<  i << std::endl;
    std::cout << "Tijdsloten:" << std::endl;
    for(int j = 0; j < int(this->tijdSlotenDocenten[i].size()); j++)
    {
      std::cout << this->tijdSlotenDocenten[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "================================================" << std::endl;
  for(int i = 0; i < this->nrVakken; i++)
  {
    std::cout << std::endl;
    std::cout << "Vak:    " << this->vakken[i].krijgVakNaam() << std::endl;
    std::cout << "Docent: " << this->vakken[i].krijgNrDocent() << std::endl;
    std::cout << "Tracks: ";
    for(int j = 0; j < int(this->vakken[i].krijgTracks().size()); j++)
    {
      std::cout << this->vakken[i].krijgTracks()[j] << " ";
    }
    std::cout << std::endl;
  }
}  // drukAf ()

//*************************************************************************

bool Rooster::bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{

  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      rooster[s][z] = -1;
    }
  }
  /*
  
  for(int v = 0; v < this->vakken.size(); v++)
  {
    for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
    {
      for(int z = 0; z < this->nrZalen; z++)
      {
        rooster[s][z] = -1;
        std::cout << rooster[s][z] << "\n";
        if(true)
        {

        }
        else
        {

        }
      }
    }
  }*/
  

  return true;

}  // bepaalRooster

//*************************************************************************

bool Rooster::bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{

  return true;

}  // bepaalMinRooster
  
//*************************************************************************

void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  std::cout << "          ";
  for(int u = 0; u < this->nrUrenPerDag; u++)
  {
    std::cout << "|";
    for(int i = 1; i < this->nrZalen; i++)
    {
      std::cout << " ";
      if(i & 1)
        std::cout << " ";
    }
    std::cout << " " << u;
    for(int i = 1; i < this->nrZalen; i++)
    {
      std::cout << " ";
      if(!(i & 1))
        std::cout << " ";
    }
    std::cout << "|";
  }

  
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    if(s % this->nrUrenPerDag == 0)
    {
      std::cout << "\n";
      std::cout << this->weekDag((s/this->nrUrenPerDag));
    }

    std::cout << "|";
    for(int z = 0; z < this->nrZalen; z++)
    {
      if(rooster[s][z] != -1)
      {
        std::cout <<  " " << 0;
      }else{
        std::cout <<  -1;
      }
      if(z+1 < this->nrZalen)
        std::cout << "|";

    }
    std::cout << "|";
  }

}  // drukAfRooster

//*************************************************************************

void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  // TODO: implementeer deze functie

}  // bepaalRoosterGretig

std::string Rooster::weekDag(int nrDag)
{
  std::string dag;
  switch(nrDag)
  {
    case 0:
      dag = "Maandag   ";
      break;
    case 1:
      dag = "Dinsdag   ";
      break;
    case 2:
      dag = "Woensdag  ";
      break;
    case 3:
      dag = "Donderdag ";
      break;
    case 4:
      dag = "Vrijdag   ";
      break;
  }
  return dag;
}