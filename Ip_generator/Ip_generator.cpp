#include <iostream>
#include <time.h>
#include <math.h>
#include <string>

#include "Ip_generator.h"

using namespace std;

#define SIZE 80

Ip_generator::Ip_generator() {
    /* initialize random seed: */
    srand( time(NULL) );
    
    rand_ip = new char[SIZE];
}

Ip_generator::~Ip_generator() {
    delete [] rand_ip;
}

char * Ip_generator::IP_random() {
    
    int lowest=1, highest=255;
    int range=(highest-lowest)+1;
    
    ip_rand1 = lowest+int((double)range*rand()/(RAND_MAX + 1.0));
    sprintf(buf1, "%d", ip_rand1);
    
    ip_rand2 = lowest+int((double)range*rand()/(RAND_MAX + 1.0));
    sprintf(buf2, "%d", ip_rand2);
    
    ip_rand3 = lowest+int((double)range*rand()/(RAND_MAX + 1.0));
    sprintf(buf3, "%d", ip_rand3);
    
    ip_rand4 = lowest+int((double)range*rand()/(RAND_MAX + 1.0));
    sprintf(buf4, "%d", ip_rand4);
    
    ip_adr1 = buf1;
    ip_adr2 = buf2;
    ip_adr3 = buf3;
    ip_adr4 = buf4;
    
    ip_adr = ip_adr1 + "." + ip_adr2 + "." + ip_adr3 + "." + ip_adr4;
    
    memset(rand_ip, 0x0, SIZE);
    
    strcpy(rand_ip, buf1);
    strcat(rand_ip, ".");
    strcat(rand_ip, buf2);
    strcat(rand_ip, ".");
    strcat(rand_ip, buf3);
    strcat(rand_ip, ".");
    strcat(rand_ip, buf4);
    
    return rand_ip;
}
