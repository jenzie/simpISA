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
// load_to_ac() - load accumulator from memory address given.
// 
// RTL (Register Transfer Language)
// MAR <- IR[11-0]
// MDR <- Mem[MAR]
// AC <- MDR
//
// Code 0
//

void load_to_ac() {

	/*
	Without using fetch_into().

	// MAR <- IR[11-0]
	abus.IN().pullFrom(ir); // buses are pull from
	m.mar.IN().latchFrom(abus.OUT()); // registers are latch from; have to specify IN/OUT for buses (not storage objects, i.e. register)
	Clock::tick();
	
	// MDR <- Mem[MAR]
	m.read();
	mdr.latchFrom(m.READ());
	Clock::tick();

	// AC <- MDR
	dbus.IN().pullFrom(mdr);
	ac.latchFrom(dbus.OUT());
	*/

	// Using fetch_into().
	// MAR <- IR[11-0] and MDR <- Mem[MAR]
	fetch_into(ir, abus, mdr);

	// AC <- MDR
	dbus.IN().pullFrom(mdr);
	ac.latchFrom(dbus.OUT());

}

//
// store_to_mem() - store accumulator to memory address given.
//
// RTL (Register Transfer Language)
// MAR <- IR[11-0], MDR <- AC
// Mem[MAR] <- MDR
//
// Code 1
//

void store_to_mem() {

	// MAR <- IR[11-0], MDR <- AC
	// The parallelization is simulated by the Clock::tick() for this CPU.
	abus.IN().pullFrom(ir);
	m.mar.IN().latchFrom(abus.OUT());

	dbus.IN().pullFrom(ac);
	mdr.latchFrom(dbus.OUT());

	Clock::tick();

	// Mem[MAR] <- MDR
	m.mar.IN().latchFrom(mdr);

}

//
// increment_skip_if_result_equals_zero() - 
// 		increments the contents of the memory address given.
//		If the result is zero, skip execution of the instruction following this one.
//		Note that the accumulator should not be changed by this instruction.
//
// RTL (Register Transfer Language)
// MAR <- IR[11-0]
// MDR <- Mem[MAR]
// MDR <- MDR + 1
// Mem[MAR] <- MDR
// if MDR == 0 then PC <- PC + 1
//
// Code 2
//

void increment_skip_if_result_equals_zero() {

	// MAR <- IR[11-0] and MDR <- Mem[MAR]
	fetch_into(ir, abus, mdr);

	// MDR <- MDR + 1
	mdr.incr();
	Clock::tick();

	// Mem[MAR] <- MDR
	m.mar.IN().latchFrom(mdr);
	Clock::tick();

	// if MDR == 0 then PC <- PC + 1
	if (mdr.read() == 0)
		pc.incr();

}

//
// jump() - make the address given in the instruction the location of the next 
//			instruction to execute.
//
// RTL (Register Transfer Language)
// PC <- IR(11-0)
//
// Code 3
//

void jump() {

	// PC <- IR[11-0]
	abus.IN().pullFrom(ir);
	pc.latchFrom(abus.OUT());

}

//
// halt() - halt/stop the machine.
//
// RTL (Register Transfer Language)
// null
//
// Code 4
//

void halt() {

	cout << endl << "MACHINE HALTED due to halt instruction" << endl << endl;
	exit();

}

//
// branch_if_ac_equals_zero() - bzac; conditional branch on accumulator equal to zero.
//		If the AC has a value of zero, branch to the address given in the instruction.
//
// RTL (Register Transfer Language)
// if AC == 0 then PC <- IR[11-0]
//
// Code 5
//

void branch_if_ac_equals_zero() {

	// if AC == 0 then PC <- IR[11-0]
	if (ac.read() == 0) {
		abus.IN().pullFrom(ir);
		pc.latchFrom(abus.OUT);
	}

}

//
// add_to_ac() - add from memory to the accumulator.
//
// RTL (Register Transfer Language)
// MAR <- IR[11-0]
// MDR <- Mem[MAR]
// AC <- AC + MDR
//
// Code 6
//

void add_to_ac() {

	// Get memory operand using address in IR[11,0].
	// MAR <- IR[11-0] and MDR <- Mem[MAR]
	fetch_into(ir, abus, mdr);

	// Set up the addition.
	// AC <- AC + MDR
	alu.OP1().pullFrom(ac);
	alu.OP2().pullFrom(mdr);
	ac.latchFrom(alu.OUT());
	alu.perform(BusALU::op_add);

}

//
// swap_mem_with_ac() - swap memory with the accumulator.
//
// RTL (Register Transfer Language)
// MAR <- IR[11-0]
// MDR <- Mem[MAR]
// AC <- MDR, MDR <- AC
// Mem[MAR] <- MDR
//
// Code 7
//

void swap_mem_with_ac() {

	// MAR <- IR[11-0] and MDR <- Mem[MAR]
	fetch_into(ir, abus, mdr);

	// AC <- MDR, MDR <- AC
	// Value of AC goes to Dbus, Dbus goes to MDR.
	dbus.IN().pullFrom(ac);
	mdr.latchFrom(dbus.OUT());

	// MDR goes to ALU, ALU does pass through op_rop2. ALU goes to AC.
	alu.OP2().pullFrom(mdr);
	alu.perform(BusALU::op_rop2); // ALU does not transfer data unless if there is an operation to perform.
	ac.latchFrom(alu.OUT());
	Clock::tick();

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
