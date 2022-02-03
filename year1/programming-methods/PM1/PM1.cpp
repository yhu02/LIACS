#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath> 

//Programma van Yvo Hu en Aydeniz
//Laatste update 17-9-2020
//Gebruike compiler GNU g++
bool abc();

int main(){
	//Sectie 1 variabelen
	int dag, maand, jaar;
	int flag = 0;
	int resultdag, resultmaand, resultjaar;
	int ongeldige_data[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int geboortejaar, geboortemaand, geboortedag, resultaatjaar, resultaatmaand;

	tm s;
	time_t t;
	time (&t);
	s = * localtime (&t);
	dag = s.tm_mday;
	maand = s.tm_mon + 1;
	jaar = s.tm_year + 1900;

	//Sectie 2 variabelen
	char i1;
	char i2;
	int antwoord2;
	int antwoord2_;

	//Sectie 3 variabelen
	srand(time(NULL)); //Initialisatie van de random seed
	const double epsilon = 0.1;
	int getal1 = rand() % 90 + 10;
	int getal2 = rand() % 90+ 10;
	int antwoord3 = getal1 * getal2;
	int tempx = 0;
	int tempy = 0;
	int tempresult;

	//Sectie 4 variabelen
	char keuze;

	//Infoblok
	//std::cout << "Gemaakt door Yvo Hu studentnummer: 2962802 en Aydeniz Yildiz studentnummer: 2990989" << std::endl;
	//std::cout << "Studiejaar: 2020" << std::endl;
	//std::cout << "Studierichting: Informatica " << std::endl;

	//Sectie 1
	std::cout << "Voer je geboortejaar, geboortemaand en geboortedag in" << std::endl;
	std::cin >> geboortejaar;
	if(geboortejaar < jaar - 101){
		std::cout << "Je bent ouder dan 100 jaar en wordt onmiddelijk geweigerd" << std::endl;
		return 0;
	}
	else if(geboortejaar > jaar - 10){
		std::cout << "Je bent jonger dan 10 jaar en wordt onmiddelijk geweigerd" << std::endl;
		return 0;
	}
	std::cin >> geboortemaand >> geboortedag;

	resultaatjaar = jaar - geboortejaar;
	resultaatmaand = maand - geboortemaand;

	if(geboortemaand < maand){
		if(geboortedag > dag){
			resultaatmaand = ((12 - (geboortemaand - maand +1)) % 12);
		}
		else;
	}
	else if(geboortemaand > maand){
			if(geboortedag > dag){
				resultaatmaand = ((12 - (geboortemaand - maand) - 1) % 12);
				resultaatjaar -= 1;
			}
			else{
				resultaatjaar -=1;
				resultaatmaand = ((12 - (geboortemaand - maand)) % 12);
			}
		}
	else if(geboortemaand == maand){
		if(geboortedag > dag){
			resultaatmaand = ((12 - (geboortemaand - maand) - 1) % 12);
			resultaatjaar -= 1;
		}
		else{
			resultaatmaand = ((12 - (geboortemaand - maand)) % 12);
		}
	}	
	//Check of de datum in de toekomst ligt
	if(geboortejaar >= jaar && geboortemaand >= maand && geboortedag > dag){
		flag = 1;
	}
	//Check of de datum bestaat
	if(geboortedag > ongeldige_data[geboortemaand-1]){
		flag = 1;
	}
	else if(geboortemaand > 12){
		flag = 1;
	}
	else if(geboortejaar % 4 == 0){
		if(geboortejaar % 100 == 0){
			if(geboortejaar % 400 == 0);
			else if(geboortemaand == 2 && geboortedag > 28){
				flag = 1;
			}
		}
	}
	//Check of de student ouder of jonger is dan toegestaan
	if(resultaatjaar < 10){
		flag = 2;
	}
	if(resultaatjaar > 100){
		flag = 3;
	}
	//Tekstberichten die aan de student worden weergegeven
	switch(flag){
		case 0:
			std::cout << "Je bent " << resultaatjaar << " jaar en " << resultaatmaand << " maanden oud" << std::endl;
			break;
		case 1:
			std::cout << "Je hebt een ongeldige datum ingevoerd" << std::endl;
			return 0;
			break;
		case 2:
			std::cout << "Je bent jonger dan 10 jaar en wordt onmiddelijk geweigerd" << std::endl;
			return 0;
			break;
		case 3:
			std::cout << "Je bent ouder dan 100 jaar en wordt onmiddelijk geweigerd" << std::endl;
			return 0;
			break;
	}
	if(geboortemaand == maand && geboortedag == dag){
		std::cout << "Gefeliciteerd je bent jarig!" << std::endl;
	}

	if(geboortemaand != maand && geboortedag == dag){
		std::cout << "Gefeliciteerd je bent maandig!" << std::endl;
	}
	//Sectie 2
	//Invoer gegevens voor de dag
	std::cout << "Op welke dag werd je geboren?" << std::endl;
	std::cin >> i1;
	if(i1 == 'd'){
		std::cin >> i2;
		switch(i2){
			case('i'):
				antwoord2 = 2;
				break;
			case('o'):
				antwoord2 = 4;
				break;
		}
	}
	else if(i1 == 'z'){
		std::cin >> i2;
		switch(i2){
			case('a'):
				antwoord2 = 6;
				break;
			case('o'):
				antwoord2 = 0;
				break;
		}
	}
	else{
		switch(i1){
			case('m'):
				antwoord2 = 1;
			break;
			case('w'):
				antwoord2 = 3;
			break;
			case('v'):
				antwoord2 = 5;
			break;
		}
	}
	//Array met waardes voor iedere maand als maat voor de weekdag
	int i[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };  
	//Verminder jaar met 1 als de maand minder is dan 3. (m < 3) geeft een returnwaarde van 1 of 0
	geboortejaar -= geboortemaand < 3;  
	antwoord2_ = (geboortejaar + (geboortejaar / 4) - (geboortejaar / 100) +  (geboortejaar / 400) + i[geboortemaand - 1] + geboortedag) % 7;

	if(antwoord2 == antwoord2_){
		std::cout << "Dit is goed" << std::endl;
	}
	else{
		std::cout << "Dit is fout. Het programma eindigt nu." << std::endl;
		return 0;
	}
	//Sectie 3	
	//Voer twee getallen in en check of de som van deze getallen overeenkomen met het product van de willekeurige getallen
	resultaatjaar >= 30 ? std::cout << "Geachte aankomende student " : std::cout << "Beste aankomende student ";

	//Controleer of het antwoord binnen de marges liggen van het werkelijke antwoord + epsilon
	if(abc()){	
		std::cout << "Gefeliciteerd! ";
		resultaatjaar >= 30 ? std::cout << "U ": std::cout << "Je ";
		std::cout << "bent toegelaten tot een beta-studie!" << std::endl;
	}//Sectie 4
	else {
		std::cout << "Het antwoord is niet goed" << std::endl;
		resultaatjaar >= 30 ? std::cout << "U ": std::cout << "Je ";
		std::cout << "komt niet in aanmerking voor een beta-studie, maar wellicht wel voor een alpha-studie na successvolle afronding van de volgende meerkeuzevraag" << std::endl;
		std::cout << "Wat is de hoofdstad van Australie?" << std::endl;
		"A: Sydney ", "B: Canberra ","C: Melbourne ","D: London ";
		std::cout << "A: Sydney B: Canberra C: Melbourne D: London" << std::endl;
		std::cin >> keuze;
		if(keuze == 'b' || keuze == 'B'){
			std::cout << "Gefeliciteerd! ";
			resultaatjaar >= 30 ? std::cout << "U ": std::cout << "Je ";
			std::cout << "bent toegelaten tot een alpha-studie!" << std::endl;
		}
		else{
			resultaatjaar >= 30 ? std::cout << "U ": std::cout << "Je ";
			std::cout << "bent helaas niet toegelaten tot een studie bij deze universiteit" << std::endl;
		}
	}
	return 0;
}


bool abc(){
    bool flag;
    int answer;
    int a = rand() % 20, b = rand() % 20, c = rand() % 20;
    double x1, x2, discriminant, realPart, imaginaryPart;
    
    discriminant = b*b - 4*a*c;
    std::cout << "De kwadratische formule is " << a << "x^2 + " << b << "x + " << c << std::endl;
    std::cout << "Hoeveel reeele wortels heeft deze functie?" << std::endl;
    std::cin >> answer;
    

    if (discriminant > 0 && answer == 2 
        || discriminant == 0 && answer == 1
        || discriminant < 0 && answer == 0) {
        std::cout << "Dit antwoord is goed" << std::endl;
        flag = true;
    } else {
        std::cout << "Dit antwoord is fout" << std::endl;
        flag = false;
    }
 
    if (discriminant > 0) {
        x1 = (-b + sqrt(discriminant)) / (2*a);
        x2 = (-b - sqrt(discriminant)) / (2*a);
        std::cout << "Wortels zijn reeel en verschillend" << std::endl;
        std::cout << "x1 = " << x1 << std::endl;
        std::cout << "x2 = " << x2 << std::endl;
    }
    
    else if (discriminant == 0) {
        std::cout << "Wortels zijn reeel en hetzelfde" << std::endl;
        x1 = -b/(2*a);
        std::cout << "x1 = x2 = " << x1 << std::endl;
    }

    else if (discriminant < 0) {
        realPart = -b/(2*a);
        imaginaryPart =sqrt(-discriminant)/(2*a);
        std::cout << "Wortels zijn complex en verschillend"  << std::endl;
        std::cout << "x1 = " << realPart << "+" << imaginaryPart << "i" << std::endl;
        std::cout << "x2 = " << realPart << "-" << imaginaryPart << "i" << std::endl;
    }
    return flag;
}