// execute.C
//
// Modified program: 	Jenny Zhen, March 2014
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
// add_to_ac() - add from memory to the accumulator.
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
// load_to_ac() - load accumulator from memory address given.
//

void load_to_ac() {
	
}

//
// store_to_mem() - store accumulator to memory address given.
//

void store_to_mem() {
}

//
// increment_skip_if_result_equals_zero() - 
// 		increments the contents of the memory address given.
//		If the result is zero, skip execution of the instruction following this one.
//		Note that the accumulator should not be changed by this instruction.
//

void increment_skip_if_result_equals_zero() {
}

//
// jump() - make the address given in the instruction the location of the next 
//			instruction to execute.

void jump() {
}

//
// halt() - halt the machine.
//

void halt() {
}

//
// branch_if_ac_equals_zero() - bzac; conditional branch on accumulator equal to zero.
//		If the AC has a value of zero, branch to the address given in the instruction.
//

void branch_if_ac_equals_zero() {
}

//
// swap_mem_with_ac() - swap memory with the accumulator.

void swap_mem_with_ac() {
}

//
// execute() - decode and execute the instruction
//

void execute() {
	long opc;

	// In each case, note that the last set of operations aren't actually performed 
	// until we leave the switch statement.
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
