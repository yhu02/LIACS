#include "help_functions.h"

bool isNL(char c) {
	return (c == '\r' || c == '\n');
}
bool isAlphaLower(char c) {
	return (c >= 'a' && c <= 'z');
}
bool isLpar(char c) {
   return (c == '(');
}
bool isRpar(char c) {
   return (c == ')');
}
bool isAsterisk(char c) {
	return (c == '*');
}
bool isBar(char c) {
	return (c == '|');
}
bool isNull(char c) {
	return (c == '\0');
}

bool isValid(char c) { 
	if (isNL(c) || isAlphaLower(c) || isLpar(c) || isRpar(c) || isAsterisk(c) ||
		isBar(c) || isNull(c)) {
		return true;
	} else {
		return false;
	}
}
