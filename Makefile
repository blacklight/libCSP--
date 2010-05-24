INCLUDES = -I.
CCFLAGS = -O3 -g
CC = g++
FOURCOLOURS = fourcolours
SUDOKU = sudoku
SUFFIX = .cpp
INCLUDEDIR=csp++
INSTALLDIR=/usr/local

examples: fourcolours sudoku

examples-clean: fourcolours-clean sudoku-clean

install:
	mkdir -p ${INSTALLDIR}/include
	mkdir -p ${INSTALLDIR}/include/${INCLUDEDIR}
	cp ${INCLUDEDIR}/csp++-def.h ${INSTALLDIR}/include/${INCLUDEDIR}
	cp ${INCLUDEDIR}/csp++.h ${INSTALLDIR}/include/${INCLUDEDIR}
	cp ${INCLUDEDIR}/csp++.cpp ${INSTALLDIR}/include/${INCLUDEDIR}

fourcolours:
	$(CC) $(INCLUDES) $(CCFLAGS) -o $(FOURCOLOURS) $(FOURCOLOURS)${SUFFIX}

sudoku:
	$(CC) $(INCLUDES) $(CCFLAGS) -o $(SUDOKU) $(SUDOKU)${SUFFIX}

fourcolours-clean:
	rm ${FOURCOLOURS}

sudoku-clean:
	rm ${SUDOKU}
