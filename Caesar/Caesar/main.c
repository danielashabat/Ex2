#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

int main(int argc, char* argv[]) {

	char expected_v = char_decryptor('y',3);
	assert(expected_v == 'v');

	char expected_T = char_decryptor('Y', 5);
	assert(expected_T == 'T');

	int is_one = is_char_digit('6');
	assert(is_one == 1);
	int is_zero = is_lower_case('6');
	assert(is_zero == 0);

	char expected_4 = char_decryptor('6', 2);
	assert(expected_4 == '4' && ("my expected is %c"));

	return 0;
}
