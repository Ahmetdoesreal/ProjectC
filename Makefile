appointment: appointment.o readline.o student.o teacher.o
	gcc-13 -o appointment appointment.o readline.o student.o teacher.o
appointment.o: appointment.c readline.h student.h teacher.h
	gcc-13 -c appointment.c
readline.o: readline.c readline.h
	gcc-13 -c readline.c
student.o: student.c student.h
	gcc-13 -c student.c
teacher.o: teacher.c teacher.h
	gcc-13 -c teacher.c