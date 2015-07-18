#include <NTL/ZZ.h>
using namespace NTL;
#include <iostream>
#include <gmpxx.h>
#include <fstream>
#include "PrGlib.h"
#include "RSA.h"
using namespace std;
#define _TIME int starts,finishs;
#define STARTS_TIME starts=clock();
#define FINISHS_TIME finishs=clock(); cout<<"Time ~ "<<(double)(finishs-starts)/CLOCKS_PER_SEC<<endl;
_TIME
int main()
{

/*RSA*/
    /*RSA thang;
    thang.createNewKey("PKey.txt","BKey.txt");
    thang.cryptRSA("KEY128bit.txt","BKey.txt","thang1.txt");
    thang.decryptRSA("thang1.txt","PKey.txt","thang2.txt");*/
/*Generate prime*/
    /*PrGlib a;
    a.PrG_set_size_prime(3072);
    a.PrG_renew();
    while(true)
    {
    STARTS_TIME
    a.PrG_generate_strong_prime(3072);
    a.PrG_renew();
    cout<<"~~~~~~~~~~~~"<<endl;
    FINISHS_TIME
    }
    */
/*Generate strong prime*/
    PrGlib a;
    while(true)
    {
    STARTS_TIME
    a.PrG_generate_strong_prime(3072);
    cout<<"~~~~~~~~~~~~"<<endl;
    FINISHS_TIME
    }
    return 0;
}

