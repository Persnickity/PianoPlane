test:
	make build;
	./PianoPlane;

build:
	gcc -I"/usr/include/SDL" -c PianoPlane.c;
	gcc PianoPlane.o -lSDL -o PianoPlane -lSDL_mixer;

clean:
	-rm PianoPlane.o;
	-rm PianoPlane;


