maxlinks:= 1000000
pagelimit:= 100
threads:= 20
# er ror
all_targets:= clear compile run clean

all: ${all_targets}

clear:
	@rm -r -f thread_logs
	@rm -r -f crawler
	@mkdir thread_logs

compile:
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o crawler -lssl -lpthread -w

run:
	@echo "Running..."
	./crawler $(maxlinks) $(pagelimit) $(threads)
	#python graph.py

clean:
	rm -r -f crawler
	rm -r -f thread_logs
	rm -r -f logs.txt
	rm -r -f th_timings.csv
	@echo "All cleaned."