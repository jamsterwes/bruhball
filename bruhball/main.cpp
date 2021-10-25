#include "Engine.h"
#include "Gameplay/VibrationScript.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Engine engine(hInstance);

	engine.AddScript<VibrationScript>();
	engine.Run();

	return 0;
}