CFLAGS=   -Wfatal-errors $(shell pkgconf --cflags glfw3 opengl)
LDFLAGS=  -lm $(shell pkgconf --libs glfw3 opengl)
OBJS=     glfw3draw

all: $(OBJS)
glfw3draw: main.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	$(RM) $(OBJS)
