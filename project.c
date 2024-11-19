#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //This is Thai I'm working on this
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
    //TODO
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    //TODO
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
    int values = 0xffff << 16; *extended_value = (offset & 1 << 15 ? offset | values : offset);
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    //TODO
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
    //TODO
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //TODO
}

