# engines
IGME-750 Game Engine Design and Development
Chengchen Yang, Reuben Brenner-Adams, Shashwat Sinha, Uday Shanker Reddy Bujala

To run in Visual Studio:
1. Download 64-bit versions of AntTweakBar, GLEW, OpenAL, SDL, SDL_net.
2. Link all libraries in Visual Studio.
3. Change configuration to Debug and x64. The project may work with other configurations, but this is the one we have been using.
4. Run the EngineServer project. There will be two windows that pop up: a black console window, and a white OpenGL window. Nothing else needs to be done with this project, but it must stay open and running.
5. Run the GameEngine project. The console window will pop up with no text prompt. Enter 127.0.0.1 and press enter. The OpenGL context window will now pop up.

Controls:
Esc. -> close the project

F1 -> make the project fullscreen
F2 -> make the project not fullscreen

mouse movement -> rotate the camera (FPS-style)

wasd -> move up, left, down, and right relative to the camera

mouse click on OpenGL context -> have context capture mouse for movement
p -> free mouse from capture of context

q -> play a sound