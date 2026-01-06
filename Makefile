max: src/max.c
	gcc -o dist/max\
		src/max.c\
		src/module/parser/parser.c\
		src/module/cmdopts/cmdopts.c\
		src/module/version/version.c

clean:
	rm dist/max
