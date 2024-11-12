#include "../nclgl/window.h"
#include "Renderer.h"
#include <chrono>
#include <string>

int main() {	
	Window w("Index Buffers!", 1280, 720,false);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	int total_frames = 0;
	auto now = std::chrono::high_resolution_clock::now();

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		total_frames += 1000;
		float timestep = w.GetTimer()->GetTimeDeltaSeconds();
		renderer.UpdateScene(timestep);
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}

	auto last = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> duration = last - now;
	std::cout << "DURATION: " << duration.count() << std::endl;
	std::cout << "FRAME_RATE: " << total_frames / duration.count() << std::endl;
	
	return 0;
}