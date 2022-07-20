// Implementatie van klasse Haven

#include <iostream>
#include <iomanip>
#include <climits>
#include <fstream>
#include "standaard.h"
#include "haven.h"
#include <chrono>
#include <thread>
using namespace std;

//*************************************************************************

Haven::Haven()
{
  this->geldig = false;
  this->table = nullptr;
} // default constructor

Haven::~Haven()
{
  deleteTable();
} // default constructor

//*************************************************************************

// Verwijder geheugen
void Haven::deleteTable()
{
  if (this->table != nullptr)
  {
    double ***kraan = this->table;
    double **from;
    double *to;

    for (int i = 0; i < this->K; i++)
    {
      from = kraan[i];
      for (int j = 0; j < this->N; j++)
      {
        to = from[j];
        delete[] to;
      }
      delete[] from;
    }
    delete[] kraan;
    this->table = nullptr;
  }
} // default constructor

//*************************************************************************

// Maak geheugen vrij
void Haven::initTable()
{
  double ***kraan = new double **[this->K];
  double **from;
  double *to;

  for (int i = 0; i < this->K; i++)
  {
    from = new double *[this->N];
    for (int j = 0; j < this->N; j++)
    {
      to = new double[this->N]{0};
      from[j] = to;
    }
    kraan[i] = from;
  }
  this->table = kraan;
}

bool Haven::leesInInstantie(const char *invoerNaam)
{
  deleteTable();
  bool checkFlag = true; // voldoet de invoer flag
  std::ifstream invoer;

  invoer.open(invoerNaam, std::ios::in);
  if (invoer.fail())
  {
    std::cout << "Bestand bestaat niet";
    return false;
  }

  invoer >> this->b >> this->N;

  for (int i = 0; i < N; i++)
  {
    int tmp;
    invoer >> tmp;

    if (!(1 <= tmp && tmp <= this->b))
    {
      checkFlag = false;
    }
    this->l.push_back(tmp);
  }

  invoer >> this->s;
  invoer >> this->c >> this->K;

  for (int i = 0; i < K; i++)
  {
    std::vector<double> rij;
    this->o.push_back(rij);
    for (int j = 0; j < this->N; j++)
    {
      double tmp;
      invoer >> tmp;

      if (!(tmp >= 0))
      {
        checkFlag = false;
      }
      this->o[i].push_back(tmp);
    }
  }

  // Check invoer volgens opdracht
  if (!(this->b >= 1) ||
      !(1 <= this->N && this->N <= 1024) ||
      !(this->s >= 0) ||
      !(this->c >= 0) ||
      !(1 <= this->K && this->K <= 512) ||
      !(checkFlag))
  {
    std::cout << "Invoer is ongeldig";
    return false;
  }

  initTable();
  this->geldig = true;
  return true;

} // leesInInstantie

//****************************************************************************

void Haven::genereerInstantie(int b, int N, int ogLengte, int bgLengte,
                              int s, double c, int K, double ogOpKosten, double bgOpKosten)
{
  deleteTable();
  this->b = b;
  this->N = N;

  // vul container lengtes
  for (int i = 0; i < this->N; i++)
  {
    int tmp = rand();
    tmp = (tmp % (ogLengte - bgLengte)) + ogLengte;
    this->l.push_back(tmp);
  }
  this->s = s;
  this->c = c;
  this->K = K;

  // vul operationele kosten
  for (int i = 0; i < this->K; i++)
  {
    std::vector<double> rij;
    this->o.push_back(rij);

    for (int j = 0; j < this->N; j++)
    {
      int tmp = rand();

      // modulus werkt niet met double, dus verander naar int en * 10 om significantie te behouden
      tmp = (tmp % (int(ogOpKosten * 10) - int(bgOpKosten * 10))) + ogOpKosten * 10;

      this->o[i].push_back(tmp / 10.0);
    }
  }

  initTable();
  this->geldig = true;
} // genereerInstantie

//*************************************************************************

void Haven::drukAfInstantie()
{
  if (this->geldig)
  {
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Breedte: " << this->b << std::endl;
    std::cout << "Aantal containers: " << this->N << std::endl;
    std::cout << "Tussenruimte containers: " << this->s << std::endl;
    std::cout << "Constante factor rijkosten: " << this->c << std::endl;
    std::cout << "Aantal kranen: " << this->K << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
  }
  else
    std::cout << "Er is geen instantie opgeslagen" << std::endl;
} // drukAfInstantie

