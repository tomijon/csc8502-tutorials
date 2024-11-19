#include "../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Coursework!", 1080, 720, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float timestep = w.GetTimer()->GetTimeDeltaSeconds();
		renderer.getAnimator()->update(w.GetTimer()->GetTotalTimeSeconds(), timestep);
		renderer.UpdateScene(timestep);
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
			renderer.applyTimeSkip();
		}
	}
	return 0;
}