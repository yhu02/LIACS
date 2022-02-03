// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "standaard.h"
#include "rooster.h"
#include "vak.h"
#include <unistd.h>

Rooster::Rooster()
{
  nrDagen      = 0; // Aantal dagen in het rooster
  nrUrenPerDag = 0; // Aantal uren per dag
  nrZalen      = 0; // Aantal beschikbare zalen
  nrDocenten   = 0; // Aantal docenten
  nrVakken     = 0; // Aantal vakken
  maxKarakters = 0; // Aantal karakters van het vak met de langste naam

  /* Zalensymmetrie aantal tijdsloten in het rooster
  std::vector<std::vector<int>> tijdSlot;
  for(int i = 0; i < MaxNrTijdsloten; i++)
  {
    roosterVakCombinaties.push_back(tijdSlot);
  }
  */
}

bool Rooster::leesIn (const char* invoerNaam)
{
  std::ifstream invoer;      
  
  invoer.open(invoerNaam,std::ios::in);
  if(invoer.fail())
  {
    std::cout << "Bestand bestaat niet";
    return false;
  } 
  invoer >> nrDagen >> nrUrenPerDag >> nrZalen >> nrDocenten;
  
  //Lees in docenten en de bijbehorende tijdsloten
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
    docenten.push_back(tijdSloten);
  }
  
  //Lees in vakken en bijbehorende data
  invoer >> nrVakken;
  for(int i = 0; i < nrVakken; i++)
  {
    std::string vakNaam;
    int nrDocent;
    int nrTracks;

    invoer >> vakNaam >> nrDocent >> nrTracks;

    Vak vak(vakNaam, nrDocent, nrTracks);
    for(int j = 0; j < nrTracks; j++)
    {
      int track;
      invoer >> track;
      vak.voegTrackToe(track);
    }
    vakken.push_back(vak);
  }

  //Doorloop alle vakken en bereken hoeveel tracks er bestaan
  for(int i = 0; i < int(this->vakken.size()); i++)
  {
    for(int j = 0; j < int(this->vakken[i].krijgTracks().size()); j++)
    {
      std::vector<int>trackVakken;                      //Element om de tracks datastructuur te populeren
      int nrTrack = this->vakken[i].krijgTracks()[j];   //Track van een vak
      
      while(int(this->tracks.size()-1) < nrTrack)       //Voeg tracks toe aan de tracks datastructuur als deze nog niet bestaan
      {
        this->tracks.push_back(trackVakken);
      }
      this->tracks[nrTrack].push_back(i);               //Voeg vak toe aan track
    }
  }

  //Bereken de lengte van het vak met de langste naam
  for(int i = 0; i < int(this->vakken.size()); i++)
  {
    if(int(this->vakken[i].krijgVakNaam().size()) > this->maxKarakters)
    {
      this->maxKarakters = this->vakken[i].krijgVakNaam().size();
    }
  }

  //Initialisatie van de uitzondering
  uitzondering1Functie();

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
    std::cout << "Tijdsloten:" << this->docenten[i].size() << std::endl;
    for(int j = 0; j < int(this->docenten[i].size()); j++)
    {
      std::cout << this->docenten[i][j] << " ";
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
  Vak::nrVakCount = 0;                          //Vaknummer voor elk vak object
  std::vector<Vak> vakken_temp = this->vakken;
  
  //leegRoosterVakCombinaties();                //Zalensymmetrie

  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      rooster[s][z] = -1;
    }
  }
  
  //Doorloop alle tijdsloten
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    //Doorloop alle zalen
    for(int z = 0; z < this->nrZalen; z++)
    {
      //Doorloop alle vakken
      for(int v = 0; v < int(vakken_temp.size()); v++)
      {
        Vak vak = vakken_temp[v];
        rooster[s][z] = vak.krijgNrVak();
        vakken_temp.erase(vakken_temp.begin() + v); 
        /*
        if(zalenSymmetrie(s, rooster))
        {
          //Reset
          vakken_temp.insert(vakken_temp.begin() + v, vak);
          rooster[s][z] = -1;
          continue;
        }else{
          //Voeg nog onbekende vakcombinatie toe aan tijdslot
          std::vector<int> vakCombinatie;
          for(int z = 0; z < this->nrZalen; z++)
          {
            vakCombinatie.push_back(rooster[s][z]); 
          }
          roosterVakCombinaties[s].push_back(vakCombinatie);
        }
        */
        //Check of er aan de condities wordt voldaan
        if(checkCondities(rooster, s, z, vakken_temp))
        {
          //Check of er nog overige vakken zijn
          if(vakken_temp.size() > 0){       
            //Check of er een oplossing is gevonden
            if(bepaalRoosterRecursief(rooster, aantalDeelroosters, vakken_temp))
            {
              return true;                      //Retourneer true als er een oplossing is gevonden
            }
          }
          else{
            return true;                        //Retourneer true als er een oplossing is gevonden
          }
        }
        //Reset als er geen  oplossing is gevonden
        rooster[s][z] = -1;
        vakken_temp.insert(vakken_temp.begin() + v, vak);
      }
    }
  }
  return false;
}   // bepaalRooster

