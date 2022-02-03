/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: main.cpp

Run het programma op de volgende manier:
1. g++ main.cpp
2. ./a.out "input string"
*/

#include <iostream>
#include "maxstapel.h"

int main(int argc, char **argv){

	if(argc != 2)
		exit(1);
	std::string invoer = argv[1];

	std::cout << "\nArray Stack:          ";
	maxStapel<Array<int> >(std::string(invoer));

	std::cout << "\nLinked List Stack:    ";
	maxStapel<LinkedList<int> >(std::string(invoer));

	std::cout << "\nSTL Vector Stack:     ";
	maxStapel<StlVector<int> >(std::string(invoer));

	std::cout << "\nSTL Stack Stack:      ";
	maxStapel<StlStack<int> >(std::string(invoer));
	
	std::cout << "\nDone!" << std::endl;
	
	return 0;
}//main()