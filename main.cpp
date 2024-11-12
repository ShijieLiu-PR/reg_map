#include <iostream>
#include <string.h>
#include "include/reg_map.h"



int main()
{
    RegMap rmp_obj(0,5);
    rmp_obj.SetRegVal(0, 10);
    rmp_obj.SetRegVal(1, 20);

    char path[] = "Register Map.csv";
    char line[] = "0x03,dout_TADC,dout_TADC[7],dout_TADC[6],dout_TADC[5],dout_TADC[4],dout_TADC[3],dout_TADC[2],dout_TADC[1],dout_TADC[0],0x00";
    rmp_obj.ImportRegMapFile(path);
    rmp_obj.ParseLine(line);
    return 0;
}