//Hulpfunctie bepaalRooster
bool Rooster::bepaalRoosterRecursief(int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters, std::vector<Vak> &vakken_temp)
{
  aantalDeelroosters++;
  //Doorloop alle tijdsloten
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    //Doorloop alle zalen
    for(int z = 0; z < this->nrZalen; z++)
    {
      if(rooster[s][z] != -1)                   //Skip berekeningen als het tijdslot en zaal al bezet zijn
      {
        continue;
      }else{
        //Doorloop alle overige vakken
        for(int v = 0; v < int(vakken_temp.size()); v++)
        {
          Vak vak = vakken_temp[v];
          rooster[s][z] = vak.krijgNrVak();
          vakken_temp.erase(vakken_temp.begin() + v);
          /*
          if(zalenSymmetrie(s, rooster))
          {
            //Reset
            vakken_temp.insert(vakken_temp.begin() + v, vak);
            rooster[s][z] = -1;
            continue;
          }else{
            //Voeg nog onbekende vakcombinatie toe aan tijdslot
            std::vector<int> vakCombinatie;
            for(int z = 0; z < this->nrZalen; z++)
            {
              vakCombinatie.push_back(rooster[s][z]); 
            }
            roosterVakCombinaties[s].push_back(vakCombinatie);
          }
          */
          //Check of er aan de condities wordt voldaan
          if(checkCondities(rooster, s, z, vakken_temp))
          {
            //Check of er nog overige vakken zijn
            if(vakken_temp.size() > 0){       
              //Check of er een oplossing is gevonden
              if(bepaalRoosterRecursief(rooster, aantalDeelroosters, vakken_temp))
              {
                return true;                      //Retourneer true als er een oplossing is gevonden
              }
            }
            else{
              return true;                        //Retourneer true als er een oplossing is gevonden
            }
          }
          //Reset als er geen  oplossing is gevonden
          rooster[s][z] = -1;
          vakken_temp.insert(vakken_temp.begin() + v, vak);
        }
      }
    }
  }
  return false;
}

