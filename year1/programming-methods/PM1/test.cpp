#include <iostream>
#include <cmath>

bool abc();
int main() {
    abc();
}

bool abc(){
    bool flag;
    int answer;
    //int a = rand() % 20, b = rand() % 20, c = rand() % 20;
    int a = -4, b = 12, c = -9;
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