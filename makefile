CFLAGS	= -g3 -Wall
EXEC	= diseaseMonitor
OBJS	= diseaseMonitor.o heap.o functions.o list.o rbt.o 


.PHONY : all clean run
all: $(EXEC) 

$(EXEC): $(OBJS) 

clean:
	rm -f $(OBJS) $(EXEC) 

run: $(EXEC) 
	./diseaseMonitor -p small.txt -h1 5 -h2 5 -b 70
	
val: $(EXEC) 
	valgrind -v --leak-check=full --show-leak-kinds=all ./diseaseMonitor \
	-p small.txt -h1 5 -h2 5 -b 70

