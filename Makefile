max: src/max.c
	gcc -Iinclude -o dist/max\
		src/max.c\
		src/module/cmdopts/cmdopts.c\
		src/module/version/version.c

clean:
	rm dist/max