//*************************************************************************

double Haven::bepaalMinKostenRec()
{
  if (this->geldig == false)
    return -1;

  this->mode = 0;
  std::vector<std::vector<int>> rijen;
  return totaalKosten(this->K, this->N, rijen, 0, 0, 0);
} // bepaalMinKostenRec

//*************************************************************************

double Haven::bepaalMinKostenTD()
{
  if (this->geldig == false)
    return -1;

  this->mode = 1;
  std::vector<std::vector<int>> rijen;
  double totaal = totaalKosten(this->K, this->N, rijen, 0, 0, 0);

  return totaal;
} // bepaalMinKostenTD

//*************************************************************************

std::vector<std::vector<std::vector<int>>> Haven::initRijenBU()
{
  std::vector<std::vector<std::vector<int>>> rijenCombinaties;

  // Initialisatie rijen
  for (int n = 0; n < this->N; n++)
  {
    std::vector<int> rij;
    std::vector<std::vector<int>> rijen;
    int itj;
    int j;

    // De startRijen worden gevuld met containers 0 tot en met i.
    // Iedere rij is bedoelt als startpunt voor de creatie van vervolgrijen
    for (int m = 0; m <= n; m++)
    {
      // Check of containers j tot en met i wel in een rij passen en geldig is
      if (getRijKosten(m, n) != -1)
        rij.push_back(m);
      else
        break;
    }
    // Stop
    if (rij.size() == 0)
      break;

    // Bereken alle mogelijke rijencombinaties van rijen met
    // daarin containers aan de hand een gegeven this->N, container grootte en rijbreedte
    itj = rij.back() + 1;
    rijen.push_back(rij);
    j = rijen.back().back() + 1;

    // Itereer niet over de laatste rijcombinatie als deze slechts 1 rij bevat met alle containers want deze is al compleet
    if (rijen.back().back() == this->N - 1)
    {
      rijenCombinaties.push_back(rijen);
      continue;
    }
    do
    {
      std::vector<int> rij;
      if (getRijKosten(itj, j) == -1)
      {
        itj = j + 1;
        // De laatste container is in een rij geplaatst, Sla deze combinatie op
        if (rijen.back().back() + 1 == this->N)
        {
          rijenCombinaties.push_back(rijen);
        }
        j = rijen.back().back() + 1;
        rijen.pop_back();
        itj = rijen.back().back() + 1;
      }
      else
      {
        while (itj <= j)
        {
          rij.push_back(itj);
          itj++;
        }
        j++;

        rijen.push_back(rij);
      }
      // Stop als de rijen 1 groot is (de beginrij) en containers it tot en met j niet aan een volgende rij toegevoegd kunnen worden
    } while (!(rijen.size() == 1 && getRijKosten(itj, j) == -1));
  }
  return rijenCombinaties;
}

std::vector<std::vector<int>> Haven::initKranenCombinatiesBU(int rijenSize, std::vector<std::pair<int, std::vector<std::vector<int>>>> kranenCombinatiesN)
{
  // Bereken alle mogelijke combinaties van kranen en rijen op basis van de grootte van rijen
  std::vector<std::vector<int>> kranenCombinaties;
  std::vector<int> kranen;

  // Vind een kranencombinatie voor een gegeven rijensize, return deze als gevonden
  for (size_t j = 0; j < kranenCombinatiesN.size(); j++)
  {
    if (kranenCombinatiesN[j].first == rijenSize)
    {
      kranenCombinaties = kranenCombinatiesN[j].second;
      return kranenCombinaties;
    }
  }

  // Een kranencombinatie is nog niet gevonden in de opgeslagen kranencombinaties, dus genereer er 1
  // Initialisatie kranen
  for (int m = 0; m < this->K; m++)
  {
    kranen.push_back(0);
  }
  kranen[0] = rijenSize;

  int itk = 0;
  int restant = 0;
  do
  {
    if (restant == 0)
    {
      kranenCombinaties.push_back(kranen);
      // Laatste iteratie
      if (kranen.back() == rijenSize)
        break;

      for (int m = this->K - 1; m >= 0; m--)
      {
        if (kranen[m] != 0)
        {
          kranen[m]--;
          restant = rijenSize;
          for (int n = 0; n <= m; n++)
          {
            restant -= kranen[n];
          }
          itk = m + 1;
          if (itk == this->K && restant != 0)
          {
            m++;
            continue;
          }
          break;
        }
      }
    }
    else
    {
      kranen[itk] = restant;
      restant = 0;
      itk++;
    }
    //Stop als de laatste kraancombinatie is geplaatst. Dus als K = 3 en N = 4 dan is de laatste combinatie 0,0,4
  }while(kranen.back() != rijenSize);

  return kranenCombinaties;
}

