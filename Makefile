all:
	g++ -std=c++11 main.cpp -pthread

debug:
	g++ -std=c++11 main.cpp -g3 -pthread

test:
	python tests/run_all.py
