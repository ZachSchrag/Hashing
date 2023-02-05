CXXFLAGS = -std=c++17 -Wall -Wextra -Weffc++ -pedantic-errors -g

objects = separate_chaining open_addressing

all:  $(objects)

memory_errors: separate_chaining_memory_errors open_addressing_memory_errors

clean: 
	rm -f *.gcov *.gcda *.gcno a.out
	
$(objects): %: clean hashtable_%.h hashtable_%_tests.cpp
	g++ $(CXXFLAGS) --coverage hashtable_$@_tests.cpp && ./a.out && gcov -mr hashtable_$@_tests.cpp

separate_chaining_memory_errors: %_memory_errors: clean hashtable_%.h hashtable_%_tests.cpp
	g++ $(CXXFLAGS) hashtable_separate_chaining_tests.cpp && valgrind --leak-check=full ./a.out
	
open_addressing_memory_errors: %_memory_errors: clean hashtable_%.h hashtable_%_tests.cpp
	g++ $(CXXFLAGS) hashtable_open_addressing_tests.cpp && valgrind --leak-check=full ./a.out
	