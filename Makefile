maxlinks:= 1000
pagelimit:= 100
threads:= 50
#error
all_targets:= compile run plots clean

all: ${all_targets}

compile:
	@clear
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o _crawler -lssl -lpthread -w

run:
	@echo "Running..."
	./_crawler $(maxlinks) $(pagelimit) $(threads)
	
plots:
	@mkdir OUTPUT -p
	python th_timings.py ${threads}

clean:
	rm -r -f _crawler
	rm -r -f logs.txt
	@echo "All cleaned."


test_bench:
	@rm -r -f OUTPUT/crawler_timings.csv
	@g++ test_bench.cpp -o _tb
	@./_tb
	@rm -r -f _tb
	python test_bench.py