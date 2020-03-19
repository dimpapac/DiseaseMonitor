CFLAGS	= -g3 -Wall
EXEC	= diseaseMonitor
OBJS	= diseaseMonitor.o functions.o list.o rbt.o
# LDFLAGS = -lpthread

.PHONY : all clean run
all: $(EXEC) 

$(EXEC): $(OBJS) #$(LDFLAGS) 

clean:
	rm -f $(OBJS) $(EXEC) 

run: $(EXEC) 
	./diseaseMonitor -p patientRecordsFile -h1 1 -h2 13 -b 70
	
val: $(EXEC) $(CEXEC) $(QEXEC) $(HEXEC)
	valgrind -v --leak-check=full --show-leak-kinds=all ./diseaseMonitor \
	-p patientRecordsFile -h1 7 -h2 13 -b 50

