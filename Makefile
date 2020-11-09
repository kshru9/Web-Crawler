maxlinks:= 1000
pagelimit:= 10
threads:= 3

all_targets:= clear compile run clean

all: ${all_targets}

clear:
	@clear
	@rm -r -f thread_logs > /dev/null 2>&1
	@rm -r -f crawler > /dev/null 2>&1
	@mkdir thread_logs

compile:
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o crawler -lssl -lpthread

run:
	@echo "Running..."
	./crawler $(maxlinks) $(pagelimit) $(threads)

clean:
	@rm -r -f crawler > /dev/null 2>&1
	@rm -r -f thread_logs > /dev/null 2>&1
	@echo "All cleaned."
	