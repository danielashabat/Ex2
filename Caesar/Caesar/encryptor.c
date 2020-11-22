#include <stdio.h>
#include <stdlib.h> 
#include "decryptor.h"

char char_encryptor(char letter, int key) {

	if (is_lower_case(letter)) {
		return ('a' + (letter - 'a' + key) % 26);
	}
	else if (is_capital_letter(letter)) {
		return ('A' + (letter - 'A' + key) % 26);
	}
	else if (is_char_digit(letter)) {
		return ('0' + (letter - '0' + key) % 10);
	}
	else {
		return letter;
	}
}

