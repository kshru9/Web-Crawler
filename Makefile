maxlinks:= 100
pagelimit:= 100
threads:= 100
rankerFlag:= -sp
#error
all_targets:= compile run plots ranker clean

all: ${all_targets}

compile:
	@mkdir OUTPUT -p
	@clear
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o _crawler -lssl -lpthread -w

run:
	@echo "Running..."
	./_crawler $(maxlinks) $(pagelimit) $(threads)

plots:
	@mkdir OUTPUT -p
	python3 th_timings.py ${threads}

ranker:
	python3 ranker.py ${rankerFlag}

clean:
	rm -r -f _crawler
	rm -r -f logs.txt
	@echo "All cleaned."


test_bench:
	@rm -r -f OUTPUT/crawler_timings.csv
	g++ test_bench.cpp -o _tb
	./_tb
	@rm -r -f _tb
	python3 test_bench.py
	@echo "Successfully testing finished."