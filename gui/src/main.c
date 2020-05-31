#include "win32.h"
int WinMain(HINSTANCE instance, HINSTANCE previousInstance, char* cmd, int showCmd)
{
	MessageBox(NULL, "Hello Windows", "My message", 0);
	return 0;
}