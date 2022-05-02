/**
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int);
void initalize(stateType *);
void run(stateType *, unsigned int, int, int, int, int, int);
void add(int, int, int, int, stateType*);
void nor(int, int, int, int, stateType*);
void lw(int, int, int, int, int, stateType*);
void sw(int, int, int, int, int, stateType*);
void beq(int, int, int, int, int, stateType*);
void jalr(int, int, int, int, int, stateType*);
void noop();
void halt(stateType*);
int runs = 0;


int
main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    unsigned int machineCode = 0;
    int opcode = 0, reg0 = 0, reg1 = 0, reg2 = 0, address = 0;
    state.pc = 0;
    initalize(&state);

    run(&state, machineCode, opcode, reg0, reg1, reg2, address);

    return(0);
}

void initalize(stateType* state)
{
    for(int i = 0; i < NUMREGS; i++)
    {
        state->reg[i] = 0;
    }
}

void run(stateType* state, unsigned int machineCode, int opcode, int reg0, int reg1, int dest, int address)
{
    while(true)
    {
        printState(state);
        machineCode = state->mem[state->pc];
        opcode = machineCode >> 22;

        if(opcode == 0b000)
        {
            add(machineCode, reg0, reg1, dest, state);
        }

        else if(opcode == 0b001)
        {
            nor(machineCode, reg0, reg1, dest, state);
        }

        else if(opcode == 0b010)
        {
            lw(machineCode, reg0, reg1, dest, address, state);
        }

        else if(opcode == 0b011)
        {
            sw(machineCode, reg0, reg1, dest, address, state);
        }

        else if(opcode == 0b100)
        {
            beq(machineCode, reg0, reg1, dest, address, state);
        }

        else if(opcode == 0b101)
        {
            jalr(machineCode, reg0, reg1, dest, address, state);
        }

        else if(opcode == 0b110)
        {
            halt(state);
            break;
        }

        else if(opcode == 0b0111)
        {
            noop();
        }
        else
        {
            exit(1);
        }
        state->pc++;
        runs++;

    }
}

void add(int machineCode, int reg0, int reg1, int dest, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;
    dest = (machineCode << 23) >> 23;

    state->reg[dest] = state->reg[reg0] + state->reg[reg1];   
}

void nor(int machineCode, int reg0, int reg1, int dest, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;
    dest = (machineCode << 23) >> 23;

    state->reg[dest] = ~state->reg[reg0] | ~state->reg[reg1];   
}

void lw(int machineCode, int reg0, int reg1, int dest, int address, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;
    dest = convertNum((machineCode << 16) >> 16);

    address = state->reg[reg0] + dest;
    state->reg[reg1] = state->mem[address];
}

void sw(int machineCode, int reg0, int reg1, int dest, int address, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;
    dest = convertNum((machineCode << 16) >> 16);
    
    address = state->reg[reg0] + dest;
    state->reg[address] = state->mem[reg1];
}

void beq(int machineCode, int reg0, int reg1, int dest, int address, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;

    if(state->reg[reg0] == state->reg[reg1])
    {
        int destination = ((machineCode << 16) >> 16);

        state->pc = state->pc + destination;
    }
}

void jalr(int machineCode, int reg0, int reg1, int dest, int address, stateType* state)
{
    reg0 = (machineCode << 10) >> 29;
    reg1 = (machineCode << 13) >> 29;

    state->reg[reg1] = state->pc + 1;
    state->pc = state->reg[reg0] - 1;
}

void halt(stateType* state)
{
    state->pc++;
    runs++;
    printf("machine halted\n");
    printf("total of %d instructions executed\n", runs);
    printf("final state of machine:\n");
    printState(state);    
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

void noop()
{
    //because of compile error
}

int
convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

