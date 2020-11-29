
#ifndef	DECRYPTOR_H
#define DECRYPTOR_H

// Includes --------------------------------------------------------------------


// Function Declarations -------------------------------------------------------

//this function return 1  if 'letter' is capital letter, othereise return 0 
int is_capital_letter(char letter);

//this function return 1  if 'letter' is lower case letter, othereise return 0 
int is_lower_case(char letter);

//this function return 1  if 'letter' is digit letter, othereise return 0 
int is_char_digit(char letter);

//this function decrypt 'letter' with the given key and return the decoded char
char char_decryptor(char letter, int key);

//this function encrypt 'letter' with the given key and return the encrypted char
char char_encryptor(char letter, int key);

//this function decryptes the string with the given 'key' and change the string the the decrypted string.
void string_decryptor(char* str, //pointer to char array that holds the string you want to decrypt
	int key,//key for decryption
	int num_to_count); // length of the char array 'str'

//this function encrypts the string with the given 'key' and change the string the the encrypted string.
void string_encryptor(char* str, //pointer to char array that holds the string you want to encrypt
	int key,//key for encryption
	int num_to_count);//length of the char array 'str'
#endif
#pragma once
