#include<SDL2/SDL.h>
#include<GL/gl.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

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

void draw_ortho_pixels(uint32_t*pixels,uint32_t width)
{
	for(int i=0;i<16*16;++i)
	{
		float r=pixels[i]>>16&0xff;
		float g=pixels[i]>>8&0xff;
		float b=pixels[i]&0xf;

		glBegin(GL_POINTS);
		glColor3f(r,g,b);
		glVertex3f(i/width,i%width,0);
		glEnd();
	}
}

int main(void)
{
	SDL_GLContext c;
	SDL_Renderer*r;
	SDL_Window*w;
	int running=true;
	uint32_t pixels[16*16]={0};
	GLuint tex;

	for(int i=0,j=0xff;i<16*16;++i)
	{
		if(i%16==0)j=~j;
		pixels[i]=(j=~j);
	}
	for(int i=0;i<4;++i)
		printf("(%d,%d,%d)\n",
				pixels[i]>>16&0xff,
				pixels[i]>>8&0xff,
				pixels[i]&0xff
				);

	w=SDL_CreateWindow("ok",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
			320,240,0);
	r=SDL_CreateRenderer(w,-1,0);
	c=SDL_GL_CreateContext(w);
	SDL_RenderPresent(r);

	/* glMatrixMode(GL_PROJECTION); */
	glViewport(0,0,320,240);
	glLoadIdentity();
	glOrtho(0,320,240,0,0,1);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1,&tex);
	load_pixels(pixels,&tex,16,16);

	/* glBindTexture(GL_TEXTURE_2D,tex); */
	/* glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,pixels); */
	/* glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,16,16,0,GL_RGB,GL_UNSIGNED_INT_8_8_8_8,pixels); */
	/* glBindTexture(GL_TEXTURE_2D,0); */

	while(running)
	{
		SDL_Event e;
		if(SDL_PollEvent(&e))
		{
			switch(e.type)
			{

				case SDL_KEYDOWN:
					switch(e.key.keysym.sym)
					{

						case SDLK_q:
						case SDLK_ESCAPE:
							running=false;
							break;

					}
					break;

				case SDL_QUIT:
					running=false;
					break;

			}
		}

		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
			glColor3f(1,1,1);
			glVertex3f(10,10,0);
		glEnd();

		draw_pixels(pixels,tex,16,16);

		glFlush();
		SDL_GL_SwapWindow(w);
		usleep(20000);
	}

	SDL_DestroyWindow(w);
	SDL_GL_DeleteContext(&c);
	glDeleteTextures(1,&tex);
}
