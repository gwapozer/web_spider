#ifndef IP_GENERATOR_H
#define IP_GENERATOR_H

#include <string>

using namespace std;

class Ip_generator {
public:
    Ip_generator();
    ~Ip_generator();
    char *IP_random();
private:
    char *rand_ip;
    
    /* For random generator */
    int ip_rand1, ip_rand2, ip_rand3, ip_rand4;
    string ip_adr, ip_adr1, ip_adr2, ip_adr3, ip_adr4;
    char buf1[50], buf2[50], buf3[50], buf4[50];
    /* End */
    
};
#endif