//*************************************************************************
bool Rooster::bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
                        long long &aantalDeelroosters)
{
  //Initialisatie
  int minRooster[MaxNrTijdsloten][MaxNrZalen];  //Kortste rooster
  int minTijd = MaxNrTijdsloten;                //Laatst bezette tijdslot van kortste rooster

  aantalDeelroosters = 0;
  Vak::nrVakCount = 0;                          //Vaknummer voor elk vak object
  std::vector<Vak> vakken_temp = this->vakken;

  //leegRoosterVakCombinaties();                //Zalensymmetrie

  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      rooster[s][z] = -1;
      minRooster[s][z] = -1;

    }
  }

  //Doorloop alle tijdsloten
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    //Doorloop alle zalen
    for(int z = 0; z < this->nrZalen; z++)
    {
      for(int v = 0; v < int(vakken_temp.size()); v++)
      {
        //Ga door als s kleiner is dan het huidige kortste rooster eindtijd of als minRooster nog niet bestaat
        if(s <= minTijd)             
        {  
          if(minTijd <= (nrVakken / nrZalen))
          {
            goto end;
          }
          //Reset rooster
          for(int s2 = 0; s2 < (this->nrUrenPerDag * this->nrDagen); s2++)
          {
            for(int z2 = 0; z2 < this->nrZalen; z2++)
            {
              rooster[s2][z2] = -1;
            }
          }
          Vak vak = vakken_temp[v];
          rooster[s][z] = vak.krijgNrVak();
          vakken_temp.erase(vakken_temp.begin() + v);
          /*
          if(zalenSymmetrie(s, rooster))
          {
            //Reset
            vakken_temp.insert(vakken_temp.begin() + v, vak);
            rooster[s][z] = -1;
            continue;
          }else{
            //Voeg nog onbekende vakcombinatie toe aan tijdslot
            std::vector<int> vakCombinatie;
            for(int z = 0; z < this->nrZalen; z++)
            {
              vakCombinatie.push_back(rooster[s][z]); 
            }
            roosterVakCombinaties[s].push_back(vakCombinatie);
          }
          */
          //Check of er aan de condities wordt voldaan
          if(checkCondities(rooster, s, z, vakken_temp))
          {
            //Check of er nog overige vakken zijn
            if(vakken_temp.size() > 0){ 
              bepaalMinRoosterRecursief(rooster, minRooster, minTijd, aantalDeelroosters, vakken_temp);
            }else{
              //Check of het gevonden rooster korter is dan het huidige kortste rooster
              bepaalMinRoosterFunctie(rooster, minRooster, minTijd);
            }
          }
          //Reset
          vakken_temp.insert(vakken_temp.begin() + v, vak);
          rooster[s][z] = -1;
        }else{                    //Stop berekeningen als het nieuwe rooster niet korter kan zijn dan de huidige kortste rooster
          goto end;
        }
      }
    }
  }

  end:;
  //Check of er een geldig rooster is gevonden
  if(minTijd < MaxNrTijdsloten)
  {
    for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
    {
      for(int z = 0; z < this->nrZalen; z++)
      {
        rooster[s][z] = minRooster[s][z];     //Kopieer minRooster naar rooster
      }
    }
    return true;
  }
  return false;
}  // bepaalMinRooster
  

//Hulpfunctie bepaalRooster
bool Rooster::bepaalMinRoosterRecursief(int rooster[MaxNrTijdsloten][MaxNrZalen],
                        int minRooster[MaxNrTijdsloten][MaxNrZalen], int &minTijd,
                        long long &aantalDeelroosters, std::vector<Vak> &vakken_temp)
{
  aantalDeelroosters++;
  //Doorloop alle tijdsloten
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    //Doorloop alle zalen
    for(int z = 0; z < this->nrZalen; z++)
    { 
      if(rooster[s][z] != -1)                               //Skip berekeningen als het tijdslot en zaal al bezet zijn
      {
        continue;
      }else  if(s <= minTijd){  
        if(minTijd <= (nrVakken / nrZalen))
        {
          return false;
        }
        //Doorloop alle overige vakken
        for(int v = 0; v < int(vakken_temp.size()); v++)
        {
          Vak vak = vakken_temp[v];
          rooster[s][z] = vak.krijgNrVak();
          vakken_temp.erase(vakken_temp.begin() + v);
          /*
          if(zalenSymmetrie(s, rooster))
          {
            //Reset
            vakken_temp.insert(vakken_temp.begin() + v, vak);
            rooster[s][z] = -1;
            continue;
          }else{
            //Voeg nog onbekende vakcombinatie toe aan tijdslot
            std::vector<int> vakCombinatie;
            for(int z = 0; z < this->nrZalen; z++)
            {
              vakCombinatie.push_back(rooster[s][z]); 
            }
            roosterVakCombinaties[s].push_back(vakCombinatie);
          }
          */
          //Check of er aan de condities wordt voldaan
          if(checkCondities(rooster, s, z, vakken_temp))
          {
            //Check of er nog overige vakken zijn
            if(vakken_temp.size() > 0){ 
              bepaalMinRoosterRecursief(rooster, minRooster, minTijd, aantalDeelroosters, vakken_temp);
            }else{
              //Check of het gevonden rooster korter is dan het huidige kortste rooster
              bepaalMinRoosterFunctie(rooster, minRooster, minTijd);
            }
          }
          //Reset
          vakken_temp.insert(vakken_temp.begin() + v, vak);
          rooster[s][z] = -1;
        }
      }else{//Stop berekeningen als het nieuwe rooster niet korter kan zijn dan de huidige kortste rooster
        return false;
      }
    }
  }
  return false;
}

