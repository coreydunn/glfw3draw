#include<GLFW/glfw3.h>
#include<fcntl.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<unistd.h>

#define WIDTH 640
#define HEIGHT 480
#define IMAGE_MAX_WIDTH 1024
#define IMAGE_MAX_HEIGHT 1024

void draw_pixels(uint32_t*pixels,GLuint texture,uint32_t width,uint32_t height,uint32_t x,uint32_t y);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void load_pixels(uint32_t*pixels,GLuint*texture,uint32_t width,uint32_t height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

static double mouse_x=0;
static double mouse_y=0;
static int32_t pixels[IMAGE_MAX_WIDTH*IMAGE_MAX_HEIGHT]={0};
static uint32_t image_height=256;
static uint32_t image_width=256;
static uint32_t window_height=480;
static uint32_t window_width=640;
static uint32_t redraw=true;

int main(void)
{

	GLFWwindow* window;
	GLuint tex;
	int32_t running=true;

	srand(time(NULL));

	// GLFW3
	glfwInit();
	glfwSetErrorCallback(error_callback);
	window=glfwCreateWindow(window_width, window_height, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSwapInterval(1);

	// OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WIDTH,HEIGHT,0,0,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	// Texture
	for(int i=0,j=0xff;i<image_width*image_height;++i)
		pixels[i]=0xffffff00;
	glGenTextures(1,&tex);
	load_pixels(pixels,&tex,image_width,image_height);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glfwGetCursorPos(window,&mouse_x,&mouse_y);

		// Render
		if(redraw)
		{
			load_pixels(pixels,&tex,image_width,image_height);
			redraw=false;
		}

		glClearColor(0.5,0.5,0.5,0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw_pixels(pixels,tex,image_width,image_height,(window_height/2)-(image_width/2),(window_height/2)-(image_height/2));
		draw_pixels(pixels,tex,image_width,image_height,0,0);

		glFlush();
		glfwSwapBuffers(window);
		usleep(20000);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action==GLFW_PRESS)
	{
		switch(key)
		{

			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;

			case GLFW_KEY_SPACE:
				for(int i=0;i<image_width*image_height;++i)
				{
					unsigned char red=rand()%256;
					unsigned char green=rand()%256;
					unsigned char blue=rand()%256;
					unsigned char alpha=0;

					pixels[i]=(red<<24)|(green<<16)|(blue<<8)|(alpha);
				}
				redraw=true;
				break;

		}
	}
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	glfwGetCursorPos(window,&mouse_x,&mouse_y);

	if(button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS)
	{
		if(mouse_x<image_width&&mouse_y<image_height)
		{
			pixels[(uint32_t)mouse_y*(uint32_t)image_width+(uint32_t)mouse_x]=0;
			printf("mouse_x: %0.2f\nmouse_y: %0.2f\n",mouse_x,mouse_y);
			redraw=true;
		}
	}

}
