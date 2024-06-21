#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    // checks if the ALUControl is 0
    if (ALUControl == '0') {
        // ALUresult is the sum of A and B
        *ALUresult = A + B;
    }
    // checks if the ALUControl is 1
    else if (ALUControl == '1') {
        // ALUresult is the difference of A and B
        *ALUresult = A - B;
    }
    // checks if the ALUControl is 2
    else if (ALUControl == '2') {
        // checks if signed integer A is less than signed integer B
        if ((signed) A < (signed)B)
            // ALUresult is 1
            *ALUresult = 1;
        // checks if signed integer A isn't less than signed integer B
        else
            // ALUresult is 0
            *ALUresult = 0;
    }
    // checks if the ALUControl is 3
    else if (ALUControl == '3') {
        // checks if unsigned integer A is less than unsigned integer B
        if (A < B)
            // ALUresult is 1
            *ALUresult = 1;
        // checks if unsigned integer A isn't less than unsigned integer B
        else
            // ALUresult is 0
            *ALUresult = 0;
    }
    // checks if the ALUControl is 4
    else if (ALUControl == '4') {
        // ALUresult the result of A AND B
        *ALUresult = A & B;
    }
    // checks if the ALUControl is 5
    else if (ALUControl == '5') {
        // ALUresult the result of A OR B
        *ALUresult = A | B;

    }
    // checks if the ALUControl is 6
    else if (ALUControl == '6') {
        // ALUresult the result of B left shift by 16 bits
        *ALUresult = B << 16;

    }
    // checks if the ALUControl is 7
    else if (ALUControl == '7') {
        // ALUresult the result of NOT A
        *ALUresult = ~A;
    }
    // checks if the ALUresult is 0
    if (*ALUresult == 0)
        // assign zero to 1
        *Zero = '1';
    // if the ALUresult is not 0
    else
        // assign zero to 0
        *Zero = '0';
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // checks if the program counter is in bounds
    if (((PC % 4) == 0) && PC <= 0xFFFF && PC >= 0) {
      // fetch the instuction addressed by PC from Mem and write it to instruction
      *instruction = Mem[PC >> 2];
      // halt condition didn't occur
      return 0;
    }
    else
      // halt condition did occur
      return 1;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // check instruction partition and if statements and car
    // partition instuction into several parts (op, r1, r2, r3, funct, offset, jsec)
    // op is instruction [31-26]
    *op = (instruction & 0xFC000000) >> 26;
    // r1 is instruction [25-21]
    *r1 = (instruction & 0x03E00000) >> 21;
    // r2 is instruction [20-16]
    *r2 = (instruction & 0x001F0000) >> 16;
    // r3 is instruction [15-11]
    *r3 = (instruction & 0x0000F800) >> 11;
    // funct is instruction [5-0]
    *funct = instruction & 0x0000003F;
    // offset is instruction [15-0]
    *offset = instruction & 0x0000FFFF;
    // jsec is instruction [25-0]
    *jsec = instruction & 0x03FFFFFF;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // checks if the instruction is R-Type
    if (op == 0) {
        controls->RegDst = '1';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '7';
        controls->MemWrite = '0';
        controls->ALUSrc = '0';
        controls->RegWrite = '1';
    }
    // checks if the instruction is Jump
    else if (op == 2) {
        controls->RegDst = '2';
        controls->Jump = '1';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '0';
    }
    // checks if the instruction is BEQ
    else if (op == 4) {
        controls->RegDst = '2';
        controls->Jump = '0';
        controls->Branch = '1';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '1';
        controls->MemWrite = '0';
        controls->ALUSrc = '0';
        controls->RegWrite = '0';
    }
    // checks if the instruction is Add Immediate
    else if (op == 8) {
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    }
    // checks if the instruction is Set Less Than Immediate
    else if (op == 10) {
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '2';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    }
    // checks if the instruction is Set Less Than Immediate Unsigned
    else if (op == 11) {
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '3';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    }
    // checks if the instruction is load upper word immediate
    else if (op == 15) {
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '6';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    }
    // checks if the instruction is load word
    else if (op == 35) {
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '1';
        controls->MemtoReg = '1';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    }
    // checks if the instruction is store word
    else if (op == 43) {
        controls->RegDst = '2';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '0';
        controls->MemWrite = '1';
        controls->ALUSrc = '1';
        controls->RegWrite = '0';
    }
    // if the opcode wasn't found
    else
        // halt condition occurred
        return 1;
    // halt condition didn't occur
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // checks if the value of the AND operation with 16th bit in offset is 0
    if (((offset >> 15) & 1) == 1)
        // the first 16 bits of the extended value is 0
        *extended_value = 0xFFFF0000 | offset;
    // checks if the value of the AND operation with 16th bit in offset is 1
    else
        // the first 16 bits of the extended value is 1
        *extended_value = 0x0000FFFF & offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  
    if (ALUSrc == '1') {
        data2 = extended_value;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    // checks if the ALU source is asserted
    else {
        ALU(data1, data2, ALUOp, ALUresult, Zero);

    }

    // checks if the ALUOp is an R-Type Instruction
    if (ALUOp == '7') {
        if (funct == 32) {
            // ALU will do Addition
            ALUOp = '0';
        }
        else if (funct == 34) {
            // ALU will do Subtraction
            ALUOp = '1';
        }
        else if (funct == 36) {
            // ALU will do AND operation
            ALUOp = '4';
        }
        else if (funct == 37) {
            // ALU will do OR operation
            ALUOp = '5';
        }
        else if (funct == 42) {
            // ALU will do Set Less Than Signed
            ALUOp = '2';
        }
        else if (funct == 43) {
            // ALU will do Set Less Than Unsigned
            ALUOp = '3';
        }
        else
          // halt condition occured
          return 1;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
        // halt condition didn't occur
        return 0;
    }
    else {
        if (ALUOp == '0') {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
            return 0;
        }
        else if (ALUOp == '1') {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
            return 0;
        }
        else if (ALUOp == '2') {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
            return 0;
        }
        else if (ALUOp == '3') {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
            return 0;
        }
        else if (ALUOp == '6') {
            ALU(data1, data2, ALUOp, ALUresult, Zero);
            return 0;
        }
    }
    return 1;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {
    if (MemRead == '1') {
      if (ALUresult % 4 != 0)
        return 1;
      *memdata = Mem[ALUresult >> 2];
    }
    if (MemWrite == '1') {
      if (ALUresult % 4 != 0)
        return '1';
      Mem[ALUresult >> 2] = data2;
    }
  return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // checks if the register to write is 1
    if (RegWrite == '1') {
        // checks if information is from mem
        if (MemtoReg == '1') {
            Reg[r2] = memdata;
        //information must be coming from a register
        } else if (MemtoReg == '0') {
            if (RegDst == '1') {
                Reg[r3] = ALUresult;
                //r-type so save in r3
            } else {
                Reg[r2] = ALUresult;
                //save in r2
            }
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // next instruction = PC + 4
    *PC += 4;
    // if branch signal is asserted and zero signal is asserted, we shift the PC by the counter
    if (Branch == '1' && Zero == '1') {
        *PC += extended_value << 2;
    }
    if (Jump == '1') {
        *PC = (jsec << 2) | (*PC & 0xF000000);
    }
}

