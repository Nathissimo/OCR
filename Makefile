# Makefile

CC = gcc
CFLAGS = -Wall -Wextra `pkg-config sdl2 --cflags`
CPPFLAGS =
LDLIBS = `pkg-config sdl2 --libs`
LDFLAGS =

all: interface

temp_files/info.txt:
	mkdir temp_files
	touch temp_files/info.txt

tempfiles: temp_files/info.txt
	touch temp_files/info.txt
	

interface: tempfiles Network/training.o Network/network.o change_image/dilatation_and_erosion.o change_image/binarization.o change_image/grayscale.o change_image/blur.o change_image/sobel.o change_image/rotation.o change_image/detection.o change_image/draw_grid.o solve/solver.o change_image/rescale.o change_image/cut.o main.o interface.o
	gcc  Network/training.o Network/network.o change_image/dilatation_and_erosion.o change_image/binarization.o change_image/grayscale.o change_image/blur.o change_image/sobel.o change_image/rotation.o change_image/detection.o  change_image/draw_grid.o solve/solver.o change_image/rescale.o change_image/cut.o  main.o interface.o -lm  `pkg-config --libs sdl2 SDL2_image SDL2_ttf gtk+-3.0` -o interface
interface.o: interface.c
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image SDL2_ttf gtk+-3.0` -c -o interface.o interface.c

clean_temp_files:
	rm -f temp_files/*

clean:
	rm -f solve/*.o
	rm -f change_image/*.o
	rm -f Neuron/*.o
	rm -f *.neuron
	rm -f Network/*.o
	rm -f main.o
	rm -f interface.o
	rm -f interface
	rm -f temp_files/*
	rm -Rf temp_files
	rm -f sudoku_result.png
# END
