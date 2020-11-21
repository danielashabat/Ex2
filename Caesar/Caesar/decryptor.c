#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int is_capital_letter(char letter) {
	if (('A' <= letter) && (letter <= 'Z')) return 1;
	else return 0;
}
int is_lower_case(char letter) {
	if (('a' <= letter) && ( letter <= 'z')) return 1;
	else return 0;
}
int is_char_digit(char letter) {
	if (('0' <= letter) && (letter <= '9')) return 1;
	else return 0;
}

char char_decryptor(char letter, int key) {
	
	if (is_lower_case(letter)) {
		return ('a' + (letter - 'a' - key) % 26);
	}
	else if (is_capital_letter(letter)) {
		return ('A' + (letter - 'A' - key) % 26);
	}
	else if (is_char_digit(letter)) {
		return ('0' + (letter - '0' - key) % 10);
	}
	else {
		return letter;
	}
}

//this function decryptes the string with the given 'key' and change the string the the decrypted string.
void string_decryptor(char* str, int key) {
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = char_decryptor(str[i], key);
	}

}