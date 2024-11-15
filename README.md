# CDA-project

void ALU() {

}

int instruction_fetch(int pc, int *mem, int *instruction) {

    //instruction addressed from mem sent to 'instruction'
    int i = pc >> 2;
    if (pc % 4 != 0)
        //the halt as specified
        return 1;
    
    //else case if halt condition does not occur
    else
        *instruction = mem[i];
    return 0;

}

void instruction_partition(int *op, int *r1, int *r2, int *r3, int *funct, int *offset, int *jsec) {
    
    
}

int instruction_decode() {
    //either switch case or if else
}

void read_register(int r1, int r2, int *Reg, int *data1, int *data2) {
    
    //values read from register and then inputted into data1 and 2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

void sign_extend(unsigned offset, unsigned *extended_value) {
    int values = 0xffff << 16;
    *extended_value = (offset & 1 << 15 ? offset | values : offset);
}

int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult) {

    //halt condition
    if (ALUSrc != 1) {
       return 0;
   }
    //if halt happens
     else if (ALUSrc == 1) {
        data2 = extended_value;
    }
    
    //output results
    else {
        ALU(data1, data2, ALUOp, ALUresult);
        
    }
    
}

int rw_memory (int ALUresult, int data2, char MemWrite, char memread, int *memdata, int *Mem) {

    if (MemWrite || memread == 1) {
        if ((ALUresult % 4) == 0)
            Mem[ALUresult >> 2] = data2;
        
        else
            //halt
            return 1;
    }
    //other halt condition
    return 0;
}

void write_register(int r2, int r3, int memdata, int ALUresult, int *Reg) {

}

void pc_update() {

}
