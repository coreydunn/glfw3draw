CFLAGS=   -Wfatal-errors $(shell pkgconf --cflags sdl2 opengl)
LDFLAGS=  -lm $(shell pkgconf --libs sdl2 opengl)
OBJS=     sdlgl

all: $(OBJS)
sdlgl: main.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	$(RM) $(OBJS)
