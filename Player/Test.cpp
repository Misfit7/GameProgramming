#include <bits\stdc++.h>

#ifdef MYCODE
printf("Code only to compile/run for me \n");
#endif
#ifdef MYFASTMODE
#error("Error : This code won't work in FastMode" )
#endif

#pragma message( "This is a reminder...hello..." )

#define HALT __asm { int 3 }
#define DBG_ASSERT(exp) {if ( !(exp) ) {DBG_HALT;}}
#define INVALID_FLOAT 2139095040
#define DBG_CHECKFLOAT(f) \
 {if (((*(unsigned int *)((void *)&f)) & INVALID_FLOAT) == INVALID_FLOAT) {DBG_HALT;}}
#define _CRTDBG_MAP_ALLOC

using namespace std;

//Saving debug information to a log file
__forceinline void dprintf(const char* fmt, ...)
{
    va_list parms;
    static char buf[2048]; // Dangerous - buffer overrun issues?
    // Try to print in the allocated space.
    va_start(parms, fmt);
    vsprintf_s(buf, fmt, parms);
    va_end(parms);
    // Dirty quick write the information out to a txt file
#if 0
    FILE* fp = fopen("output.txt", "a+");
    fprintf(fp, "%s", buf);
    fclose(fp);
#endif
    // Output to the visual studio window
    OutputDebugStringA(buf);
    system("pause");
}// End dprintf(..)

//int main() {
//	// Enable memory leak detection
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
//	// Any point in your code you can dump all the memory allocations
//	_CrtDumpMemoryLeaks(); // dump memory leaks to the output window
//	_crtBreakAlloc = 76; // Break at the 76th memory allocation.
//    // Or
//	_CrtSetBreakAlloc(76); // Break at the 76th memory allocation.
//
//	dprintf("asd");
//	return 0;
//}