//Hulpfunctie bepaalMinRooster
void Rooster::bepaalMinRoosterFunctie( int rooster[MaxNrTijdsloten][MaxNrZalen],
                              int minRooster[MaxNrTijdsloten][MaxNrZalen],
                              int &minTijd)
{
  int last = 0;   //Laatst bezette tijdslot
  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      if(rooster[s][z] != -1)
      {
        last = s;
      }
    }
  }

  //Check of het nieuwe rooster korter is dan de huidige.
  if(last < minTijd)
  {
    //Kopieer rooster naar minRooster
    for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
    {
      for(int z = 0; z < this->nrZalen; z++)
      {
        minRooster[s][z] = rooster[s][z];     
      }
    }
    minTijd = last;               //Nieuwe kortste tijd
  }
}
//*************************************************************************

//Bepaal het rooster op een gretige manier
void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  //Initialisatie
  Vak::nrVakCount = 0;                          //Vaknummer voor elk vak object
  std::vector<Vak> vakken_temp = this->vakken;

  for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    for(int z = 0; z < this->nrZalen; z++)
    {
      rooster[s][z] = -1;
    }
  }

  //Blijf doorgaan tot er geen vakken meer over zijn om te plaatsen
  while(int(vakken_temp.size()) > 0)
  {
    std::vector<int> roosterHoogsteScores(MaxNrTijdsloten, -1);
    std::vector<int> roosterNrVakHoogsteScore(MaxNrTijdsloten, -1);
    //Doorloop alle tijdsloten
    for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
    {
      //Doorloop alle zalen
      for(int z = 0; z < this->nrZalen; z++)
      { 
        std::vector<int> scores;        //Vector met scores voor ieder vak op tijdslot s
        int score;                      //Score om scores te populeren
        int hoogsteScore = -1;          //Hoogste score
        int besteVakIndex = -1;         //Index van het vak met de hoogste score in vakken_temp

        //Sla over als  tijdslot en zaal al bezet
        if(rooster[s][z] != -1)
        {
          continue;
        }else{
          //Bereken de score van alle vakken op tijdslot s
          for(int v = 0; v < int(vakken_temp.size()); v++)
          {
            Vak vak = vakken_temp[v];
            rooster[s][z] = vak.krijgNrVak();
            score = (checkCondities2(rooster, s, z, vakken_temp));
            scores.push_back(score);
          }
          //Reset
          rooster[s][z] = -1;
          
          //Bereken het vak met de beste score op tijdslot s
          for(int v = 0; v < int(vakken_temp.size()); v++)
          {
            if(scores[v] > hoogsteScore)
            {
              besteVakIndex = v;
              hoogsteScore = scores[v];
              roosterHoogsteScores[s] = hoogsteScore;
              roosterNrVakHoogsteScore[s] = vakken_temp[besteVakIndex].krijgNrVak();
            }
          }
        }
      }
    }

    int nrVak = -1;           //nrVak met de beste score
    int tijdSlot = -1;        //Tijdslot met de beste score
    int hoogsteScore = -1;    //Hoogste score

    //Bereken het tijdslot met de hoogste score en het bijbehorende vaknummer
    for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
    {
      if(roosterHoogsteScores[s] > hoogsteScore)
      {
        hoogsteScore = roosterHoogsteScores[s];
        nrVak = roosterNrVakHoogsteScore[s];
        tijdSlot = s;
      }
    }

    //Plaats in het tijdslot het beste vak 
    for(int z = 0; z < this->nrZalen; z++)
    {
      if(rooster[tijdSlot][z] == -1)
      {
        
        for(int v = 0; v < int(vakken_temp.size()); v++)
        {
          //Haal vak uit de vakkenlijst
          if(vakken_temp[v].krijgNrVak() == nrVak)
          {
            vakken_temp.erase(vakken_temp.begin() + v);
          }
        }
        rooster[tijdSlot][z] = nrVak;
        //Beeindig zodra het vak is geplaatst in de eerstmogelijke zaal
        break;
      }
    }
  }
}  // bepaalRoosterGretig

