CFLAGS=   -Wfatal-errors $(shell pkgconf --cflags glfw3 opengl)
LDFLAGS=  -lm $(shell pkgconf --libs glfw3 opengl)
OBJS=     sdlgl

all: $(OBJS)
sdlgl: main.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	$(RM) $(OBJS)
