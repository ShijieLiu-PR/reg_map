#include <iostream>
#include <string.h>
#include "include/reg_map.h"



int main()
{
    RegMap rmp_obj(0,5);
    rmp_obj.SetRegVal(0, 10);
    rmp_obj.SetRegVal(1, 20);

    return 0;
}
