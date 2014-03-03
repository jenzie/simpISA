simpISA
=======

Simulation of a simple 8-instruction CPU, using the C++ arch package.

execute.cpp  
	Added code for the 8-instructions (load, store, isz, jump, halt, bzac, add, and swp).

fetch_into.cpp  
	Move a word from the specified location in memory to the indicated destination via a particular bus.

globals.cpp  
	Global variables for "The Dummest Little Computer."
	
make_connections.cpp  
	Component connection function. At execution time, this will connect all the CPU components according to the design of the architecture.
	
run_simulation.cpp  
	The primary execution loop for "The Dummest Little Computer."  
	Added code to print the output.
	
simpISA.cpp  
	Contains the main() function from the original "The Dummest Little Computer."  
	On line 24, debug mode can be turned off.
		
globals.h  
	Extern declarations of global variables for "The Dummest Little Computer."
	
includes.h  
	Contains the system includes, arch library includes, and program specific includes.  
	Added cstdio to use printf for the output.
	
prototypes.h  
	Function prototypes for "The Dummest Little Computer."
