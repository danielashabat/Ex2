#ifndef	ENCRYPTOR_H
#define ENCRYPTOR_H

// Includes --------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> 
#include "decryptor.h"

// Function Declarations -------------------------------------------------------
//this function enryptes the string with the given 'key' and change the string the the decrypted string.
char char_encryptor(char letter, int key);
char* string_encryptor(char* str, int key);

#endif
#pragma once


#pragma once
