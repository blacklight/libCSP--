SRC = csp++.cpp
OBJ = $(SRC:.cpp=.o)
OUT = libcsp++.a
INCLUDES = -I.
CCFLAGS = -O3 -g
CC = g++
FOURCOLOURS = fourcolours

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(OUT) $(EXAMPLE)

fourcolours:
	$(CC) $(INCLUDES) $(CCFLAGS) -o $(FOURCOLOURS) $(FOURCOLOURS).cpp $(OUT)

examples: fourcolours

clean-examples:
	rm ${FOURCOLOURS}
