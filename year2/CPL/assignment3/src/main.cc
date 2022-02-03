/**
 * Description: main
 * @author D.V. Stokkermans, S.S. Zeidi, Y. Hu
 * @date 23-11-2021
 * @file main.cc
 **/

#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
#include "hulpfuncties.h"

int main(int argc, char *argv[]) {
	std::string line;
	if (argc != 2) {
		fprintf(stderr, "Not enough arguments\n");
		exit(1);
	}
	std::fstream input(argv[1]);
	if (!input) {
		error("Cannot open file");
	}
	while (std::getline(input, line)) {
		Parser *p = new Parser(line);
		delete p;
	}
	input.close();
	return 0;
}