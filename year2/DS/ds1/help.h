/*
Eerste opdracht datastructuren: stapels.
Auteurs: Yvo hu s2962802 en Seyed Saqlain Zeidi s2982048
File: help.h
*/

#ifndef help_h
#define help_h

//Convert number string to a number
template <class X>
X		atoi(std::string str){
	X c;
	X s;
	X res;

	c = 0;
	s = 1;
	res = 0;
	while ((str[c] >= '\t' && str[c] <= '\r') || str[c] == ' ')
		c++;
	while (str[c] == '+' || str[c] == '-'){
		if (str[c] == '-')
			s *= -1;
		c++;
	}
	while (str[c] >= '0' && str[c] <= '9'){
		res = (str[c] - '0') + (res * 10);
		c++;
	}
	return (res * s);
}

//Error message
void error(std::string err){
    std::cout << "Error:" << err << "\n";
    exit(1);
}


#endif