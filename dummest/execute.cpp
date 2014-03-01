// execute.C
//
// Original program:	Jim Heliotis, March 2000
// Updates:
//
//	2000/11/29:	wrc, restructured code
//	2001/12/10:	wrc, split execute() into separate routines
//
// Instruction execution routine for the dummest little computer.

#include "includes.h"

//
// clear_ac() - clear the accumulator
//

void clear_ac() {

	ac.clear();

}

//
// add_to_ac() - add from memory to the accumulator
//

void add_to_ac() {

	// get memory operand using address in IR[11,0]
	fetch_into( ir, abus, mdr );

	// set up the addition
	alu.OP1().pullFrom( ac );
	alu.OP2().pullFrom( mdr );
	ac.latchFrom( alu.OUT() );
	alu.perform( BusALU::op_add );

}

//
// execute() - decode and execute the instruction
//

void execute() {
	long opc;

	// in each case, note that the last set of operations
	// aren't actually performed until we leave the switch
	// statement
	//
	// instr format:  opcode = bits 14:12, address = bits 11:0

	opc = ir( DATA_BITS-1, DATA_BITS-3 );

	switch( opc ) {

		case 0:	clear_ac();  break;

		case 1:	add_to_ac();  break;

		default:
			cout << "Illegal op code " << opc << endl;
			done = true;
	}

	Clock::tick();

}
