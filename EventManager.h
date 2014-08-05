#pragma once

struct GLFWwindow;

class EventManager {
public:
	static GLFWwindow* getWindow();
	static void setUp();
	static void terminateProgram();
	static bool exitLoop();
	static void update();

	static float getFrameTime();
	static float getMouseMovementX();
	static float getMouseMovementY();
private:
	
	static GLFWwindow* window;
	static int windowWidth;
	static int windowHeight;

	static float sMouseDeltaX;
	static float sMouseDeltaY;
	static float sLastMousePositionX;
	static float sLastMousePositionY;

	static float sFrameTime;
	static float sLastFrameTime;
};