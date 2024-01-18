CC=gcc
end=-w -fopenmp -lm -lgsl -lgslcblas -O3

all: Fig3_formula Fig4_formula Fig5_formula Fig6_formula Fig7_formula Fig3_simulation Fig4_simulation Fig5_simulation Fig6_simulation Fig7_simulation clean

Fig3_formula: Fig3_formula.c
	${CC} -o Fig3_formula Fig3_formula.c ${end}

Fig4_formula: Fig4_formula.c
	${CC} -o Fig4_formula Fig4_formula.c ${end}

Fig5_formula: Fig5_formula.c
	${CC} -o Fig5_formula Fig5_formula.c ${end}

Fig6_formula: Fig6_formula.c
	${CC} -o Fig6_formula Fig6_formula.c ${end}

Fig7_formula: Fig7_formula.c
	${CC} -o Fig7_formula Fig7_formula.c ${end}

Fig3_simulation: Fig3_simulation.c calc.o
	${CC} -o Fig3_simulation Fig3_simulation.c calc.o ${end}

Fig4_simulation: Fig4_simulation.c calc.o
	${CC} -o Fig4_simulation Fig4_simulation.c calc.o ${end}

Fig5_simulation: Fig5_simulation.c calc.o
	${CC} -o Fig5_simulation Fig5_simulation.c calc.o ${end}

Fig6_simulation: Fig6_simulation.c calc.o
	${CC} -o Fig6_simulation Fig6_simulation.c calc.o ${end}

Fig7_simulation: Fig7_simulation.c calc.o
	${CC} -o Fig7_simulation Fig7_simulation.c calc.o ${end}

calc.o: calc.c
	${CC} -o calc.o -c calc.c ${end}

clean:
	rm *.o
