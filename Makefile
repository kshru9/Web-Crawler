maxlinks:= 5
pagelimit:= 100
threads:= 100
# er ror
all_targets:= compile run plots clean

all: ${all_targets}

compile:
	@clear
	@echo "Compiling file..."
	g++ -std=c++14 main.cpp -o crawler -lssl -lpthread -w

run:
	@echo "Running..."
	./crawler $(maxlinks) $(pagelimit) $(threads)
	
plots:
	@mkdir OUTPUT -p
	python graph.py

clean:
	rm -r -f crawler
	rm -r -f logs.txt
	@echo "All cleaned."