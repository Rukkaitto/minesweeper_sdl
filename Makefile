CFLAGS = -g -Wall -std=c99 
CPPFLAGS = -I./include
LDFLAGS = -L./target/lib -lgame
OBJETS = target/build/minesweeper_text.o target/build/game.o

all: target/build/minesweeper_text

run: target/build/minesweeper_text
	./$^

debug: target/build/minesweeper_text
	valgrind $^

target/build/minesweeper_text: target/build/minesweeper_text.o target/lib/libgame.a 
	$(CC) -o $@ $(LDFLAGS)

target/build/minesweeper_text.o: src/minesweeper_text.c include/game.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/minesweeper_text.c -o $@ 

target/build/game.o: src/game.c include/game.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/game.c -o $@ 

target/lib/libgame.a: $(OBJETS)
	ar -r $@ $^
	rm -f $(OBJETS)

clean: 
	rm -f target/lib/* target/build/* 

.phony: all run clean