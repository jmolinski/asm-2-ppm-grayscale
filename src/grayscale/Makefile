all: grayscale

grayscale: grayscale-c.c ppm.o grayscale-asm.o
	gcc -std=c99 -o $@ $^

grayscale-asm.o: grayscale-asm.s
	as -o $@ $<

ppm.o: ppm/ppm.c ppm/ppm.h
	gcc -std=c99 -c -o $@ $<

clean:
	rm -vf grayscale *.o