void Haven::berekenPlaatsingBU(std::vector<std::pair<int, int>> &plaatsing, std::vector<std::vector<int>> besteRijCombinatie, std::vector<int> besteKraanCombinatie)
{
  // Bereken de plaatsing op basis van de beste kranencombinatie en beste rijencombinatie
  int itk = 0;
  int k = -1;
  for (size_t i = 0; i < besteRijCombinatie.size(); i++)
  {
    if (itk == 0)
    {
      k++;
      while (besteKraanCombinatie[k] == 0)
      {
        k++;
      }
      itk = besteKraanCombinatie[k];
    }

    plaatsing.push_back(std::pair<int, int>(k, besteRijCombinatie[i].front()));
    itk--;
  }
}

double Haven::bepaalMinKostenBU(vector<pair<int, int>> &plaatsing)
{
  if (this->geldig == false)
    return -1;

  this->mode = 1;
  plaatsing.clear();

  // Alle mogelijke rijen combinaties voor een gegeven this->N
  std::vector<std::vector<std::vector<int>>> rijenCombinaties = initRijenBU();

  // Hou de beste combinatie bij
  std::vector<int> besteKraanCombinatie;
  std::vector<std::vector<int>> besteRijCombinatie;
  double besteScore = -1;

  // Vector met alle mogelijke kranencombinaties voor een gegeven rijensize
  // dus een rijen set met N rijen heeft een eigen kranenCombinatie en een andere rijenset met N rijen heeft dezelfde kranencombinatie
  std::vector<std::pair<int, std::vector<std::vector<int>>>> kranenCombinatiesN;

  // Itereer over alle rijencombinaties om uiteindelijk
  // De rijencombinatie en kranencombinatie te vinden met de minste totale kosten
  for (size_t i = 0; i < rijenCombinaties.size(); i++)
  {
    // Een enkele combinatie rijenSize aantal rijen met bijbehorende containers
    int rijenSize = rijenCombinaties[i].size();

    // Bereken alle mogelijke combinaties van kranen en rijen op basis van de grootte van rijen
    // En sla deze eventueel op als deze nog niet eerder zijn gevonden in een vector
    std::vector<std::vector<int>> kranenCombinaties = initKranenCombinatiesBU(rijenSize, kranenCombinatiesN);

    std::vector<int> kranen;
    for (size_t j = 0; j < kranenCombinaties.size(); j++)
    {
      size_t huidigeRij = 0;
      double kosten = 0;

      kranen = kranenCombinaties[j];

      // Loop door k aantal kranen oftewel this->K
      for (int k = 0; k < this->K; k++)
      {
        // Loop door het aantal aaneengesloten rijen die op volgorde van de kranen horen
        for (int m = 0; m < kranen[k] && (int)huidigeRij < rijenSize; m++)
        {

          kosten += getKosten(k, rijenCombinaties[i][m + huidigeRij].front(), rijenCombinaties[i][m + huidigeRij].back());
        }
        huidigeRij += kranen[k];
      }
      // Check of de nieuwe kosten minder zijn dan de oude
      if ((besteScore == -1) || (besteScore != -1 && (kosten < besteScore)))
      {
        besteScore = kosten;
        besteKraanCombinatie = kranenCombinaties[j];
        besteRijCombinatie = rijenCombinaties[i];
      }
    }
  }

  berekenPlaatsingBU(plaatsing, besteRijCombinatie, besteKraanCombinatie);

  return besteScore;
} // bepaalMinKostenBU

//*************************************************************************

