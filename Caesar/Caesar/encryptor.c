#include <stdio.h>
#include <stdlib.h> 
#include "decryptor.h"

char char_encryptor(char letter, int key) {

	if (is_lower_case(letter)) {
		return ('a' + (letter - 'a' + key +26) % 26);
	}
	else if (is_capital_letter(letter)) {
		return ('A' + (letter - 'A' + key +26) % 26);
	}
	else if (is_char_digit(letter)) {
		return ('0' + (letter - '0' + key +10) % 10);
	}
	else {
		return letter;
	}
}



//this function decryptes the string with the given 'key' and change the string the the decrypted string.
char* string_encryptor(char* str, int key) {
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = char_encryptor(str[i], key);
	}
	return str;

}

