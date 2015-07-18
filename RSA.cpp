#include "RSA.h"

RSA::RSA()
{
    //ctor
}

void RSA::createNewKey(char *filePrivateKey,char * filePublicKey)
{
    PrGlib dnthang;
    ofstream PK(filePrivateKey);
    ofstream BK(filePublicKey);
    /*Generate p and q as strong primes */
    ZZZ p=dnthang.PrG_generate_strong_prime(3072);

    ZZZ q=dnthang.PrG_generate_strong_prime(3072);
    ZZZ n=q*p;
    ZZZ phi=(p^1)*(q^1);
    ZZZ e,k;
    /*Find e such that gcd(e,phi)=1*/
    do
    {
        gmp_randclass rr(gmp_randinit_default);
		rr.seed(time(NULL));
        e =rr.get_z_bits(dnthang.PrG_get_length());
		ZZZ num = e & 1;
		if (num == 0)e = e | 1;
        mpz_gcd (k.get_mpz_t(),e.get_mpz_t(), phi.get_mpz_t());
    }while(k!=1);
    /*******************************/
    /*Compute d= e^-1 mod n*/
    ZZZ d;
    mpz_invert(d.get_mpz_t(),e.get_mpz_t(),phi.get_mpz_t());
    /*private key*/
    PK<<n.get_str()<<endl;
    PK<<d.get_str()<<endl;
    /*public key*/
    BK<<n.get_str()<<endl;
    BK<<e.get_str()<<endl;
    PK.clear();
    PK.close();
    BK.clear();
    BK.close();
}
void RSA::decryptRSA (char* fileText,char* fileKey,char *fileDecrypt)
{
    ifstream filetext(fileText);
    ofstream decrypt(fileDecrypt);
    ifstream PK(fileKey);
     if(!PK){
        cout<<"Fail!publicKey.txt isn't exist";
        return;
    }
    if(!filetext){
        cout<<"Fail!filetext.txt isn't exist";
        return;
    }
    ZZZ n,d,code;
    string a;
    /*read key from fileKey*/
    PK>>a; n=a;
    PK>>a; d=a;
    /*read text from fileText*/
    filetext>>a;
    /*decrypto base64 to bits*/
    code.set_str(de_convert_base64(a),2);
    /*M=C^d mode n*/
    mpz_powm(code.get_mpz_t(),code.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    /*convert to bit and crop length of KEY{128,192,256}*/
    a=code.get_str(2);
    a=a.substr(2048-KEY,KEY);
    /********************/
    code.set_str(a,2);
    decrypt<<code.get_str();

    filetext.close();
    decrypt.close();
    PK.close();
}
void RSA::cryptRSA (char* fileMText,char* fileKey,char *fileCrypt)
{
    string headCode="1000010010000100";/*head16bit*/
    ifstream filetext(fileMText);
    ifstream filekey(fileKey);
    ofstream filecrypt(fileCrypt);
    if(!filetext){
        cout<<"Fail!Filetext isn't exist";
        return;
    }
    if(!filekey){
        cout<<"Fail!Filekey isn't exist";
        return;
    }
    /*read key from fileKey*/
    string TEXT;
    ZZZ n,e;
    filekey>>TEXT;
    n=TEXT;
    filekey>>TEXT;
    e=TEXT;
    /*linked head16bit with bit random + KEY{128,192,256}*/
    TEXT=headCode;
    ZZ numbersite;
    RandomBits(numbersite,(2032-KEY));
    stringstream buffer;
    buffer<<numbersite;
    ZZZ code;
    code=buffer.str();
    int strlen=code.get_str(2).length();
    for(;strlen<2032-KEY;strlen++)TEXT+='1';
    TEXT+=code.get_str(2);
    filetext>>headCode;
    code=headCode;
    TEXT+=code.get_str(2);
    /*********************/
    /*C=M^e mod n*/
    code.set_str(TEXT,2);
    mpz_powm(code.get_mpz_t(),code.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
    filecrypt<<convert_base64(code.get_str(2))<<endl;

    filecrypt.close();
    filetext.close();
    filekey.close();
}
string RSA::convert_base64(const string &code)
{
    string decode;
    ZZZ convert_number;
    for(int i=0;i<code.length();i+=6)
    {

        convert_number.set_str(code.substr(i,6),2);
        long ka=convert_number.get_ui();
        if(ka<26) ka+=65;
        else if(ka<52)ka+=71;
        else if(ka<62)ka-=4;
        else if(ka==62)ka=43;
        else if(ka==63)ka=47;
        decode+=(char)ka;
    }
    return decode;
}
string RSA::de_convert_base64(const string &code)
{
    string decode;
    ZZZ convert_number;
    long ka;
    for(int i=0;i<code.length();i++)
    {
        ka=code[i];
        if(ka<='Z'&& ka>='A') ka-=65;
        else if(ka>='a'&&ka<='z')ka-=71;
        else if(ka<='9'&&ka>='0')ka+=4;
        else if(ka=='+')ka=62;
        else if(ka=='/')ka=63;
        convert_number=ka;
        string ma=convert_number.get_str(2);
        if(i<code.length()-1)
        {
            for(int j=0;j<6-ma.length();j++)
            {
                decode+='0';
            }
        }
        decode+=ma;
    }
    return decode;
}
RSA::~RSA()
{
    //dtor
}
