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
  huidigStand = 0;                 // Huidige stand van het spel
  aanBeurt = 1,                    // Speler die aan de beurt is in het begin
  huidigStenen = &speler1Stenen;   // Pointer naar de hand van de speler die aan de beurt is in het begin(speler 1)
  score = -99;                     // score initialiseren
}  // default constructor

//*************************************************************************

bool AapjeOmino::leesIn (const char* invoernaam)
{
  ifstream invoer;
  char letter;
  int som;

  invoer.open(invoernaam,ios::in);                                                      //Check of het bestand kan worden geopend
  if(invoer.fail())
  {
    cout << "fail";
    return 0;
  }

  hoogte = leesGetal(letter, invoer);                                                   //Lees het bestand in, en sla de gegevens op bij de bijbehorende variabelen
  breedte = leesGetal(letter, invoer);
  nrStenen = leesGetal(letter, invoer);
  beginStenen = leesGetal(letter, invoer);
  rij = leesGetal(letter, invoer);
  kolom = leesGetal(letter, invoer);

  som = leesGetal(letter,invoer);                                                       //Lees iedere steen in en sla deze op
  for(int i = 0; i < nrStenen; i++){
    vector<int> steen;
    for(int j = 0; j < SteenZijden; j++)
    {
      steen.push_back(som);
      som = leesGetal(letter,invoer);
    }
    steen.push_back(i);
    stenen.push_back(steen);
    if(invoer.eof())
    {
      break;
    }
  }
                                                                                        //Conditie checks
  if(hoogte > MaxDimensie || breedte > MaxDimensie)                                     //Bord dimensies zijn binnen de limieten
    std::cout << "hoogte of breedte zijn buiten de toegestane limieten";
  else if(nrStenen != int(stenen.size()))                                               //Het aantal stenen komt overeen met het werkelijk aantal stenen die beschreven zijn
    std::cout << "Aantal stenen komt niet overeen met de beschrijving";
  else if(rij >= hoogte || kolom >= breedte)                                            //Positie van de beginsteen is binnen het bord
    std::cout << "Positie van beginsteen is buiten het bord";
  else if(nrStenen < (beginStenen * 2) + 1)                                             //Er zijn genoeg stenen om te verdelen over het spel
    std::cout << "Het aantal stenen is niet genoeg om over het hele spel te verspreiden";
  else
  {
    for(int i = 0; i < hoogte; i++)                                                  //Initialiseer het bord
    {
      for(int j = 0; j < breedte; j++)
      {
        bord[i][j].first = -1;
      }
    }

    for(int i = 0; i < nrStenen; i++)                                               //Verspreid de stenen over het bord, de spelers, en de pot
    {
      if(i == 0)                                                                    //Plaats eerste steen op bord
      {
        bord[rij][kolom].first = 0;
        bord[rij][kolom].second = 0;
      } else if(i <= beginStenen*2)
      {
        if(i & 1)                                                                   //Bitwise AND om de stenen te verdelen over even en oneven indexes
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
  } 
  return false;
}  // leesIn

//*************************************************************************

bool AapjeOmino::eindstand ()                                                          //Check of er aan de eindcondities wordt voldaan
{
  if((speler1Stenen.size() == 0 || speler2Stenen.size() == 0) ||                       //1 van beide spelers heeft geen stenen meer of
    (bepaalMogelijkeZetten().size() == 0 && potStenen.size() == 0)                     //De pot is leeg, maar er kunnnen geen zetten meer gedaan worden 
  )
  {
    return true;
  }
  return false;

}  // eindstand

//*************************************************************************

void AapjeOmino::drukAf()                                                              //Print functie
{
  for(int j = 0; j < breedte; j++)
  {
    cout << "-------";
  }
  cout << "\n";
  for(int i = 0; i < hoogte; i++)
  {
    for(int j = 0; j < breedte; j++)
    {
      if(bord[i][j].first >= 0)
      {
        cout << "-  " << schuif(stenen[bord[i][j].first],bord[i][j].second)[0] << "  -";//Noord getal
      }else
      {
        cout << "-     -";
      }
    }
    cout << "\n";
    for(int j = 0; j < breedte; j++)
    {
      if(bord[i][j].first >= 0)
      {
        cout << "-" << schuif(stenen[bord[i][j].first],bord[i][j].second)[3] << "   ";  //West getal
        cout << schuif(stenen[bord[i][j].first],bord[i][j].second)[1] << "-";           //Oost getal
      }else
      {
        cout << "-     -";
      }
    }
    cout << " " << i << "\n";
    for(int j = 0; j < breedte; j++)
    {
      if(bord[i][j].first >= 0)
      {
        cout << "-  " << schuif(stenen[bord[i][j].first],bord[i][j].second)[2] << "  -"; //Zuid getal
      }else
      {
        cout << "-     -";
      }
    }
    cout << "\n";
   for(int j = 0; j < breedte; j++)
    {
      cout << "-------";
    }
    cout << "\n";
  }
  for(int j = 0; j < breedte; j++)
  {
    cout << "   " << j << "   ";
  }
  cout << "\n";
  cout << "Pot:";
  drukAfStenen(potStenen);
  cout << "Speler 1 stenen:";
  drukAfStenen(speler1Stenen);
  cout << "Speler 2 stenen:";
  drukAfStenen(speler2Stenen);
  cout << "Speler aan zet stenen:";
  drukAfStenen(*huidigStenen);

  

}  // drukAf


//*************************************************************************

int AapjeOmino::haalSteenUitPot ()
{
// wisselSpeler();
  if(int(potStenen.size()) >= 1)                                            //Haal steen uit pot als er nog stenen in de pot zitten
  {
    (*huidigStenen).push_back(potStenen[0]);
    potStenen.erase(potStenen.begin());
    return (*huidigStenen).back()[4];                                       //Return steennummer van steen uit de pot gehaald
  }else
  {
    return -1;                                                              //Anders return -1
  }

}  // haalSteenUitPot

//*************************************************************************

void AapjeOmino::wisselSpeler ()
{ 
  if(aanBeurt){                      //Wissel speler en verander der pointer van huidigStenen naar de hand van de andere speler
    huidigStenen = &speler2Stenen;
    aanBeurt = 0;
  }else{
    huidigStenen = &speler1Stenen;
    aanBeurt = 1;
  }                     
  

}  // wisselSpeler

//*************************************************************************

bool AapjeOmino::doeZet (Zet zet)                                //Implementatie kan geen ongeldige zetten uitvoeren<=========================
{
  huidigStand++;

  for(int i = 0; i < int(huidigStenen->size());i++)               //Herhaal voor iedere steen in hand
  {
    
    if((*huidigStenen)[i][4] == zet.getI())                       //Check of de steennummer van de zet overeenkomt met steennummer in hand
    {
      (*huidigStenen).erase((*huidigStenen).begin() + i);          //Verwijder steen uit hand van speler
    }
  }
  bord[zet.getRij()][zet.getKolom()].first = zet.getI();
  bord[zet.getRij()][zet.getKolom()].second = zet.getR();
  oudeZetten.push_back(zet);

  wisselSpeler();
  return true;
}  
                                                                  //Precies hetzelfde als doeZet maar met score reset voor als speler een handmatige zet doet
bool AapjeOmino::doeZet2 (Zet zet)                                //Helpt om zoveelste stand te berekenen
{ 
  score = -99;                                                    //Reset score   
  for(int i = 0; i < int(huidigStenen->size());i++)               
  {
    
    if((*huidigStenen)[i][4] == zet.getI())                       
    {
      (*huidigStenen).erase((*huidigStenen).begin() + i);         
    }
  }
  bord[zet.getRij()][zet.getKolom()].first = zet.getI();
  bord[zet.getRij()][zet.getKolom()].second = zet.getR();
  oudeZetten.push_back(zet);

  wisselSpeler();
  return true;
}  

// undoeZet
bool AapjeOmino::undoeZet (Zet zet)                                 //Undoe de zettendoor stenen van het bord te vegen
{

  bord[zet.getRij()][zet.getKolom()].first = -1;                    //Steennummer op bord[rij][kolom] word -1
  wisselSpeler();

  return true;

}  // undoeZet


//*************************************************************************

vector<Zet> AapjeOmino::bepaalGoedeZetten ()
{ 
  int buren = 0;
  vector<Zet> besteZetten;
  Zet nieuweZet;

  zetten = bepaalMogelijkeZetten();           //Neem alle mogelijke zetten
  for(int i = 0; i<int(zetten.size());i++)    //Neem de het hoogst aantal bezette buren van alle zetten
  {
    if(zetten[i].getSom() > buren)
    {
      buren = zetten[i].getSom();
    }
  }

  for(int i = 0; i<int(zetten.size());i++)    //Zet alle zetten waarvan de zetscore gelijk is aan de hoogste zetscore in de besteZetten vector
  {
    if(zetten[i].getSom() == buren)
    {
      nieuweZet.setWaardes(zetten[i].getI(),zetten[i].getR(),zetten[i].getRij(),zetten[i].getKolom(),zetten[i].getSom());
      besteZetten.push_back(nieuweZet);
    }
  }

  return besteZetten;                          //Retourneer de beste zetten

}  // bepaalGoedeZetten

//*************************************************************************

int AapjeOmino::besteScore (Zet &besteZet, long long &aantalStanden)
{
  besteZet.setDefaultWaardes();                                                 //Initialiseer besteZet als er geen zet mogelijk is
  int score2 = -99;
  int uitPotGehaald = -1;
  vector<Zet> zetten;

  if(eindstand())                                                               //Base case check of er wordt voldaan aan eindcondities
  { 
    score2 = speler2Stenen.size() - speler1Stenen.size();                       //Bereken score
    if(huidigStand & 1){
      score2 = -score2;  
    }
    aantalStanden = 0;                                                          //Basis waarde aantal standen        
                                                    
    return score2;
  }else
  {
    zetten = bepaalMogelijkeZetten();                                           //Laat alle mogelijke zetten zien
    if(zetten.size() == 0)                                                      //Check of er geen zetten mogelijk zijn
    {
      uitPotGehaald = haalSteenUitPot();                                        //Haal steen uit pot en laat alle mogelijke zetten weer zien
      zetten = bepaalMogelijkeZetten();                                       
    }
    if(zetten.size() == 0)                                                      //Als er nog steeds geen zetten mogelijk zijn, wissel beurt
    {
      wisselSpeler();
      score2 = besteScore(besteZet, aantalStanden);                                           
      wisselSpeler();                                                           
      if(score2 > score){                                                       //Vergelijk nieuwe score met oude score
        score = score2;             
        if(oudeZetten.size() > 0)                                               //Check of het programma al niet stopt voor de eerste zet
        besteZet = oudeZetten[huidigStand];                                           
      }                     
      potStenen.push_back(stenen[uitPotGehaald]);                               //Undoe de zetten door steen terug te zetten in de pot
      for(int j = 0; j < int((*huidigStenen).size()); j++)                      //Verwijder de gepakte steen als hij nog in de hand zit
      {
        if((*huidigStenen)[j][4] == uitPotGehaald)
        {
          (*huidigStenen).erase((*huidigStenen).begin() + j);
        }
      }

    }
    else
    {
      for(int i = 0;  i < int(zetten.size()); i++)
      {
        doeZet2(zetten[i]);                                                      //Doe Zet volgens de beschrijvingen in zetten[i]
        score2 = besteScore(besteZet, aantalStanden);

        if(score2 > score)                                                       //Vergelijk nieuwe score met oude score
        {
          score = score2;
          if(oudeZetten.size() > 0)                                              //Check of het programma al niet stopt voor de eerste zet
           besteZet = oudeZetten[huidigStand];
        }
        undoeZet(oudeZetten.back());                                             //Verwijder steen van bord
        (*huidigStenen).push_back(stenen[oudeZetten.back().getI()]);             //Zet verwijderde steen terug in hand
        oudeZetten.pop_back();                                                   //Verwijder laatse zet in oudeZetten
      }
      if(uitPotGehaald != -1)                                                    //Check of er steen gepakt is en gelijk neergezet
      {
        potStenen.push_back(stenen[uitPotGehaald]);                              //Undoe de zetten door steen terug te zetten in de pot
        for(int j = 0; j < int((*huidigStenen).size()); j++)                     //Verwijder de gepakte steen als hij nog in de hand zit  
        {
          if((*huidigStenen)[j][4] == uitPotGehaald)
          {
            (*huidigStenen).erase((*huidigStenen).begin() + j);
          }
        }
      }
    }
  }

  aantalStanden++;                                                                //verhoog Aantal standen met 1 na elke diepte
  return score;

}  // besteScore


//*************************************************************************

bool AapjeOmino::genereerRandomSpel (int hoogte0, int breedte0,
       int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
       int minGetal, int maxGetal)
{
  hoogte = hoogte0;                                                          //Initialiseer variabelen
  breedte = breedte0;
  nrStenen = nrStenen0;
  beginStenen = nrStenenInHand0;
  rij = rij0;
  kolom = kolom0;

  for(int i = 0; i < nrStenen0; i++)
  {
    vector<int> steen;
    for(int j = 0; j < SteenZijden; j++)
    {
      steen.push_back(randomGetal(minGetal, maxGetal));
    }
    steen.push_back(i);
    stenen.push_back(steen);
  }

  for(int i = 0; i < hoogte; i++)                                            //Initialiseer het bord
  {
    for(int j = 0; j < breedte; j++)
    {
      bord[i][j].first = -1;
    }
  }

  for(int i = 0; i < nrStenen; i++)                                          //Verspreid de stenen over het bord, de spelers, en de pot
  {
    if(i == 0)                                                               //Plaats eerste steen op bord
    {
      bord[rij][kolom].first = 0;
      bord[rij][kolom].second = 0;
    } else if(i <= beginStenen*2)
    {
      if(i & 1)                                                              //Bitwise AND om de stenen te verdelen over even en oneven indexes
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

}  // genereerRandomSpel

//*************************************************************************
int AapjeOmino::leesGetal(char& letter, ifstream& invoer)
{
  int sum = 0;
  letter = invoer.get();
  while(!isdigit(letter) && !invoer.eof())
  {
    letter = invoer.get();                                          //Pak getal uit invoer
  }
  while(isdigit(letter)){                                           //Indien meer getallen achter elkaar, vermenigvuldig met 10                                                                   
      sum *= sum > 0 ? 10 : sum;
      sum += (letter - '0');
      letter = invoer.get();
  }
  return sum;
}

vector<int> AapjeOmino::schuif(vector<int> vec, int shift)        //Herordert een vector met een verschuiving
{
  vector<int> vec2 = vec;
  for(int i = 0; i < SteenZijden; i++){
  vec[i] = vec2[(4-shift + i)%SteenZijden ];                      // shift vector elementen met n = shift naar rechts tot het einde en weer naar het begin.
  }
  return vec;
}

//Print alle stenen in een set
void AapjeOmino::drukAfStenen(vector<vector<int>> stenen)
{
  for(int i = 0; i < int(stenen.size()); i++)
  {
    cout << " [" << stenen[i][4] << "]"<< "(" << stenen[i][0] << "," << stenen[i][1] << ",";
    cout << stenen[i][2] << "," << stenen[i][3] << ") ";
  }
  cout << "\n";

}

vector<Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  zetten.clear();
  Zet nieuweZet;
  int x[SteenZijden] = {0, 1, 0, -1}; // Kolom
  int y[SteenZijden] = {-1, 0, 1, 0}; // Rij
  int arr[] = {2,3,0,1};              // Richting vergelijking op basis van de positie van de vergelijkende steen

  //wisselSpeler();

  for(int i = 0; i < int((*huidigStenen).size()); i++)    //Herhaal voor elke steen
  {
    for(int j = 0;j < hoogte; j++)
    {
      for(int k = 0;k < breedte; k++)
      {
        if(bord[j][k].first >= 0)                       //Check of er een steen bestaat op de hudige plek
        {
          for(int l = 0;l < SteenZijden; l++)           //Check alle buurvakken van de huidige steen
          {
            if(j+y[l] < 0 || k+x[l] < 0||j+y[l] >= hoogte ||k+x[l] >= breedte){}      //Check of de buurvak binnen het bord is
            else if(bord[j+y[l]][k+x[l]].first < 0)                                   //Check of de buurvak leeg is
            {
              for(int m = 0; m < SteenZijden; m++)                                    //Check alle rotaties van de buurvak
              {
                bool flag = true;
                int som = 0;
                for(int n = 0; n < SteenZijden; n++)                                  //Check alle buurvakken van de buurvak
                {
                  if(j+y[l]+y[n] < 0 || k+x[l]+x[n] < 0 || j+y[l]+y[n] >= hoogte|| k+x[l]+x[n] >= breedte){     //Check of de buurvakken van de buurvak binnen het bord is 
                  }
                  else if(bord[j+y[l]+y[n]][k+x[l]+x[n]].first >= 0)                  //Check of de buurvakken van de buurvak niet leeg is
                  {
                    if(schuif(stenen[bord[j+y[l]+y[n]][k+x[l]+x[n]].first],bord[j+y[l]+y[n]][k+x[l]+x[n]].second)[arr[n]] != schuif((*huidigStenen)[i],m)[n])   //Vergelijk alle buurvakken van de buurvak die niet leeg zijn
                    {
                      flag = false;
                    }else if(schuif(stenen[bord[j+y[l]+y[n]][k+x[l]+x[n]].first],bord[j+y[l]+y[n]][k+x[l]+x[n]].second)[arr[n]] == schuif((*huidigStenen)[i],m)[n]){ //+1 Zetscore voor elk buurvak van de buurvak die bestaan en passen met de hudige rotatie
                      som++;
                    }
                  }
                }
                if(flag)                     //Check of een zet geldig is
                {
                  bool flag2 = true;                                                     //Flag om herhalingen te detecteren
                  for(int p = 0;  p < int(zetten.size()); p++)                           //Detecteer herhalingen, en skip deze zetten
                  {
                     if(
                        zetten[p].getI() == (*huidigStenen)[i][4] && 
                        zetten[p].getR() == m &&
                        zetten[p].getRij() == j+y[l] &&
                        zetten[p].getKolom() == k+x[l])
                    {
                      flag2 = false;
                    }
                  }
                  if(flag2)                 //Check of er geen herhalende zetten zijn voorgekomen
                  {                         //Stop de zet in de zetten vector
                    nieuweZet.setWaardes((*huidigStenen)[i][4],m,j+y[l],k+x[l],som);
                    zetten.push_back(nieuweZet);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return zetten;                      //Retourneer alle mogelijke zetten

}  // bepaalMogelijkeZetten 


