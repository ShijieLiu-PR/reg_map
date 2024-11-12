#include<string.h>
#include<fstream>
#include<iostream>
#include "../include/reg_map.h"

#define LINE_MAX_SIZE 1000



//constructor
RegMap::RegMap() : start_addr(0), end_addr(255) 
{
    reg_table = new RegUnit[256];
    for (int i = 0; i <= end_addr - start_addr;i++)
    {
        (reg_table + i)->value = 0;
        (reg_table + i)->reg_element_table = nullptr;
    }
        var_table = nullptr;
};

RegMap::RegMap(int start, int end) : start_addr(start), end_addr(end)
{
    reg_table = new RegUnit[end_addr - start_addr + 1];
    for (int i = 0; i <= end_addr - start_addr; i++)
    {
        (reg_table + i)->value = 0;
        (reg_table + i)->reg_element_table = nullptr;
    }
    var_table = nullptr;
};

//destructor
RegMap::~RegMap()
{
    // release reg element table one by one
    for (int i = 0; i <= end_addr - start_addr; i++)
    {
        RegElement *temp;
        while((reg_table + i)->reg_element_table)
        {
            temp = (reg_table + i)->reg_element_table;
            (reg_table + i)->reg_element_table = (reg_table + i)->reg_element_table->next;
            delete temp;
        }
    }
    //release reg table
    if (reg_table)
    {
        delete[] reg_table;
    }

    //release var table
    VarUnit *temp_var_unit;
    VarElement *temp_var_element;
    while(var_table)
    {
        temp_var_unit = var_table;
        var_table = var_table->next;
        while(temp_var_unit->var_element_table)
        {
            temp_var_element = temp_var_unit->var_element_table;
            temp_var_unit->var_element_table = (temp_var_unit->var_element_table)->next;
            if(temp_var_element){
                delete temp_var_element;
            }
        }
        if(temp_var_unit)
        {
            delete temp_var_unit;
        }
    }
    reg_table = nullptr;
    var_table = nullptr;
};

//methods
int RegMap::SetRegVal(unsigned char addr, unsigned char value)
{
    (reg_table + addr - start_addr)->value = value;
    return 0;
};

unsigned char RegMap::GetRegVal(unsigned char addr)
{
    return (reg_table + addr - start_addr)->value;
};

RegElement *RegMap::GetRegElementTable(int addr)
{
    return (reg_table + addr - start_addr)->reg_element_table;
};

int RegMap::AddRegElement(int addr, int start, int length, VarUnit *ptr_var_unit)
{
    RegElement* temp = (reg_table + addr - start_addr)->reg_element_table;
    if(!temp)
    {
        temp = new RegElement;
        temp->start_bit = start;
        temp->length = length;
        temp->next = nullptr;
        temp->ptr_var_unit = ptr_var_unit;
        (reg_table + addr - start_addr)->reg_element_table = temp;
    }
    else 
    {
        while(temp->next){
            temp = temp->next;
        }
        temp->next = new RegElement;
        temp = temp->next;
        temp->start_bit = start;
        temp->length = length;
        temp->next = nullptr;
        temp->ptr_var_unit = ptr_var_unit;
    }
    return 0;
};

int RegMap::AddVarUnit(char *var_name, WORD value, int length, VarElement *var_element_table)
{
    VarUnit *temp;
    if(!var_table)
    {
        temp = new VarUnit;
        strcpy(temp->name, var_name);
        temp->length = length;
        temp->value = value;
        temp->next = nullptr;
        temp->var_element_table = var_element_table;
        var_table = temp;
    }
    else
    {
        temp = var_table;
        while(temp->next)
        {
            temp = temp->next;
        }
        temp->next = new VarUnit;
        temp = temp->next;
        strcpy(temp->name, var_name);
        temp->value = value;
        temp->length = length;
        temp->var_element_table = var_element_table;
        temp->next = nullptr;
    }
    return 0;
};

VarUnit *RegMap::GetVarUnit(char *name)
{
    VarUnit *temp = var_table;
    while(temp)
    {
        if(strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
};

int RegMap::AddVarElement(char *name, int reg_addr, int start_bit, int length)
{
    VarUnit *ptr_var_unit = GetVarUnit(name);
    if(ptr_var_unit!=nullptr)
    {
        VarElement *temp;
        temp = ptr_var_unit->var_element_table;
        // if this is the first var element
        if(temp==nullptr)
        {
            temp = new VarElement;
            temp->reg_addr = reg_addr;
            temp->start_bit = start_bit;
            temp->length = length;
            temp->next = nullptr;
            ptr_var_unit->var_element_table = temp;
            return 0;
        }
        // not the first var element, add it by following the start_bit decreasing direction
        VarElement *prev = temp;
        while((temp->next != nullptr) && (temp->start_bit > start_bit))
        {
            prev = temp;
            temp = temp->next;
        }
        if(temp->start_bit > start_bit)
        {
            temp->next = new VarElement;
            temp = temp->next;
            temp->reg_addr = reg_addr;
            temp->start_bit = start_bit;
            temp->length = length;
            temp->next = nullptr;
            return 0;
        }
        prev->next = new VarElement;
        prev = prev->next;
        prev->reg_addr = reg_addr;
        prev->start_bit = start_bit;
        prev->length = length;
        prev->next = temp;
        return 0;
    }
    return -1;
};


int RegMap::ImportRegMapFile(char path[])
{
    std::fstream ifs;

    ifs.open(path, std::ios::in);
    if(ifs.is_open())
    {
        char *single_line = new char[LINE_MAX_SIZE + 1];

        while(!ifs.eof())
        {
            ifs.getline(single_line, LINE_MAX_SIZE);
            std::cout << single_line << std::endl;
            
        }

        delete[] single_line;
        ifs.close();
    }
    return 0;
};

int RegMap::ParseLine(char* line)
{
    int reg_addr;
    int reg_default_value;
    char* tail = line;
    char* head = line;
    return 0;
};

int RegMap::TrimBlank(char str[], int size)
{
    const char *blank_set = " \f\n\r\t\v";
    char *head = str;
    char *tail = str + size - 1;
    while(strchr(blank_set, *head) && (head<tail)){
        head++;
    }
    while(strchr(blank_set, *tail) && (tail>=head)){
        *tail = '\0';
        tail--;
    }
    for (int i = 0; i < size; i++){
        if(head<=tail){
            str[i] = *head;
            head++;
        }
        else{
            str[i] = '\0';
        }
    }
    return 0;
};