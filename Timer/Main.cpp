// 
// File:   Main.cc
// Author: root
//
// Created on March 28, 2008, 9:16 PM
//

#include <stdlib.h>
#include <stdio.h>

#include "Timer.h"
//
// 
//
int main(int argc, char** argv) {
    
    Timer * t = new Timer();
    
    t->Start_Timer();
    
    for (int i = 0; i < 100000; i++)
    {
        //printf("Timer Test!\n");
    }
    
    t->End_Timer();
    t->CalculateElapseTime();
            
    t->print();
    
    return (EXIT_SUCCESS);
}

