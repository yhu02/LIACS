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
    std::vector<int> tijdSloten;
    invoer >> beschikbareTijdSloten;
    for(int j = 0; j < beschikbareTijdSloten; j++)
    {
      int tijdSlot;
      invoer >> tijdSlot;
      tijdSloten.push_back(tijdSlot);
    }
    tijdSlotenDocenten.push_back(tijdSloten);
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
    std::cout << "Tijdsloten:" << this->tijdSlotenDocenten[i].size() << std::endl;
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
  //Initialisatie
  aantalDeelroosters = 0;
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      rooster[s][z] = -1;
    }
  }
  std::vector<Vak> vakken_temp = this->vakken;
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {

      if(bepaalRoosterRecursief(rooster, aantalDeelroosters, vakken_temp))
      {
        return true;
      }else{
        continue;
      };
    }
    std::cout << s;
  }
  std::cout << "false";

  return false;


}  // bepaalRooster

bool Rooster::bepaalRoosterRecursief(int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters, std::vector<Vak> &vakken_temp)
{
  aantalDeelroosters++;
  

  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      for(int v = 0; v < int(vakken_temp.size()); v++)
      {
        if(rooster[s][z] != -1)
        {
          continue;
        }else{
          Vak vak = vakken_temp[v];
          rooster[s][z] = vak.krijgNrVak();
          vakken_temp.erase(vakken_temp.begin() + v);
          if(!checkCondities(rooster, s, z))
          {
            rooster[s][z] = -1;
            vakken_temp.insert(vakken_temp.begin() + v, vak);


            continue;
          }else if(vakken_temp.size() > 0){
            if(bepaalRoosterRecursief(rooster, aantalDeelroosters, vakken_temp))
            {
              return true;
            }else{
              continue;
            }
          }else{
            return true;
          }
        }

      }
    }
  }
  
  return false;
}
//Controleer of er aan alle condities worden voldaan
bool Rooster::checkCondities(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z)
{
  if(conditie2(rooster, s) && conditie3(rooster, s, z))
  {
    return true;
  }
  return false;
}
//Voorwaarde 2 in de opdracht
bool Rooster::conditie2(int rooster[MaxNrTijdsloten][MaxNrZalen], int s)
{
  std::vector<int> alleTracks;

  for(int i = 0; i < this->nrZalen; i++)
  {
    int nrVak = rooster[s][i];
    std::vector<int> vakTracks = this->vakken[nrVak].krijgTracks();
    for(int i = 0; i < int(vakTracks.size()); i++)
    {

      for(int j = 0; j < int(alleTracks.size()); j++)
      {
        if(vakTracks[i] == alleTracks[j])
        {
          return false;
        }
      }
      alleTracks.push_back(vakTracks[i]);
    }
  }
  return true;
}

//Voorwaarde 3 in de opdracht
bool Rooster::conditie3(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z)
{
  int nrVak = rooster[s][z];
  int nrDocent = this->vakken[nrVak].krijgNrDocent();

  for(int i = 0; i < int(this->tijdSlotenDocenten[nrDocent].size()); i++)
  {
    if(this->tijdSlotenDocenten[nrDocent][i] == s)
    {
      return true;
    }
  }
  return false;
}
//*************************************************************************

bool Rooster::bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{

  return true;

}  // bepaalMinRooster
  
//*************************************************************************

void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  // Uren per dag
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
    // Dagen
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
        std::cout <<  " " << rooster[s][z];
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