//Wijs een score toe aan de plaatsing van een vak
int Rooster::checkCondities2(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp)
{
  int score = 0;
  if( conditie2(rooster, s))
  {
    score += 3;
  }
  if(conditie3(rooster, s, z))
  {
    score += 3;
  }
  if(conditie4(rooster, s, z))
  {
    score += 2;
  }
  if(conditie5(rooster, s, z, vakken_temp))
  {
    score += 1;
  }
  if(conditie6(rooster, s, z))
  {
    score += 2;
  }

  return score;
}

//Controleer of er aan alle condities worden voldaan
bool Rooster::checkCondities(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp)
{
  if( conditie2(rooster, s) &&
      conditie3(rooster, s, z) &&
      conditie4(rooster, s, z) &&
      conditie5(rooster, s, z, vakken_temp) &&
      conditie6(rooster, s, z))
  {
    return true;
  }
  return false;
}

//Voorwaarde 2 in de opdracht
bool Rooster::conditie2(int rooster[MaxNrTijdsloten][MaxNrZalen], int s)
{
  std::vector<int> tracksInTijdslot; //Vector met alle tracks in het huidige tijdslot
  //Doorloop alle zalen
  for(int z = 0; z < this->nrZalen; z++)
  {
    int nrVak = rooster[s][z];
    if(nrVak != -1)
    {    
      std::vector<int> vakTracks = this->vakken[nrVak].krijgTracks();
      //Doorloop alle tracks van een vak
      for(int i = 0; i < int(vakTracks.size()); i++)
      {
        //Doorloop alle tracks in de vector
        for(int j = 0; j < int(tracksInTijdslot.size()); j++)
        {
          //Check of de nieuwe track al in het tijdslot zit
          if(vakTracks[i] == tracksInTijdslot[j])
          {
            return false;
          }
        }
        //Voeg track toe aan de vector
        tracksInTijdslot.push_back(vakTracks[i]);
      }
    }
  }
  return true;
}

//Voorwaarde 3 in de opdracht
bool Rooster::conditie3(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z)
{
  int nrVak = rooster[s][z];
  int nrDocent = this->vakken[nrVak].krijgNrDocent();

  //Doorloop alle beschikbare tijden van de docent
  for(int i = 0; i < int(this->docenten[nrDocent].size()); i++)
  {
    //Check of de docent beschikbaar is
    if(this->docenten[nrDocent][i] == s)
    {
      return true;
    }
  }
  return false;
}

//Voorwaarde 4 in de opdracht
bool Rooster::conditie4(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z)
{
  int nrVak = rooster[s][z];
  int nrDocent = this->vakken[nrVak].krijgNrDocent();

  //Doorloop de tijsloten op de dag van de verandering
  for(int s2 = (s/this->nrUrenPerDag) * this->nrUrenPerDag; s2 < ((s/this->nrUrenPerDag)+1) * this->nrUrenPerDag; s2++)
  {
    //Doorloop alle zalen
    for(int z2 = 0; z2 < this->nrZalen; z2++)
    {
      int nrVak2 = rooster[s2][z2];       //Vak op dezelfde dag
      //Check of een tijdslot bezet is
      if(nrVak2 != -1)
      {    
        int nrDocent2 = this->vakken[nrVak2].krijgNrDocent();        //Docentnummer van een vak op dezelfde dag   

        //Skip het recentst geplaatste vak
        if(s2 == s && z2 == z)
        {
          continue;
        }
        //Check of de nieuwe docent al in de vector zit
        if(nrDocent == nrDocent2)
        {
          return false;
        }

      }
    }
  }
  
  return true;
}


