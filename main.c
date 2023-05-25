#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void draw_pixels(uint32_t*pixels,GLuint texture,uint32_t width,uint32_t height);
void load_pixels(uint32_t*pixels,GLuint*texture,uint32_t width,uint32_t height);

void renderScene(void)
{
	GLfloat x, y, angle;
	int curSize=1;

	glClear(GL_COLOR_BUFFER_BIT);
	for(angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.1f)
	{
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);
		glPointSize(curSize);
		glBegin( GL_POINTS);
		glVertex3f(x, y, 0.0f);
		glEnd();
		curSize+=1;
	}
	/* glutSwapBuffers(); */
}

#define WIDTH 320
#define HEIGHT 240

int main (int argc, char **argv)
{

	GLuint tex;
	SDL_Event event;
	SDL_GLContext context;
	SDL_Window *window;
	double dx=160.0;
	double dy=120.0;
	int32_t fullscreen=false;
	int32_t pixels[16*16]={0};
	int32_t running=true;

	window = SDL_CreateWindow("OpenGL Test", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WIDTH,HEIGHT,0,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	srand(time(NULL));

	// Create, load texture
	for(int i=0;i<16*16;++i)
	{
		/* int red=rand()%0xff; */
		/* int green=rand()%0xff; */
		/* int blue=rand()%0xff; */
		int red=0;
		int green=0;
		int blue=0xff;
		pixels[i]=0xff;
		/* pixels[i]=(red<<24)|(green<<16)|(blue); */
	}
	glGenTextures(1,&tex);
	load_pixels(pixels,&tex,16,16);

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

					case SDLK_SPACE:
						if(dx==0.0)dx=160.0;
						else dx=0.0;
						if(dy==0.0)dy=120.0;
						else dy=0.0;
						printf("(%0.2f,%0.2f)\n",dx,dy);
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

		glColor3f(0.0f, 0.0f, 1.0f); // Let it be blue
		glBegin(GL_QUADS); // 2x2 pixels
		glVertex2f(1.0f, 1.0f);
		glVertex2f(2.0f, 1.0f);
		glVertex2f(2.0f, 2.0f);
		glVertex2f(1.0f, 2.0f);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f); // Let it be yellow.
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(2.5f, 0.5f);
		glVertex2f(2.5f, 2.5f);
		glVertex2f(0.5f, 2.5f);
		glVertex2f(0.5f, 0.5f);
		glEnd();
		glFlush();

		glBegin( GL_POINTS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 40.0f, 0.0f);
		glVertex3f(50.0f, 50.0f, 50.0f);
		glEnd();
		glFlush();

		GLfloat x,y,angle;
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
		for(angle=0.0f;angle<=(2.0f*M_PI);angle+=0.01f)
		{
			x=dx+50.0f*sin(angle);
			y=dy+50.0f*cos(angle);
			glVertex3f(x,y,0.0f);
		}
		glEnd();

		renderScene();

		draw_pixels(pixels,tex,16,16);

		glFlush();
		SDL_GL_SwapWindow(window);
		usleep(20000);
	}

}

void draw_pixels(uint32_t*pixels,GLuint texture,uint32_t width,uint32_t height)
{
	glBindTexture(GL_TEXTURE_2D,texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(0.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(width, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(width, height);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, height);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void load_pixels(uint32_t*pixels,GLuint*texture,uint32_t width,uint32_t height)
{
	glGenTextures(1,texture);
	glBindTexture(GL_TEXTURE_2D,*texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	if(pixels)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,pixels);
	else
		printf("Failed to load texture\n");
	glBindTexture(GL_TEXTURE_2D,0);
}
