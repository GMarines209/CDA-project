#include "spimcore.h"

//This is Thai I try to fill the todo code, can your guys check if there's any eror

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl){
        case 0:  //Addition
           *ALUresult = A+B;
            break;
        case 1:  //Subtraction
           *ALUresult = A-B;
           break;
        case 2: // < Signed
           if(A < B){
               *ALUresult = 1;
           }
           else
               *ALUresult = 0;
           break;
        case 3: // < Unsigned
           if(A < B){
               *ALUresult = 1;
           }
           else
               *ALUresult = 0;
           break;
        case 4: //AND
           if(A && B){
               *ALUresult = 1;
           }
           else
               *ALUresult = 0;
           break;
        case 5: //OR
           if(A || B){
               *ALUresult = 1;
           }
           else
               *ALUresult = 0;
           break;
        case 6: //Shift B left by 16 bits
           *ALUresult = B << 16;
           break;
        case 7:  //NOT A
           *ALUresult = (~A);
           break;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //instruction addressed from mem sent to 'instruction'
    int i = PC >> 2;
    if (PC % 4 != 0)//the halt as specified
        return 1;
    //else case if halt condition does not occur
    else
        *instruction = Mem[i];

    return 0;

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // fetching the op code
    *op = (instruction >> 26);

    // if the instruction is J type
    if (*op == 2)
    {
        // store jump target calculator
        *jsec = instruction & ((1 << 26) - 1);

        return;
    }

    // fetching the first two registers
    *r1 = (instruction >> 21) & 31;
    *r2 = (instruction >> 16) & 31;

    // if the instruction is R type
    if (*op == 0)
    {
        // fetch the destination register
        *r3 = (instruction >> 11) & 31;

        // function code
        *funct = instruction & 63;

        return;
    }

    // if the instruction is I type fetch the last 16 bits to store the constant
    *offset = instruction & ((1 << 16) - 1);
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    switch(op)
    {
    case 0: controls->RegDst = 1;   controls->ALUSrc = 0;  controls->MemtoReg = 0;
            controls->RegWrite = 1; controls->MemRead = 0; controls->MemWrite = 0;
            controls->Branch = 0;   controls->Jump = 0;    controls->ALUOp = 7;
            return 0;

    case 2: controls->RegDst = 2;   controls->ALUSrc = 2;  controls->MemtoReg = 2;
            controls->RegWrite = 0; controls->MemRead = 0; controls->MemWrite = 0;
            controls->Branch = 0;   controls->Jump = 1;    controls->ALUOp = 2;
            return 0;

    case 4: controls->RegDst = 2;   controls->ALUSrc = 0;  controls->MemtoReg = 2;
            controls->RegWrite = 0; controls->MemRead = 0; controls->MemWrite = 0;
            controls->Branch = 1;   controls->Jump = 0;    controls->ALUOp = 1;
            return 0;

    case 8: controls->RegDst = 0;   controls->ALUSrc = 1;  controls->MemtoReg = 0;
            controls->RegWrite = 1; controls->MemRead = 0; controls->MemWrite = 0;
            controls->Branch = 0;   controls->Jump = 0;    controls->ALUOp = 0;
            return 0;

    case 35:controls->RegDst = 0;   controls->ALUSrc = 1;  controls->MemtoReg = 1;
            controls->RegWrite = 1; controls->MemRead = 1; controls->MemWrite = 0;
            controls->Branch = 0;   controls->ALUOp = 0;   controls->Jump = 0;
            return 0;

    case 43:controls->RegDst = 2;   controls->ALUSrc = 1;  controls->MemtoReg = 2;
            controls->RegWrite = 0; controls->MemRead = 0; controls->MemWrite = 1;
            controls->Branch = 0;   controls->ALUOp = 0;   controls->Jump = 0;
            return 0;

    default : return 1;
  }
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

    //values read from register and then inputted into data1 and 2
    *data1 = Reg[r1];
    *data2 = Reg[r2];

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
     unsigned signBit = offset >> 15; /* the sign but is the left most bit of the 16 bit string */
    if(signBit == 1) {
        /* negative, expand to 32 bits */
        *extended_value = offset | 0xFFFF0000; /* left half are all 1's */
    } else {
        /* not negative, just keep at 16 bits */
        *extended_value = offset & 0x0000ffff;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
     // if ALUSrc is 0, we use data2 with data1 to make calculations
    if (ALUSrc == 0)
    {
        // if ALUOp is 7, the instruction is R type. So, check the function code
        if (ALUOp == 7)
        {
            switch(funct)
            {
                // addition
                case 32: ALU(data1, data2, 0, ALUresult, Zero);
                         return 0;

                // subtract
                case 34: ALU(data1, data2, 1, ALUresult, Zero);
                         return 0;

                // and
                case 36: ALU(data1, data2, 4, ALUresult, Zero);
                         return 0;

                // or
                case 37: ALU(data1, data2, 5, ALUresult, Zero);
                         return 0;

                // set less than
                case 42: ALU(data1, data2, 2, ALUresult, Zero);
                         return 0;

                // set less than unsigned
                case 43: ALU(data1, data2, 3, ALUresult, Zero);
                         return 0;

                // if we have an unknown function code, halt
                default: return 1;
            }
        }

        // if instruction is not R type, use ALUOp to make calculations
        ALU(data1, data2, ALUOp, ALUresult, Zero);
        return 0;
    }

    // if ALUSrc is 1, use extended_value along with data1 to make calculations
    ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

    if (MemWrite || MemRead == 1) {
        if ((ALUresult % 4) == 0)
            Mem[ALUresult >> 2] = data2;
        
        else
            //halt
            return 1;
    }
    //other halt condition
    return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // if RegWrite is activated
    if (RegWrite == 1){
        if (RegDst == 1){    //RegDst = 1, set r3 to ALUresult
            Reg[r3] = ALUresult;
            
            if (MemtoReg == 1)     //MemtoReg active, set r3 to memdata
                Reg[r3] = memdata;
        }

        if (RegDst == 0){      //RegDst = 0, set r2 to ALUresult
            Reg[r2] = ALUresult;

            if (MemtoReg == 1)    //MemtoReg active, set r2 to memdata
                Reg[r2] = memdata;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4;

   //Checks to see if Branch and Zero is in ALU
   if(Branch == 1 && Zero == 1){
       //Extended value is added to PC
       *PC += (extended_value << 2);
   }
   
    //If Jump occurs, shift left two times
   if(Jump == 1){
       *PC =  (*PC & 0xf000000) | (jsec << 2);
   }
}