//Voorwaarde 5 in de opdracht 
bool Rooster::conditie5(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z, std::vector<Vak> vakken_temp)
{
  int nrVak = rooster[s][z];
  //Doorloop alle tracks van het vak
  for(int t = 0; t < int(this->vakken[nrVak].krijgTracks().size()); t++)
  {
    int nrTrack = this->vakken[nrVak].krijgTracks()[t];
    int s2 = 0; //Counter voor de tijdslot

    //Doorloop alle dagen
    for(int nrDag = 0; nrDag < this->nrDagen; nrDag++)
    {
      int nrTrackVakken = 0;          //Counter voor het aantal vakken van de track op dag nrDag
      //Doorloop alle uren
      for(int nrUur = 0; nrUur < this->nrUrenPerDag; nrUur++)
      {
        //Doorloop alle zalen
        for(int z2 = 0; z2 < this->nrZalen; z2++)
        {
          int nrVak = rooster[s2][z2];
          //Check of een tijdslot bezet is
          if(nrVak != -1)
          {   
            //Doorloop alle tracks van een vak
            for(int k = 0; k < int(this->vakken[nrVak].krijgTracks().size()); k++)
            {
              int nrTrack2 = this->vakken[nrVak].krijgTracks()[k];
              //Check of track in de vak gelijk is aan track nrTrack en increment nrTrackvakken
              if(nrTrack2 == nrTrack){
                nrTrackVakken++;
              }
            } 
          }
        }
        s2++;
      }
      //Check of de track geen uitzondering is
      if((this->tracks[nrTrack].size() != 1) && !(uitzondering1(nrTrack)) && !(uitzondering2()))
      {
        //Check of nrTrackvakken gelijk is aan 1
        if(!(nrTrackVakken == 0 || nrTrackVakken >= 2))
        {
          //Doorloop alle vakken van track nrTrack
          for(int i = 0; i < int(this->tracks[nrTrack].size()); i++)
          {
            //Check of er nog een vak over is in vakken_temp die behoort bij track nrTrack
            for(int v = 0; v < int(vakken_temp.size()); v++)
            {
              //Als deze bestaat ga door met de volgende iteratie, anders return false
              if(vakken_temp[v].krijgNrVak() == tracks[nrTrack][i])
              {
                goto end;
              }
            }
          }
          return false;
        }
      }
      end:;
    }
  }
  return true;
}

//Voorwaarde 6 in de opdracht 
bool Rooster::conditie6(int rooster[MaxNrTijdsloten][MaxNrZalen], int s, int z)
{
  //Doorloop alle tracks
  for(int i = 0; i < int(tracks.size()); i++)
  {
    int s2 = (s/this->nrUrenPerDag) * this->nrUrenPerDag;      //Counter voor tijdslot begint bij eerste uur van de dag van tijdslot s
  
    int first = -1;             //Eerste tijdslot waar een vak behorend bij track[i] is gevonden
    int last = -1;              //Laatste tijdslot waar een vak behorend bij track[i] is gevonden
    int tussenUren = 0;         //Aantal tussenuren van de track
    int teller = 0;             //Tussenwaarde om tussenUren te berekenen

    //Doorloop alle uren
    for(int nrUur = 0; nrUur < this->nrUrenPerDag; nrUur++)
    {
      //Doorloop alle vakken van track[i]
      for(int j = 0; j < int(this->tracks[i].size()); j++)
      {
        //Doorloop alle zalen
        for(int z = 0; z < this->nrZalen; z++)
        {
          int nrVak = rooster[s2][z];
          //Check of het vak in track[i] zit, anders ga door met de volgende iteratie
          if(nrVak == tracks[i][j])
          {
            //Initialiseer waardes
            if(first == -1)
            {
              first = s2;
              teller = -1;
            }
            //Assign last aan s en increment teller
            last = s2;
            goto end;
          }
        }
        continue;
        end:;
        teller++;
      }
    s2++;
    }
    //Bereken tussenUren
    tussenUren = (last - first) - teller; 
    if(tussenUren > 1){
      return false;
    }
  }
 
  return true;
}

//Drukt het rooster af
void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  std::cout << "Dag" << std::endl;
  std::cout << "[(Uur | Zaal) Vaknaam : Docentnummer]" << std::endl << std::endl;
  for(int dag = 0; dag < this->nrDagen; dag++)
  {
    std::cout << weekDag(dag) << std::endl;
    for(int uur = 0; uur < this->nrUrenPerDag; uur++)
    {
      for(int z = 0; z < this->nrZalen; z++)
      {
        int teller;
        int s = (dag*this->nrUrenPerDag) + uur;
        std::cout << "[(" << (s % this->nrUrenPerDag);
        if((s % this->nrUrenPerDag) < 10)
        {
          std::cout << " ";
        }
        std::cout << "|";
        if(z < 10)
        {
          std::cout << " ";
        } 
        std::cout << z << ") ";
        if(rooster[s][z] == -1)
        {
          teller = this->maxKarakters;
        }else{
          teller = (this->maxKarakters - vakken[rooster[s][z]].krijgVakNaam().size());
          std::cout << vakken[rooster[s][z]].krijgVakNaam();
  
        }

        for(int i = 0; i < teller + 1; i++)
        {
          std::cout << " ";
        }
        std::cout << ":";
        if(rooster[s][z] != -1)
        {
          if(vakken[rooster[s][z]].krijgNrDocent() < 10){
            std::cout << " ";
          }
          std::cout << vakken[rooster[s][z]].krijgNrDocent();

        }else{
          std::cout << "  ";
        }
        std::cout << "]" << std::endl;
      }
    }
    std::cout << std::endl;
  }

}  // drukAfRooster

