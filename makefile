policyTests: policyTests.cpp
	g++ -std=c++17 -o $@ $^ -ltbb
clean:
	rm policyTests
