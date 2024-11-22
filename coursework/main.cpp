#include "../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Coursework!", 1920, 1080, false);
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
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			renderer.toggleBloom();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			renderer.toggleBlackAndWhite();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
			renderer.toggleBlackAndWhiteSmooth();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_4)) {
			renderer.toggleSmooth();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_5)) {
			renderer.toggleBlur();
		}
	}
	return 0;
}