void Haven::drukAfPlaatsing(vector<pair<int, int>> &plaatsing)
{

  if (this->geldig == false)
  {
    std::cout << "Er is geen instantie ingelezen" << std::endl;
    return;
  }

  std::cout << "\nDe plaatsing is:" << std::endl;
  for (size_t i = 0; i < plaatsing.size(); i++)
  {
    std::cout << "(" << plaatsing[i].first << ", " << plaatsing[i].second << ")";
    if (i < plaatsing.size() - 1)
    {
      std::cout << ", ";
    }
  }
  std::cout << std::endl;

} // drukAfPlaatsing

// Bereken de rijkosten voor een rij met containers i tot en met j
double Haven::getRijKosten(int i, int j)
{
  // Begin container is groter dan eind container, of begin of eind container is groter dan het aantal containers
  if (i > j || i >= this->N || j >= this->N)
    return -1;

  int restant = this->b;

  for (int n = i; n <= j; n++)
  {
    // Elke iteratie na de eerste voeg tussenruimte toe
    if (n != i)
      restant -= this->s;

    restant -= this->l[n];
  }

  // Het aantal containers in deze rij passen niet
  if (restant < 0)
    return -1;

  // Laatste rij heeft geen rijkosten, en de laatste container zit automatisch in de laatste rij.
  if (j == this->N - 1)
    return 0;

  return this->c * restant * restant;
}

double Haven::getKraanKosten(int k, int i, int j)
{
  double totaal = 0;

  for (int it = i; it <= j; it++)
  {
    totaal += this->o[k][it];
  }

  return totaal;
}

double Haven::getKosten(int k, int i, int j)
{
  double huidigeKosten;
  if (this->mode == 1 && this->table[k][i][j])
    huidigeKosten = this->table[k][i][j];
  else
  {
    double huidigeKraanKosten = getKraanKosten(k, i, j);
    double huidigeRijKosten = getRijKosten(i, j);

    huidigeKosten = huidigeKraanKosten + huidigeRijKosten;
    if (this->mode == 1)
      this->table[k][i][j] = huidigeKosten;
  }
  return huidigeKosten;
}
double Haven::totaalKosten(int k, int j, std::vector<std::vector<int>> &rijen, int itk, int itj, double oudeKosten)
{
  double kosten = 0;

  // Base case slaat deze over en returned 0
  // Iteratieve tak
  if (itj != j)
  {
    double nieuweKosten;
    double huidigeRijKosten;
    double huidigeKosten;
    std::vector<std::vector<int>> rijenTmp;
    std::vector<std::vector<int>> rijenTmp2;
    std::vector<std::vector<int>> rijen2 = rijen;

    // 1e optie: Plaats container standaard naar volgende rij
    rijen.push_back(std::vector<int>{itj});
    rijenTmp = rijen;

    // Bereken de kosten voor de rijen container combinatie,
    // Waar de kraan itereert van itk tot en met totaal aantal kranen. Kies uiteindelijk de beste kraan
    for (int i = itk; i < k; i++)
    {
      rijenTmp2 = rijenTmp;

      huidigeKosten = getKosten(i, rijenTmp.back().front(), rijenTmp.back().back());
      nieuweKosten = totaalKosten(k, j, rijenTmp2, i, itj + 1, huidigeKosten) + huidigeKosten;

      // Initialiseer de eerste iteratie, en check of nieuwe iteraties minder kosten dan de huidige
      if ((i == itk) || nieuweKosten < kosten)
      {
        kosten = nieuweKosten;
        rijen = rijenTmp2;
      }
    }

    // 2e optie: Plaats container in een al bezette rij als daar ruimte voor is
    // Idem
    if (itj != 0)
    {
      huidigeRijKosten = getRijKosten(rijen2.back().front(), itj);
      if (huidigeRijKosten != -1)
      {
        rijen2.back().push_back(itj);
        for (int i = itk; i < k; i++)
        {
          rijenTmp2 = rijen2;

          huidigeKosten = getKosten(i, rijen2.back().front(), rijen2.back().back());
          // Trek oude kosten hier van af want anders tel je deze dubbel op met + huidige kosten van de rij met een extra container
          nieuweKosten = totaalKosten(k, j, rijenTmp2, i, itj + 1, huidigeKosten) - oudeKosten + huidigeKosten;

          // Gebruik de 2e optie als deze minder kost.
          if (nieuweKosten < kosten)
          {
            rijen = rijenTmp2;
            kosten = nieuweKosten;
          }
        }
      }
    }
  }

  return kosten;
}
