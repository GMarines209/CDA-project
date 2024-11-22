#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    switch (ALUControl) {
        case 0:  
            *ALUresult = A + B;
            break;

        case 1: 
            *ALUresult = A - B;
            break;

        case 2: 
            *ALUresult = ((int)A < (int)B) ? 1 : 0;
            break;

        case 3: 
            *ALUresult = ((unsigned)A < (unsigned)B) ? 1 : 0;
            break;

        case 4: 
            *ALUresult = A & B;
            break;

        case 5:  
            *ALUresult = A | B;
            break;

        case 6: 
            *ALUresult = B << 16;
            break;

        case 7: 
            *ALUresult = ~A;
            break;

        default:
            *Zero = 0;
            return; 
    }

    if (*ALUresult == 0) {
        *Zero = 1;
    } else {
        *Zero = 0;
    }
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    if ((PC % 4) != 0 ) {
        return 1; 
    }
    unsigned index = PC >> 2;

    *instruction = Mem[index];

    return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

    *op = instruction >> 26;
    *r1 = (instruction >> 21) & 0x1F;
    *r2 = (instruction >> 16) & 0x1F;
    *r3 = (instruction >> 11) & 0x1F;
    *funct = instruction & 0x3F;
    *offset = instruction & 0xFFFF;
    *jsec = instruction & 0x3FFFFFF;
}




/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    controls->RegDst = 0;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->RegWrite = 0;
    controls->MemtoReg = 0;
    controls->ALUSrc = 0;
    controls->ALUOp = 0;

    switch (op) {
        case 0x00: 
            controls->RegDst = 1;      
            controls->ALUSrc = 0;      
            controls->RegWrite = 1;    
            controls->MemRead = 0;     
            controls->MemWrite = 0;    
            controls->MemtoReg = 0;    
            controls->ALUOp = 7;       
            break;

        case 0x23:  // lw (load word)
            controls->RegDst = 0;    
            controls->ALUSrc = 1;    
            controls->RegWrite = 1;  
            controls->MemRead = 1;   
            controls->MemWrite = 0;  
            controls->MemtoReg = 1;  
            controls->ALUOp = 0;     
            break;

        case 0x2B:  // sw (store word)
            controls->RegDst = 2;     
            controls->ALUSrc = 1;     
            controls->RegWrite = 0;   
            controls->MemRead = 0;    
            controls->MemWrite = 1;   
            controls->MemtoReg = 2;   
            controls->ALUOp = 0;      
            break;

        case 0x04:  // beq (branch if equal)
            controls->RegDst = 2;     
            controls->ALUSrc = 1;     
            controls->RegWrite = 0;   
            controls->MemRead = 0;    
            controls->MemWrite = 0;   
            controls->MemtoReg = 2;   
            controls->Branch = 1;     
            controls->ALUOp = 1;      
            break;

        case 0x02:  // j (jump)
            controls->RegDst = 2;     
            controls->ALUSrc = 2;     
            controls->RegWrite = 0;   
            controls->MemRead = 0;    
            controls->MemWrite = 0;   
            controls->MemtoReg = 2;   
            controls->Branch = 2;     
            controls->Jump = 1;       
            controls->ALUOp = 2;

            break;
        case 0x8:
            controls->RegDst = 0;  
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;  
            break;
        case 0xf:
            controls->RegDst = 0;  
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 6;  
            break;
        case 0xa:
            controls->RegDst = 0;  
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 2;  

            break;
        case 0xb:
            controls->RegDst = 0;  
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 3;

            break;
        default:
            return 1; 
    }

    return 0; 
}


/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{

    *data1 = Reg[r1];  
    *data2 = Reg[r2];  
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{

    if (offset & 0x8000) { 
        *extended_value = offset | 0xFFFF0000;
    } else {
        *extended_value = offset & 0x0000FFFF;  
    }
}


/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    unsigned B;
    unsigned a =ALUOp;


 if(a == 7){
    switch(funct){
      // Add
      case 0x20:
        a = 0;
        break;
      // Subtract
      case 0x22:
        a = 1;
        break;
      case 0x2a:
        a = 2;
        break;
      case 0x2b:
        a = 3;
        break;
      // And
      case 0x24:
        a = 4;
        break;
      // Or
      case 0x25:
        a = 5;
        break;
      default:
        return 1;
    }
  }

    B = (ALUSrc == 1) ? extended_value : data2;
    ALU(data1, B, a, ALUresult, Zero);

    return 0; 
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  if(MemWrite == 1){
    if(ALUresult % 4 == 0)
      Mem[ALUresult >> 2] = data2;
    else
      return 1;
  }
  if(MemRead == 1){
    if(ALUresult % 4 == 0)
      *memdata = Mem[ALUresult >> 2];
    else
      return 1;
  }

  return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    if (RegWrite == 0) {
        return;
    }
    unsigned reg_index = (RegDst == 1) ? r3 : r2;
    unsigned data_to_write = (MemtoReg == 1) ? memdata : ALUresult;
    Reg[reg_index] = data_to_write;
}


/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC = *PC + 4;
    if (Jump == 1) {
        *PC = (*PC & 0xF0000000) | (jsec << 2);  
    }  
    if (Branch == 1 && Zero == 1) {
        *PC = *PC  + (extended_value << 2); 
    } 
}
