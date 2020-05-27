#pragma once
extern void createApplication(void);
extern void deleteApplication(void);
#include "application.h"

int main(int argc, char* argv[])
{
	createApplication();
	run();
	deleteApplication();
}
