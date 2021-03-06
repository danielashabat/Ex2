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
		char mod = (letter - 'a' - key + 26) % 26;
		if (mod < 0) mod = mod + 26;
		return ('a' + mod);
	}
	else if (is_capital_letter(letter)) {
		char mod = (letter - 'A' - key) % 26;
		if (mod < 0) mod = mod + 26;
		return ('A' + mod);
	}
	else if (is_char_digit(letter)) {
		char mod = (letter - '0' - key) % 10;
		if (mod < 0) mod = mod + 10;
		return ('0' + mod);
	}
	else {
		return letter;
	}
}

//this function decryptes the string with the given 'key' and change the string the the decrypted string.
void string_decryptor(char* str, int key, int num_to_count) {
	for (int i = 0; i<num_to_count; i++)
	{
		str[i] = char_decryptor(str[i], key);
	}

}

char char_encryptor(char letter, int key) {

	if (is_lower_case(letter)) {
		char mod = (letter - 'a' + key) % 26;
			if (mod < 0) mod = mod + 26;
		return ('a' + mod);
	}
	else if (is_capital_letter(letter)) {
		char mod = (letter - 'A' + key) % 26;
		if (mod < 0) mod = mod + 26;
		return ('A' + mod );
	}
	else if (is_char_digit(letter)) {
		char mod = (letter - '0' + key) % 10;
		if (mod < 0) mod = mod + 10;
		return ('0' + mod);
	}
	else {
		return letter;
	}
}



//this function decryptes the string with the given 'key' and change the string the the decrypted string.
void string_encryptor(char* str, int key,int num_to_count) {
	for (int i = 0; i<num_to_count; i++)
	{
		str[i] = char_encryptor(str[i], key);
	}

}
