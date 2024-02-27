############################################################
# VUT - FIT ISA projekt
############################################################
# @author Adam Hos <xhosad00@stud.fit.vutbr.cz>

CC=g++
CFLAGS=#-Wall -Werror -Wextra
LIBRARIES= -lGL -lglut -lGLU #-lGLEW
FILES=*.cpp *.h
NAME=main
COMPILEDNAME=simulator

$(NAME): $(FILES)
		$(CC) $(CFLAGS) -o $(COMPILEDNAME) $(FILES) $(LIBRARIES) 

clean:
	rm -f *.o $(COMPILEDNAME) 07_xhosad00_xpopdo00.tar.gz

# No wind, no generator, concentration 300000, max time 900 s
run1: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 0 -g

# Wind w2 -> strong South, no generator, concentration 300000, max time 900 s
run2: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 2 -g

# Wind w3 -> weak South, no generator, concentration 300000, max time 900 s
run3: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 3 -g

# No wind, generator, concentration 300000, max time 900 s
run4: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 0 -t 1 -g

# Wind w2 -> strong South, generator, concentration 300000, max time 900 s
run5: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 2 -t 1 -g

# Wind w3 -> weak South, generator, concentration 300000, max time 900 s
run6: $(NAME)
	./$(COMPILEDNAME) -m 900 -w 3 -t 1 -g

tar:
	tar -czvf 07_xhosad00_xpopdo00.tar.gz $(FILES) Makefile dokumentace.pdf
