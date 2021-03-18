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
  int som;
  //int som = 0;
  invoer.open(invoernaam,ios::in);
  if(invoer.fail())
  {
    cout << "fail";
    usleep(10000);
    return 0;
  }

  //Lees het bestand in, en sla deze op bij de bijbehorende variabelen
  hoogte = leesGetal(letter, invoer);
  breedte = leesGetal(letter, invoer);
  nrStenen = leesGetal(letter, invoer);
  beginStenen = leesGetal(letter, invoer);
  rij = leesGetal(letter, invoer);
  kolom = leesGetal(letter, invoer);

  //Lees iedere steen in en sla deze op
  som = leesGetal(letter,invoer);
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

  if(hoogte > MaxDimensie || breedte > MaxDimensie)
    std::cout << "hoogte of breedte zijn buiten de toegestane limieten";
  else if(nrStenen != int(stenen.size()))
    std::cout << "Aantal stenen komt niet overeen met de beschrijving";
  else if(rij >= hoogte || kolom >= breedte)
    std::cout << "Positie van beginsteen is buiten het bord";
  else if(nrStenen < (beginStenen * 2) + 1)
    std::cout << "Het aantal stenen is niet genoeg om over het hele spel te verspreiden";
  else
  {
    //Initialiseer het bord
    for(int i = 0; i < hoogte; i++)
    {
      for(int j = 0; j < breedte; j++)
      {
        bord[i][j].first = -1;
      }
    }
    //Verspreid de stenen over het bord, de spelers, en de pot
    for(int i = 0; i < nrStenen; i++)
    {
      //Plaats eerste steen op bord
      if(i == 0)
      {
        bord[rij][kolom].first = 0;
        bord[rij][kolom].second = 0;
      } else if(i <= beginStenen*2)
      {
        //Bitwise AND om de stenen te sorteren op even en oneven indexes
        if(i & 1)
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

bool AapjeOmino::eindstand ()
{
  if((speler1Stenen.size() == 0 || speler2Stenen.size() == 0) ||
    (bepaalMogelijkeZetten().size() == 0 && potStenen.size() == 0)
  )
  {
    return true;
  }
  return false;

}  // eindstand

//*************************************************************************

void AapjeOmino::drukAf()
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
        cout << "-  " << schuif(stenen[bord[i][j].first],bord[i][j].second)[0] << "  -";//Noord
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
        cout << "-" << schuif(stenen[bord[i][j].first],bord[i][j].second)[3] << "   ";  //West 
        cout << schuif(stenen[bord[i][j].first],bord[i][j].second)[1] << "-";           //Oost
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
        cout << "-  " << schuif(stenen[bord[i][j].first],bord[i][j].second)[2] << "  -"; //Zuid
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
  //drukAfStenen((*huidigStenen));

  

}  // drukAf


//*************************************************************************

int AapjeOmino::haalSteenUitPot ()
{
// wisselSpeler();
cout << "one" << speler1Stenen.size() << ":" << speler2Stenen.size();
  if(int(potStenen.size()) >= 1)
  {
    cout << "two" << speler1Stenen.size() << ":" << speler2Stenen.size();
    (*huidigStenen).push_back(potStenen[0]);
    cout << "three" << speler1Stenen.size() << ":" << speler2Stenen.size();
    potStenen.erase(potStenen.begin());
    cout << "four" << speler1Stenen.size() << ":" << speler2Stenen.size() << endl;
    return (*huidigStenen).back()[4];
  }else
  {
    cout << "five";
  return -1;
  }

}  // haalSteenUitPot

//*************************************************************************

void AapjeOmino::wisselSpeler ()
{
  if(aanBeurt){                      //Wissel speler
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

// undoeZet
bool AapjeOmino::undoeZet (Zet zet)                               
{

  //(*huidigStenen).push_back(stenen[zet.getI()]);
  bord[zet.getRij()][zet.getKolom()].first = -1;
  wisselSpeler();
  //(*huidigStenen).push_back(stenen[zet.getI()]); 

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
         // cout << "first" << speler1Stenen.size() << ":" << speler2Stenen.size() << endl;

  int score = 0;
  int score2 = 0;
  int uitPotGehaald = -1;
  vector<Zet> nieuweZetten;

  if(eindstand())
  { 
    cout << "eindstand bereikt" << speler1Stenen.size()  << ":" << speler2Stenen.size();
    score = speler2Stenen.size() - speler1Stenen.size();
    if(aanBeurt){
      score = -score;
    }           
    aantalStanden = 0;
    return score;
  }else
  {
  
    nieuweZetten = bepaalMogelijkeZetten();
    if(zetten.size() == 0)
    {
      
    }
    if(zetten.size() == 0)
    {
    }
    else
    {
      for(int i = 0;  i < int(nieuweZetten.size()); i++)
      {
        
        doeZet(nieuweZetten[i]);

        cout << endl << "[" << oudeZetten.size() << "]";
        for(int j = 0; j < oudeZetten.size(); j ++)
        {
          cout << "<===>" << oudeZetten[j].getI();
        }
        cout << ",";
                cout << "index:" << i << endl;
        score2 = besteScore(oudeZetten.back(), aantalStanden);
        if(score2 > score || score2 < -score){
          score = score2;
        }
        undoeZet(oudeZetten.back());
        (*huidigStenen).push_back(stenen[oudeZetten.back().getI()]);

        oudeZetten.pop_back();

       // cout << "index2:" << i << ":" << nieuweZetten.size() << endl;
        
      }
      //cout << "between:" << oudeZetten.back().getI() << ":";
                  //wisselSpeler();
      //cout << speler1Stenen.size() << "," << speler2Stenen.size();
      //nieuweZetten = bepaalMogelijkeZetten();
      return score2;

    }

  }

  aantalStanden++;
  return score;

}  // besteScore
/*
int AapjeOmino::winnend(int stand)
{
  if(eindstand())
  {
    return false;
  }
  else
  {
    for(int i = 0; i < int(zetten.size());i++)
    {
      doeZet(zetten[i]);
      if(!winnend(eindstand()))
      {
        undoeZet(zetten[i]);
        return true;
      }
      return false;
    }
  }
}*/
//*************************************************************************

bool AapjeOmino::genereerRandomSpel (int hoogte0, int breedte0,
       int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
       int minGetal, int maxGetal)
{
  hoogte = hoogte0;
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

  //Initialiseer het bord
  for(int i = 0; i < hoogte; i++)
  {
    for(int j = 0; j < breedte; j++)
    {
      bord[i][j].first = -1;
    }
  }
  //Verspreid de stenen over het bord, de spelers, en de pot
  for(int i = 0; i < nrStenen; i++)
  {
    //Plaats eerste steen op bord
    if(i == 0)
    {
      bord[rij][kolom].first = 0;
      bord[rij][kolom].second = 0;
    } else if(i <= beginStenen*2)
    {
      //Bitwise AND om de stenen te sorteren op even en oneven indexes
      if(i & 1)
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
    letter = invoer.get();
  }
  while(isdigit(letter)){
      //Indien meer iteraties, vermenigvuldig met 10
      sum *= sum > 0 ? 10 : sum;
      sum += (letter - '0');
      letter = invoer.get();
  }
  return sum;
}


//herordert een vector met een verschuiving
vector<int> AapjeOmino::schuif(vector<int> vec, int shift)
{
  vector<int> vec2 = vec;
  for(int i = 0; i < SteenZijden; i++){
  vec[i] = vec2[(4-shift + i)%SteenZijden ]; // shift vector elementen met n = shift naar rechts tot het einde en weer naar het begin.
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


