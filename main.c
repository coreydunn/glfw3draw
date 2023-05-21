#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#define WIDTH 320
#define HEIGHT 240

int main (int argc, char **argv)
{

	SDL_Event event;
	SDL_GLContext context;
	SDL_Window *window;
	int32_t running=true;
	int32_t fullscreen=false;

	window = SDL_CreateWindow("OpenGL Test", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{

				switch (event.key.keysym.sym)
				{

					case SDLK_ESCAPE:
					case SDLK_q:
						running = false;
						break;

					case 'f':
						fullscreen = !fullscreen;
						if (fullscreen)
							SDL_SetWindowFullscreen(window,SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP);
						else
							SDL_SetWindowFullscreen(window,SDL_WINDOW_OPENGL);
						break;

				}

			}
			else if (event.type==SDL_QUIT)
				running=false;
		}

		glClearColor(0.5,0.5,0.5,0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		/* glColor3f(0.0f, 0.0f, 1.0f); // Let it be blue */
		/* glBegin(GL_QUADS); // 2x2 pixels */
		/* glVertex2f(1.0f, 1.0f); */
		/* glVertex2f(2.0f, 1.0f); */
		/* glVertex2f(2.0f, 2.0f); */
		/* glVertex2f(1.0f, 2.0f); */
		/* glEnd(); */
        /*  */
		/* glColor3f(1.0f, 1.0f, 0.0f); // Let it be yellow. */
		/* glBegin(GL_LINE_STRIP); */
		/* glVertex2f(0.5f, 0.5f); */
		/* glVertex2f(2.5f, 0.5f); */
		/* glVertex2f(2.5f, 2.5f); */
		/* glVertex2f(0.5f, 2.5f); */
		/* glVertex2f(0.5f, 0.5f); */
		/* glEnd(); */
		/* glFlush(); */
        /*  */
		/* glBegin( GL_POINTS); */
		/* glVertex3f(0.0f, 0.0f, 0.0f); */
		/* glVertex3f(50.0f, 40.0f, 0.0f); */
		/* glVertex3f(50.0f, 50.0f, 50.0f); */
		/* glEnd(); */
		/* glFlush(); */

		GLfloat x, y, angle;
		glClear( GL_COLOR_BUFFER_BIT);
		glBegin( GL_POINTS);
		for (angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
		{
			x = 50.0f * sin(angle);
			y = 50.0f * cos(angle);
			glVertex3f(x, y, 0.0f);
		}
		glEnd();

		SDL_GL_SwapWindow(window);
		usleep(20000);
	}

}
