#include "Main.h"
#include "Level.h"
#include "RenderManager.h"
#include "GameManager.h"

#include <VrLib/Kernel.h>
#include <cstdlib>

void main(int argc, char* argv[])
{
	Kernel* kernel = Kernel::getInstance();

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--config") == 0)
		{
			i++;
			kernel->loadConfig(argv[i]);
		}
	}

	kernel->setApp(GameManager::getInstance());
	kernel->start();
}