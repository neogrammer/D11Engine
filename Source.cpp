#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine engine;
	engine.Initialize(hInstance, "MyWindowClass", "Title",1280, 960);

	while (engine.ProcessMessages() == true)
	{
		engine.Update();
		engine.RenderFrame();
	}
	return 0;
}

// INPUT ASSEMBLER - complete
//  input layout - check
// VERTEX SHADER - complete
// RASTERIZER
// PIXEL SHADER
// OUTPUT MERGER