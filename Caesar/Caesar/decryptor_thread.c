#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>

DWORD WINAPI decryptor_thread(LPVOID lpParam)
{
    PMYDATA pDataArray;


    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because 
    // it was checked for NULL before the thread was created.

    pDataArray = (PMYDATA)lpParam;

    // Print the parameter values using thread-safe functions.

}