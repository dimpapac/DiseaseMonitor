CFLAGS	= -g3 -Wall
EXEC	= diseaseMonitor
OBJS	= diseaseMonitor.o functions.o list.o rbt.o


.PHONY : all clean run
all: $(EXEC) 

$(EXEC): $(OBJS) 

clean:
	rm -f $(OBJS) $(EXEC) 

run: $(EXEC) 
	./diseaseMonitor -p patientRecordsFile.txt -h1 1 -h2 1 -b 70
	
val: $(EXEC) 
	valgrind -v --leak-check=full --show-leak-kinds=all ./diseaseMonitor \
	-p patientRecordsFile.txt -h1 7 -h2 13 -b 50

