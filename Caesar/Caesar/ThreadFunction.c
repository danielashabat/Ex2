#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <stdbool.h>
#include <assert.h>
#include <Windows.h>



#define MYNULL ((void*)0)
//void check_ReadFile_WriteFile_errors(BOOL bError, DWORD number_of_bytes_to_read_or_write, LPDWORD lpNumberOfBytesRead_or_Written, char* file_name);
DWORD set_file_size(HANDLE hfile, DWORD new_size);
void Thread_Function();
void read_and_write(LONG offset, DWORD number_of_bytes_to_read);
void check_file_handle(HANDLE h_file, char* file_name);
void check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written);

typedef struct ThreadData {
	char input_path[MAX_PATH];// the path of the input file
	char output_path[MAX_PATH];// the path of the output file
	int  startchar;//the statring line the thread need to decrypte/encrypte 
	int end_char;//the ending line the thread need to decrypte/encrypte (include this line)
	int key; // the key for the decryption/encryption
} ThreadData;


int main() {
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];

	HANDLE h_output_file;
	int dw_ret;
	strcpy_s(file_name_to_read, sizeof(file_name_to_read), "plaintext_example1.txt");
	strcpy_s(file_name_to_write, sizeof(file_name_to_write), "decrypt.txt");
	h_output_file = CreateFileA(file_name_to_write,// file name 
		GENERIC_WRITE,          // open for reading 
		FILE_SHARE_WRITE,      // open sharing for writing only 
		NULL,                  // default security 
		CREATE_ALWAYS,         // always creats a new file, if the file exists it overwrites it 
		FILE_ATTRIBUTE_NORMAL, // normal file 
		NULL);                 // no template 
	dw_ret = set_file_size(h_output_file, 89);
	CloseHandle(h_output_file);

}


void get_data_from_file_in_specipfic_lines(char* file_name_to_read, char* data_from_file, LONG offset, DWORD number_of_bytes_to_read) {

	HANDLE hfile_to_read;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesRead;
	int dw_pointer;

	//Create handle to output file 
	hfile_to_read = CreateFileA((LPCSTR)file_name_to_read, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_SHARE_READ, MYNULL);
	check_file_handle(hfile_to_read, file_name_to_read);

	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 

	dw_pointer = SetFilePointer(hfile_to_read, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = ReadFile(hfile_to_read, data_from_file, number_of_bytes_to_read, &lpNumberOfBytesRead, MYNULL);
	check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesRead);

	CloseHandle(hfile_to_read);


}

void write_to_specific_lines(char* file_name_to_write, char* data_to_file, LONG offset, DWORD number_of_bytes_to_read) {
	HANDLE hfile_to_write;
	BOOL bErrorFlag = FALSE;
	DWORD lpNumberOfBytesWritten;

	//Create HANDLE for input file 
	hfile_to_write = CreateFileA((LPCSTR)file_name_to_write, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_SHARE_WRITE, MYNULL);
	check_file_handle(hfile_to_write, file_name_to_write);

	// Set pointer to the place we want to start reading.
	//every thread will get the offset from the begining of the file. 
	SetFilePointer(hfile_to_write, offset, MYNULL, FILE_BEGIN);
	// read from the given number of file that given from the pointer 
	bErrorFlag = WriteFile(hfile_to_write, data_to_file, number_of_bytes_to_read, &lpNumberOfBytesWritten, MYNULL);
	check_ReadFile_WriteFile(bErrorFlag, number_of_bytes_to_read, lpNumberOfBytesWritten);
	//check_ReadFile_WriteFile_errors(bErrorFlag,number_of_bytes_to_read,lpNumberOfBytesWritten,file_name_to_write);
	CloseHandle(hfile_to_write);

}


DWORD WINAPI encrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;

	//Cast from LPVOID
	ThreadData* p_params;
	p_params = (ThreadData*)lpParam;

	//Get parameters
	strcpy_s(file_name_to_read, sizeof(file_name_to_read), p_params->input_path);
	strcpy_s(file_name_to_write, sizeof(file_name_to_write), p_params->output_path);
	start_char = p_params->startchar;
	end_char = p_params->end_char;
	key = p_params->key;
	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char;
	//Create string to ReadFile and WriteFile
	char* data_from_file;
	data_from_file = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	char* data_to_file;
	data_to_file = (char*)malloc(number_of_bytes_to_read * sizeof(char));


	//Read from file 
	get_data_from_file_in_specipfic_lines(file_name_to_read, data_from_file, (LONG)start_char, (DWORD)number_of_bytes_to_read);
	//decrypt data
	data_to_file = string_decryptor(data_from_file, key);
	//Write to file
	write_to_specific_lines(file_name_to_write, data_to_file, (LONG)start_char, (DWORD)number_of_bytes_to_read);

}

DWORD WINAPI decrypt_thread(LPVOID lpParam) {
	//Initalize
	char file_name_to_read[MAX_PATH];
	char file_name_to_write[MAX_PATH];
	int start_char;
	int end_char;
	int key;
	int number_of_bytes_to_read;

	//Cast from LPVOID
	ThreadData* p_params;
	p_params = (ThreadData*)lpParam;

	//Get parameters
	strcpy_s(file_name_to_read, sizeof(file_name_to_read), p_params->input_path);
	strcpy_s(file_name_to_write, sizeof(file_name_to_write), p_params->output_path);
	start_char = p_params->startchar;
	end_char = p_params->end_char;
	key = p_params->key;
	//calculate number of bytes to read and write
	number_of_bytes_to_read = end_char - start_char;
	//Create string to ReadFile and WriteFile
	char* data_from_file;
	data_from_file = (char*)malloc(number_of_bytes_to_read * sizeof(char));
	char* data_to_file;
	data_to_file = (char*)malloc(number_of_bytes_to_read * sizeof(char));


	//Read from file 
	get_data_from_file_in_specipfic_lines(file_name_to_read, data_from_file, start_char, number_of_bytes_to_read);
	//decrypt data
	data_to_file = string_decryptor(data_from_file, key);
	//Write to file
	write_to_specific_lines(file_name_to_write, data_to_file, start_char, number_of_bytes_to_read);


}
DWORD set_file_size(HANDLE hfile, DWORD new_size) {
	//  move output file pointer to the size of input file  
	DWORD dwPtr = SetFilePointer(hfile, new_size, NULL, FILE_BEGIN);

	if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
	{
		return INVALID_SET_FILE_POINTER;
	} // End of error handler 
	SetEndOfFile(hfile);//setting the output size equal to the input file size
	return 0;//the function successed

}

//Checking Functions ----------------------------------------------------------------------------

void check_file_handle(HANDLE h_file, char* file_name) {
	if (h_file == INVALID_HANDLE_VALUE)

		printf("Could not open %s file, error %ld\n", file_name, GetLastError());

	else

		printf("%s file HANDLE is OK!\n", file_name);

}

void check_ReadFile_WriteFile(BOOL bErrorFlag, DWORD number_of_bytes_to_read_or_write, DWORD lpNumberOfBytesRead_or_Written) {

	if (FALSE == bErrorFlag)
	{
		printf("Terminal failure: Unable to write to file.\n");
	}
	else
	{
		if (number_of_bytes_to_read_or_write != lpNumberOfBytesRead_or_Written)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			printf("Read or Write successfully.\n");
		}
	}



}