all:
	gcc -pthread -D_GNU_SOURCE -I ./include \
	./src/sce_math/*.c \
	./src/sce_blas/level1/*.c \
	./src/sce_blas/level2/*.c \
	./src/*.c \
	main.c -lm 

