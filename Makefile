all:
	g++ -std=c++11 main.cpp

debug:
	g++ -std=c++11 main.cpp -g3

test:
	python tests/run_all.py
