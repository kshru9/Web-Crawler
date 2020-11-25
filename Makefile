maxlinks:= 1000
pagelimit:= 1000
threads:= 200

all_targets:= clear compile run clean

all: ${all_targets}

clear:
	@clear
	@rm -r -f thread_logs > /dev/null 2>&1
	@rm -r -f crawler > /dev/null 2>&1
	@mkdir thread_logs

compile:
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o crawler -lssl -lpthread -w

run:
	@echo "Running..."
	./crawler $(maxlinks) $(pagelimit) $(threads)
	#@python graph.py
# valgrind --tool=helgrind 
clean:
	@rm -r -f crawler > /dev/null 2>&1
	@rm -r -f thread_logs > /dev/null 2>&1
	@rm -r -f logs.txt > /dev/null 2>&1
	@echo "All cleaned."