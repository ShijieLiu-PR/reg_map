#ifndef _REG_MAP_H_
#define _REG_MAP_H_

typedef unsigned char BYTE;
typedef unsigned short int WORD;

struct VarElement
{
    int reg_addr;
    int start_bit;
    int length;
    VarElement *next;
};

struct VarUnit
{
    char name[60];
    WORD value;
    int length;
    VarElement *var_element_table;
    VarUnit *next;
};

struct RegElement
{
    int start_bit;
    int length;
    VarUnit *ptr_var_unit;
    RegElement *next;
};

struct RegUnit
{
    BYTE value;
    RegElement *reg_element_table;
};

class RegMap
{
private:
    RegUnit *reg_table;
    VarUnit *var_table;
    const int start_addr;
    const int end_addr;

public:
    // constructor
    RegMap();
    RegMap(int start = 0, int end = 255);
    // destructor
    ~RegMap();

    // methods
    int SetRegVal(unsigned char addr, unsigned char value);
    unsigned char GetRegVal(unsigned char addr);
    RegElement *GetRegElementTable(int addr);
    int AddRegElement(int addr, int start, int length, VarUnit *ptr_var_unit);
    int AddVarUnit(char *var_name, WORD value = 0, int length = 8, VarElement *var_element_table = nullptr);
    VarUnit *GetVarUnit(char *name);
    int AddVarElement(char *name, int reg_addr, int start_bit = 7, int length = 1);
    int ParseLine(char* line);
    int SplitString(char *p[]);
    int ImportRegMapFile(char path[]);
    int TrimBlank(char str[], int size);
};

#endif