//*************************************************************************

//Retourneert dag van de week
std::string Rooster::weekDag(int nrDag)
{
  std::string dag;
  switch(nrDag)
  {
    case 0:
      dag = "Maandag";
      break;
    case 1:
      dag = "Dinsdag";
      break;
    case 2:
      dag = "Woensdag";
      break;
    case 3:
      dag = "Donderdag";
      break;
    case 4:
      dag = "Vrijdag";
      break;
  }
  return dag;
}

//Uitzondering 1 van conditie 5
bool Rooster::uitzondering1(int t)
{
  for(int i = 0; i < int(this->uitzondering1Tracks.size()); i++)
  {
    if(uitzondering1Tracks[i] == t)
    {
      return true;
    }
  }
  return false;
}

//Uitzondering 2 van conditie 5 (Een dag heeft 1 of minder lesuren)
bool Rooster::uitzondering2()
{
  if(this->nrUrenPerDag <= 1)
  {
    return true;
  }
  return false;
}

//Uitzondering1 hulpfunctie (Een docent geeft meer dan de helft van de vakken van een track)
void Rooster::uitzondering1Functie()
{
  //Doorloop alle tracks en check of deze voldoen aan uitzondering 1 
  for(int i = 0; i < int(this->tracks.size()); i++)
  {
    for(int j = 0; j < int(this->tracks[i].size()); j++)
    {
      int teller = 0;
      int nrVak  = this->tracks[i][j];
      int nrDocent = this->vakken[nrVak].krijgNrDocent();

      for(int k = 0; k < int(this->tracks[i].size()); k++)
      {
        if(nrDocent == this->vakken[this->tracks[i][k]].krijgNrDocent())
        {
          teller++;
        }
      }
      //Check of 1 docent meer dan de helft van de vakken van een track geeft
      if(teller > (int(this->tracks[i].size()) / 2) && (int(this->tracks[i].size() != 1)))
      {
        //Voeg track toe aan de uitzonderingen
        this->uitzondering1Tracks.push_back(i);
        break;
      }
    }
  }
}

/*
//ZalenSymmetrie implementatie
bool Rooster::zalenSymmetrie(int s, int rooster[MaxNrTijdsloten][MaxNrZalen])
{
  //Doorloop alle vakkencombinaties op tijdslot s die al zijn gevonden in een eerder rooster
  for(int i = 0; i < int(roosterVakCombinaties[s].size()); i++)
  {
    std::vector<int> vakkenCombinatie_temp = roosterVakCombinaties[s][i];
    //Doorloop alle vakken van de vakkencombinatie
    for(int j = 0; j <  int(roosterVakCombinaties[s][i].size()); j++)
    {
      //Doorloop alle vakken van de vakkencombinatie die wordt vergeleken
      for(int z = 0; z < this->nrZalen; z++)
      {
        //Check of het vak aanwezig is in de vakkencombinatie roosterVakCombinaties[s][i]
        if(rooster[s][z] == vakkenCombinatie_temp.front())
        {
          vakkenCombinatie_temp.erase(vakkenCombinatie_temp.begin());
          goto end;                                     //Ga door naar het volgende vak op tijdslot s
        }
      }

      //Vakkencombinatie komt niet overeen met de huidige vakkencombinatie
      goto end2;                                        //Ga door naar de volgende vakkencombinatie 

      end:;
      if(int(vakkenCombinatie_temp.size()) == 0)
      {
        return true;
      }
    }
    end2:;
  }
  return false;                                         //Retourneer false als de huidige vakkencombinatie nog niet bestaat
}

void Rooster::leegRoosterVakCombinaties()
{
 for(int s = 0; s < (this->nrUrenPerDag * this->nrDagen); s++)
  {
    this->roosterVakCombinaties[s].clear();
  }
}
*/