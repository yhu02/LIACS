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
  if(
    hoogte < MaxDimensie && breedte < MaxDimensie &&// Bord dimensies zijn binnen de limieten
    nrStenen == int(stenen.size()) &&                     // Aantal stenen komt overeen met de beschrijving
    hoogte >= rij && breedte >= kolom &&// Positie van beginsteen is binnen het bord
    (nrStenen >= (beginStenen * 2) + 1))                  //Genoeg stenen voor iedere speler en een beginsteen
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

  

}  // drukAf


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
  bord[zet.getRij()][zet.getKolom()].first = zet.getI();
  bord[zet.getRij()][zet.getKolom()].second = zet.getR();
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
    cout << "(" << stenen[i][0] << "," << stenen[i][1] << ",";
    cout << stenen[i][2] << "," << stenen[i][3] << ")";
  }
  cout << "\n";

}

vector<Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  vector<Zet> zetten;
  Zet zett;
  int x[SteenZijden] = {0, 1, 0, -1}; // kolom
  int y[SteenZijden] = {-1, 0, 1, 0}; // rij
  int arr[] = {0,1,2,3};
  
  for(int i = 0; i < int(speler1Stenen.size()); i++)
  {
    for(int j = 0;j < hoogte; j++)
    {
      for(int k = 0;k < breedte; k++)
      {
        if(bord[j][k].first >= 0)
        {
          for(int l = 0;l < SteenZijden; l++)
          {
            if(j+y[l] < 0 || k+x[l] < 0||j+y[l] >= hoogte ||k+x[l] >= breedte){}
            else if(bord[j+y[l]][k+x[l]].first < 0)
            {
              for(int m = 0; m < SteenZijden; m++)
              {
                int count = 0;
                bool flag = true;
                for(int n = 0; n < SteenZijden; n++)
                {
                  if(j+y[l]+y[n] < 0 || k+x[l]+x[n] < 0 || j+y[l]+y[n] >= hoogte|| k+x[l]+x[n] >= breedte){
                  }
                  else if(bord[j+y[l]+y[n]][k+x[l]+x[n]].first >= 0)
                  {
                    if(schuif(stenen[bord[j+y[l]+y[n]][k+x[l]+x[n]].first],bord[j+y[l]+y[n]][k+x[l]+x[n]].second)[arr[l]] != schuif(speler1Stenen[i],m)[n])
                    {
                      flag = false;
                      //cout << schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[0] << "," << schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[1] << ","<< schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[2] << ","<< schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[3] <<endl;
                      
                    }else if(schuif(stenen[bord[j+y[l]+y[n]][k+x[l]+x[n]].first],bord[j+y[l]+y[n]][k+x[l]+x[n]].second)[arr[l]] == schuif(speler1Stenen[i],m)[n])
                    {
                      
                    }else{
                      flag = false;
                    }
                  }else if(bord[j+y[l]+y[n]][k+x[l]+x[n]].first < 0)
                  {
                  }else{
                    flag = false;
                  }
                }
                
                if(flag)
                {
                  zett.setWaardes(speler1Stenen[i][4],m,j+y[l],k+x[l]);
                  zetten.push_back(zett);
                }
              }
            }
          }
        }
      }
    }
  }
  return zetten;

}  // bepaalMogelijkeZetten 


/*

vector<Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  vector<Zet> zetten;
  Zet zett;
  int x[SteenZijden] = {0, 1, 0, -1}; // kolom
  int y[SteenZijden] = {-1, 0, 1, 0}; // rij

  for(int i = 0; i < int(speler1Stenen.size());i++)                //Hoeveelste steen
  {
    for(int j = 0;j < hoogte;j++)                      
    {
      for(int k = 0;k < breedte;k++)
      {
        for(int l = 0;l < SteenZijden;l++)
        {
          if(bord[j][k].first >=0 && j+y[l] >= 0 && k+x[l]>= 0 && j+y[l] < hoogte && k+x[l] < breedte && bord[j+y[l]][k+x[l]].first < 0)
          { 
            int total = 0;
            for(int n = 0;n < SteenZijden;n++)
            {
              bool flag = true;
              for(int m = 0;m < SteenZijden;m++)
              {
                if(j+y[l]+y[m] >= 0 && k+x[l]+x[m] >= 0 && j+y[l]+y[m] < hoogte && k+x[l]+x[m] < breedte)
                {
                  if(bord[j+y[l]+y[m]][k+x[l]+x[m]].first >= 0)
                  {
                    if(schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[l]!=schuif(speler1Stenen[i],n)[m])
                    {
                      cout << "zero";
                      flag = false;
                    }else if(schuif(stenen[bord[j+y[l]+y[m]][k+x[l]+x[m]].first],bord[j+y[l]+y[m]][k+x[l]+x[m]].second)[l]==schuif(speler1Stenen[i],n)[m])
                    {
                      //
                    }

                  }else if(bord[j+y[l]+y[m]][k+x[l]+x[m]].first < 0){
                    cout << "skip: " << j+y[l]+y[m] << "," << k+x[l]+x[m] << endl;
                  }
                }
              }
              if(flag)
              {
                speler1Stenen[i][5] = total; 
                //cout << "total" << total;
                zett.setWaardes(speler1Stenen[i][4],n,j+y[l],k+x[l]);
                zetten.push_back(zett);
              }
              total = 0;
            }      
          }
        }
      }
    }
  }

  return zetten;

}  // bepaalMogelijkeZetten 

*/