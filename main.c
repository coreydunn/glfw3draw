#include<GL/gl.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<assert.h>
#include<math.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>

void draw_pixels(uint32_t*pixels,GLuint texture,uint32_t width,uint32_t height,uint32_t x,uint32_t y);
void load_pixels(uint32_t*pixels,GLuint*texture,uint32_t width,uint32_t height);
void renderScene(void);

#define WIDTH 640
#define HEIGHT 480
#define IMAGE_MAX_WIDTH 1024
#define IMAGE_MAX_HEIGHT 1024

int main (int argc, char **argv)
{

	GLuint tex;
	SDL_Event event;
	SDL_GLContext context;
	SDL_Window *window;
	int32_t fullscreen=false;
	int32_t redraw=true;
	int32_t running=true;
	uint32_t image_height=256;
	uint32_t image_width=256;
	uint32_t mouse_x=0;
	uint32_t mouse_y=0;
	uint32_t window_height=480;
	uint32_t window_width=640;

	int32_t pixels[IMAGE_MAX_WIDTH*IMAGE_MAX_HEIGHT]={0};

	window=SDL_CreateWindow("OpenGL Test",0,0,WIDTH,HEIGHT,SDL_WINDOW_OPENGL);
	context=SDL_GL_CreateContext(window);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WIDTH,HEIGHT,0,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_SCISSOR_TEST);

	srand(time(NULL));

	for(int i=0,j=0xff;i<image_width*image_height;++i)
		pixels[i]=0xffffff00;
	glGenTextures(1,&tex);
	load_pixels(pixels,&tex,image_width,image_height);

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{

						case SDLK_ESCAPE:
						case SDLK_q:
							running = false;
							break;

						case SDLK_SPACE:
							for(int i=0;i<image_width*image_height;++i)
							{
								unsigned char red=rand()%256;
								unsigned char green=rand()%256;
								unsigned char blue=rand()%256;
								unsigned char alpha=0;

								pixels[i]=(red<<24)|(green<<16)|(blue<<8)|(alpha);
							}
							redraw=true;
							//load_pixels(pixels,&tex,image_width,image_height);
							break;

						case 'f':
							fullscreen = !fullscreen;
							if (fullscreen)
								SDL_SetWindowFullscreen(window,SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP);
							else
								SDL_SetWindowFullscreen(window,SDL_WINDOW_OPENGL);
							break;

					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mouse_x,&mouse_y);
					if(mouse_x<image_width&&mouse_y<image_height)
					{
						pixels[mouse_y*image_width+mouse_x]=0;
						printf("mouse_x: %u\nmouse_y: %u\n",mouse_x,mouse_y);
						redraw=true;
					}
					break;

				case SDL_QUIT:
					running=false;
					break;

			}
		}

		// Render
		if(redraw)
		{
			load_pixels(pixels,&tex,image_width,image_height);
			redraw=false;
		}
		glViewport(0, 0, window_width, window_height);
		SDL_GL_GetDrawableSize(window,&window_width,&window_height);

		glClearColor(0.5,0.5,0.5,0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw_pixels(pixels,tex,image_width,image_height,(window_height/2)-(image_width/2),(window_height/2)-(image_height/2));
		draw_pixels(pixels,tex,image_width,image_height,0,0);

		glFlush();
		SDL_GL_SwapWindow(window);
		usleep(20000);
	}

}

void draw_pixels(uint32_t*pixels,GLuint texture,uint32_t width,uint32_t height,uint32_t x,uint32_t y)
{
	glBindTexture(GL_TEXTURE_2D,texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	glColor3f(1,1,1);
	glTexCoord2f(0.0,0.0);
	glVertex2f(x+0.0,y+0.0);

	glTexCoord2f(1.0,0.0);
	glVertex2f(x+width,y+0.0);

	glTexCoord2f(1.0,1.0);
	glVertex2f(x+width,y+height);

	glTexCoord2f(0.0,1.0);
	glVertex2f(x+0.0,y+height);

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
