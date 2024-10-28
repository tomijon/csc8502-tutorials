#include "../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Vertex Transformation!", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	float scale = 100.0f;
	float rotation = 0.0f;
	float fov = 45.0f;

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			renderer.SwitchToPerspective();
			renderer.SetFOV(fov);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS))  ++scale;
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) --scale;

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))  ++rotation;
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) --rotation;

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_E))
			fov++;
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q))
			fov--;

		renderer.SetRotation(rotation);
		renderer.SetScale(scale);

		renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.RenderScene();
		renderer.SwapBuffers();
	}

	return 0;
}