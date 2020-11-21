
#ifndef	DECRYPTOR_H
#define DECRYPTOR_H

// Includes --------------------------------------------------------------------


// Function Declarations -------------------------------------------------------


int is_capital_letter(char letter);
int is_lower_case(char letter);
int is_char_digit(char letter);
char char_decryptor(char letter, int key);

//this function decryptes the string with the given 'key' and change the string the the decrypted string.
void string_decryptor(char* str, int key);

#endif
#pragma once
