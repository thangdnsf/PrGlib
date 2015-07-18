/**********************************************************************
* RSA library.
* version: 1.5
* Was used with NTL and GMP library
*
* July, 5th, 2015
*
* This lib was written by DucThang
* Contact:thangdn.tlu@outlook.com
*
* Every comment would be appreciated.
*
* If you want to use parts of any code of mine:
* let me know and
* use it!
****************************************************************
RSA thang;
    thang.createNewKey("PKey.txt","BKey.txt");
    thang.cryptRSA("thang.txt","BKey.txt","thang1.txt");
    thang.decryptRSA("thang1.txt","PKey.txt","thang2.txt");
**********************************************************************/
#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <bitset>
#include <gmpxx.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <string>
#include "PrGlib.h"

using namespace std;
#define KEY 128
typedef mpz_class ZZZ;
class RSA
{
    public:
        RSA();
        void createNewKey(char* filePrivateKey,char* filePublicKey);
        void decryptRSA (char* fileText,char* fileKey,char *fileDecrypt);
        void cryptRSA (char* fileMText,char* fileKey,char *fileCrypt);
        string convert_base64(const string &code);
        string de_convert_base64(const string &code);
        ~RSA();
};

#endif // RSA_H
