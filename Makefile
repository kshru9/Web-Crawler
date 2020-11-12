maxlinks:= 1000
pagelimit:= 100

all_targets:= clear compile run clean

all: ${all_targets}

clear:
	@clear
	@rm -r -f crawler > /dev/null 2>&1

compile:
	@echo "Compiling file..."
	@g++ -std=c++14 main.cpp -o crawler -lssl 

run:
	@echo "Running..."
	./crawler $(maxlinks) $(pagelimit) 


clean:
	@rm -r -f crawler > /dev/null 2>&1
	@echo "All cleaned."
	
