/**
 * Project 1
 * Assembler code fragment for LC-2K
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

struct Labels
{
	char labels[MAXLINELENGTH];
	int address;
};

int readAndParse(FILE*, char*, char*, char*, char*, char*);
int isNumber(char*);
int firstPass(struct Labels*, FILE*, char*, char*, char*, char*, char*, int, int);

int rInstructions(char*, int, char*, char*, char*, int);
int add(char*, char*, char*, int);
int nor(char*, char*, char*, int);

int iInstructions(char*, int, char*, char*, char*, int, struct Labels*);
int lw(char*, char*, char*, int, struct Labels*);
int sw(char*, char*, char*, int, struct Labels*);
int beq(char*, char*, char*, int, struct Labels*);

int jInstructions(char*, int, char*, int);
int jalr(int, char*, int);

int Oinstructions(char*, int);
int noop(char*, int);
int halt(char*, int);

int fill(char*, char*, char*, int, struct Labels*, int);

int finalMachineCode = 0;
int numLabels = 0;
int pcOffset = 0;

int main(int argc, char* argv[])
{
	char* inFileString, * outFileString;
	FILE* inFilePtr, * outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	int address = 0;
	int count = 0;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
			argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	struct Labels track_labels[MAXLINELENGTH];

	address = firstPass(track_labels, inFilePtr, label, opcode, arg0, arg1, arg2, count, address);

	for (int i = 0; i < numLabels; i++)
	{
		for (int j = i + 1; j < numLabels; j++)
		{
			if (strcmp(track_labels[i].labels, track_labels[j].labels) == 0)
			{
				exit(1);
			}
		}
	}

	rewind(inFilePtr);

	int instructionOpcode = 0;
	int finalResult;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		if (!strcmp(opcode, "add"))
		{
			//int argument1 = atoi(arg1);
			//int argument2 = atoi(arg2);
			finalResult = rInstructions(opcode, count, arg0, arg1, arg2, instructionOpcode);
		}

		else if (!strcmp(opcode, "nor"))
		{
			//int argument1 = atoi(arg1);
			//int argument2 = atoi(arg2);
			finalResult = rInstructions(opcode, count, arg0, arg1, arg2, instructionOpcode);

		}
		else if (!strcmp(opcode, "lw"))
		{
			int argument1 = atoi(arg1);
			//int argument2 = atoi(arg2);
			finalResult = iInstructions(opcode, count, arg0, arg1, arg2, instructionOpcode, track_labels);

		}

		else if (!strcmp(opcode, "sw"))
		{
			int argument1 = atoi(arg1);
			//int argument2 = atoi(arg2);
			finalResult = iInstructions(opcode, count, arg0, arg1, arg2, instructionOpcode, track_labels);
		}

		else if (!strcmp(opcode, "beq"))
		{
			finalResult = iInstructions(opcode, count, arg0, arg1, arg2, instructionOpcode, track_labels);
		}

		else if (!strcmp(opcode, "jalr"))
		{
			finalResult = jInstructions(opcode, atoi(arg0), arg1, instructionOpcode);
		}

		else if (!(strcmp(opcode, "halt")))
		{
			finalResult = Oinstructions(opcode, instructionOpcode);
		}

		else if (!strcmp(opcode, "noop"))
		{
			finalResult = Oinstructions(opcode, instructionOpcode);
		}

		else if (!strcmp(opcode, ".fill"))
		{
			finalResult = fill(arg0, (arg1), (arg2), numLabels, track_labels, instructionOpcode);
		}

		else
		{
			exit(1);
		}

		pcOffset++;
		fprintf(outFilePtr, "%d\n", finalResult);

	}

	return(0);
}

int firstPass(struct Labels* track_labels, FILE* inFilePtr, char* label, char* opcode, char* arg0, char* arg1, char* arg2, int count, int address)
{

	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		if (strlen(label) != 0)
		{
			strcpy(track_labels[numLabels].labels, label);
			track_labels[numLabels].address = address;
			numLabels++;
		}

		address++;
	}

	return address;
}

int rInstructions(char* opcode, int adderess, char* register0, char* register1, char* register2, int instructionOpcode)
{
	if (strcmp(opcode, "add") == 0)
	{
		instructionOpcode = 0b000;
		finalMachineCode = add(register0, register1, register2, instructionOpcode);
	}

	if (strcmp(opcode, "nor") == 0)
	{
		instructionOpcode = 0b001;
		finalMachineCode = nor(register0, register1, register2, instructionOpcode);
	}

	return finalMachineCode;
}

int add(char* register0, char* register1, char* register2, int instructionOpcode)
{
	int reg0 = atoi(register0);
	int reg1 = atoi(register1);
	int reg2 = atoi(register2);

	instructionOpcode = ((instructionOpcode << 3) | reg0);
	instructionOpcode = ((instructionOpcode << 3) | reg1);
	instructionOpcode = instructionOpcode << 13;
	instructionOpcode = ((instructionOpcode << 3) | reg2);

	return instructionOpcode;
}

int nor(char* register0, char* register1, char* register2, int instructionOpcode)
{
	int reg0 = atoi(register0);
	int reg1 = atoi(register1);
	int reg2 = atoi(register2);

	instructionOpcode = instructionOpcode << 3;
	instructionOpcode += reg0;
	instructionOpcode = instructionOpcode << 3;
	instructionOpcode += reg1;
	instructionOpcode = instructionOpcode << 16;
	instructionOpcode += reg2;

	return instructionOpcode;
}

int iInstructions(char* opcode, int adderess, char* register0, char* register1, char* register2, int instructionOpcode, struct Labels* track_labels)
{
	if (strcmp(opcode, "sw") == 0)
	{
		instructionOpcode = 0b011;
		finalMachineCode = sw(register0, register1, register2, instructionOpcode, track_labels);
	}

	if (strcmp(opcode, "lw") == 0)
	{
		instructionOpcode = 0b010;
		finalMachineCode = lw(register0, register1, register2, instructionOpcode, track_labels);
	}

	if (strcmp(opcode, "beq") == 0)
	{
		instructionOpcode = 0b100;
		finalMachineCode = beq(register0, register1, register2, instructionOpcode, track_labels);
	}


	return finalMachineCode;
}

int lw(char* register0, char* register1, char* register2, int instructionOpcode, struct Labels* track_labels)
{
	int labelAddress = -1;
	int reg0 = atoi(register0);

	int reg2 = atoi(register2);

	instructionOpcode = (instructionOpcode << 3) | (reg0);
	instructionOpcode = (instructionOpcode << 3) | atoi((register1));

	if (register2[0] > 57)
	{
		if (*register2 <= -32769 || *register2 >= 32768)
		{
			exit(1);
		}

		for (int i = 0; i < numLabels; i++)
		{
			if (!strcmp(register2, track_labels[i].labels))
			{
				labelAddress = track_labels[i].address;
				break;
			}

		}

		if (labelAddress > -1)
		{
			instructionOpcode = (instructionOpcode << 16) | labelAddress;
		}
	}

	else
	{
		if (reg2 > 0)
		{
			instructionOpcode = ((instructionOpcode << 16) | reg2);
		}

		else
		{
			//This is probably wrong
			int toBeCombined = reg2;
			instructionOpcode = ((instructionOpcode << 16) | toBeCombined);
		}
	}

	return instructionOpcode;
}

int sw(char* register0, char* register1, char* register2, int instructionOpcode, struct Labels* track_labels)
{
	int labelAddress = -1;
	int reg0 = atoi(register0);

	int reg2 = atoi(register2);

	instructionOpcode = (instructionOpcode << 3) | (reg0);
	instructionOpcode = (instructionOpcode << 3) | atoi((register1));

	if (isNumber(register0))
	{
		if (*register2 <= -32769 || *register2 >= 32768)
		{
			exit(1);
		}

		for (int i = 0; i < numLabels; i++)
		{
			if (!strcmp(register2, track_labels[i].labels))
			{
				labelAddress = track_labels[i].address;
				break;
			}

		}

		if (labelAddress == -1)
		{
			exit(1);
		}

		if (labelAddress > -1)
		{
			instructionOpcode = (instructionOpcode << 16) | labelAddress;
		}
	}

	else
	{
		if (reg2 > 0)
		{
			instructionOpcode = ((instructionOpcode << 16) | reg2);
		}

		else
		{
			//This is probably wrong
			int toBeCombined = reg2;
			instructionOpcode = ((instructionOpcode << 16) | toBeCombined);
		}
	}

	return instructionOpcode;
}

int beq(char* register0, char* register1, char* register2, int instructionOpcode, struct Labels* track_labels)
{
	int labelAddress = -1;
	int offset;
	int reg0 = atoi(register0);

	int reg2 = atoi(register2);

	instructionOpcode = (instructionOpcode << 3) | (reg0);
	instructionOpcode = (instructionOpcode << 3) | atoi((register1));

	if (isNumber(register0))
	{
		if (*register2 <= -32769 || *register2 >= 32768)
		{
			exit(1);
		}

		for (int i = 0; i < numLabels; i++)
		{
			if (!strcmp(register2, track_labels[i].labels))
			{
				labelAddress = track_labels[i].address;
				break;
			}

		}

		if (labelAddress > -1)
		{
			offset = labelAddress - pcOffset - 1;

			if (offset > 0)
			{
				instructionOpcode = ((instructionOpcode << 16) | offset);
			}

			else
			{
				unsigned int temp = labelAddress - pcOffset - 1;
				temp = temp << 16;
				temp = temp >> 16;
				instructionOpcode = ((instructionOpcode << 16) | temp);
			}
		}

		else
		{
			instructionOpcode = ((instructionOpcode << 16) | reg2);
		}
	}
	else
	{
		int twosComp = atoi(register2);
		instructionOpcode = ((instructionOpcode << 16) | twosComp);
	}

	return instructionOpcode;
}

int jInstructions(char* opcode, int register1, char* register2, int instructionOpcode)
{
	if (strcmp(opcode, "jalr") == 0)
	{
		instructionOpcode = 0b101;
		finalMachineCode = jalr(register1, register2, instructionOpcode);
	}

	return finalMachineCode;
}

int jalr(int register1, char* register2, int instructionOpcode)
{
	int reg2 = atoi(register2);

	instructionOpcode = instructionOpcode << 3;
	instructionOpcode += register1;
	instructionOpcode = instructionOpcode << 3;
	instructionOpcode += reg2;
	instructionOpcode = instructionOpcode << 16;

	return instructionOpcode;
}

int Oinstructions(char* opcode, int instructionOpcode)
{
	if (strcmp(opcode, "halt") == 0)
	{
		instructionOpcode = 0b110;
		finalMachineCode = halt(opcode, instructionOpcode);
	}

	if (strcmp(opcode, "noop") == 0)
	{
		instructionOpcode = 0b111;
		finalMachineCode = noop(opcode, instructionOpcode);
	}

	return finalMachineCode;
}

int halt(char* opcode, int instructionOpcode)
{
	instructionOpcode = instructionOpcode << 22;

	return instructionOpcode;
}

int noop(char* opcode, int instructionOpcode)
{
	instructionOpcode = instructionOpcode << 22;

	return instructionOpcode;
}

int fill(char* register0, char* register1, char* register2, int numLabel, struct Labels* track_labels, int instructionsOpcode)
{
	int labelAddress = -1;

	if (isNumber(register0))
	{
		int reg0 = atoi(register0);

		if (reg0 <= -32769 || reg0 >= 32768)
		{
			exit(1);
		}
	}

	else
	{
		for (int i = 0; i < numLabels; i++)
		{
			if (!strcmp(track_labels[i].labels, register0))
			{
				labelAddress = track_labels[i].address;
				break;
			}
		}

		if (labelAddress == -1)
		{
			exit(1);
		}

		if (labelAddress > -1)
		{
			instructionsOpcode = labelAddress;
		}
	}
	
	instructionsOpcode += atoi(register0);

	return instructionsOpcode;
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0,
	char* arg1, char* arg2)
{
	char line[MAXLINELENGTH];

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	char* ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char* string)
{
	/* return 1 if string is a number */
	int i;
	return((sscanf(string, "%d", &i)) == 1);
}