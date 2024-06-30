#include "Main.h"
#include "Aimbot.h"

int main()
{
	arduino = new CArduino("COM7"); // Port wo das Arduino verbunden ist.
	if (!arduino->ConnectToPort()) // Verbinden mit Arduino
	{
		printf("Arduino nicht verbunden. \n");
		return 1;
	}

	mem = new CMemory("cs2.exe");
	if (!mem->Attach()) // Verbinden mit Prozess
	{
		printf("OpenProcess hat wohl versagt. \n");
		return 1;
	}

	printf("PID: %i \n", mem->GetPid());

	clientModule = mem->FindModule("client.dll");
	if (!clientModule)
		return 1;
	
	printf("client.dll: 0x%llx \n", clientModule);

	while (!GetAsyncKeyState(VK_END)) // Selbstzerstörungs Taste
	{
		aim::DoAim();

		Sleep(1);
	}

	delete(mem);
	delete(arduino);

	return 0;
}