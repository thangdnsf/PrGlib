#include "PrGlib.h"

PrGlib::PrGlib(void)
{
    //ctor
    //Renew();
}
void PrGlib::PrG_renew()
{
    /*random prime with NTL/ZZ.h*/
    ZZ numbersite;
    RandomBits(numbersite,PrG_length);
    stringstream buffer;
    buffer<<numbersite;
    this->PrG_number=buffer.str();
    /****************************/
    /*check and add bit such that bit was make have to equal size of prime was defined*/
    string ha=PrG_number.get_str(2);
    int strlen=ha.length();
    for(;strlen<this->PrG_length;strlen++)ha=ha+'1';
    this->PrG_number.set_str(ha,2);
    /****************************/
    //if((this->PrG_number & 1)== 0) this->PrG_number=(this->PrG_number | 1);
    /*renew bitget,make to PrG_preprocess and PrG_index*/
    PrG_bitGet=new vector<bool>(this->PrG_sieve_size);
    PrG_preprocess();
    this->PrG_index=0;
}

long PrGlib::PrG_get_length()const
{
    return this->PrG_length;
}

ZZZ PrGlib::PrG_generate_prime()
{
    /*after preprocess, make to check prime by rabin miller test*/
    while(true)
    {
        for(;this->PrG_index<PrG_sieve_size;this->PrG_index+=2)
        {
            if(PrG_bitGet->at(this->PrG_index)==0)
            {
                ZZZ k=this->PrG_number+this->PrG_index;
                if(PrG_check_prime(k))
                {
                    this->PrG_index+=2;
                    return k;
                }
            }
        }
        PrG_renew();
    }
}
void PrGlib::PrG_preprocess()
{
	int j = 1;
	while (j<1000) {
		ZZZ p = PrG_prime_sample[j++];
		ZZZ r = this->PrG_number%p;
		for (ZZZ i = p - r; i < PrG_sieve_size; i += p)
		{
			this->PrG_bitGet->at(i.get_ui()) = true;
		}
	}
}
bool PrGlib::PrG_prime_test(const ZZZ& n,unsigned int len)
{
    /*kiem tra gia nguyen to fermat co so */
    ZZZ tmp;
    mpz_powm(tmp.get_mpz_t(), ZZZ(2).get_mpz_t(),ZZZ((n - 1)).get_mpz_t(), n.get_mpz_t());
    if (tmp != 1) return false;
    /**************************************/
    /*let k and m such that: n-1=m*2^k*/
    ZZZ m = n^1;                    //sub(m, n, 1); because n is odd
	long k;					// MakeOdd(m);
	k = 0;
	do{
		m >>= 1;				//= > m = m / 2;
		//y = m & 1;				//check m is even number
		k++;
	} while ((m & 1) == 0);
    /*check prime number with the first 20 prime numbers*/
    gmp_randclass r(gmp_randinit_default);
    ZZZ x;
	for (int i = 0; i < 20; i++) {
		x = PrG_prime_sample[i];
		if (PrG_rabin_miller_test(n, x, k, m))return false;
	}
	/*check prime number with the random 44 numbers <= sqrt(n)*/
	ZZZ qk;
	mpz_sqrt(qk.get_mpz_t(),n.get_mpz_t());
	for (int i = 0; i < len; i++)
	{
		do {
            x= r.get_z_range(qk); //RandomBnd(x, n);
		} while (x <= 73);
		if (PrG_rabin_miller_test(n, x, k, m))return false;
	}
	return true;
}

bool PrGlib::PrG_rabin_miller_test(const ZZZ& n, const ZZZ& x, long &k, const ZZZ&m)
{
	ZZZ z,y;
	//z = modulo(x, m, n);
	mpz_powm(z.get_mpz_t(),x.get_mpz_t(),m.get_mpz_t(),n.get_mpz_t());
	if (z == 1 || z == (n^1)) return 0;

	long j=0;
	do {
		y = z;
		//z=y*y%n;
		mpz_powm_ui(z.get_mpz_t(),y.get_mpz_t(),2,n.get_mpz_t());
		j++;
	} while (j < k && z != 1);
	return (z != 1 || y != (n^1))?true:false;
}

ZZZ PrGlib::PrG_generate_strong_prime(unsigned long size_strong_prime)
{
    /*STEP1: Generate two other primes: P-- and p+*/
    ZZZ p_sub_sub,p_plus;

    PrG_set_size_prime(size_strong_prime/2-10);
    PrG_renew();
    p_sub_sub=PrG_generate_prime();

    PrG_set_size_prime(size_strong_prime/2-3);
    PrG_renew();

    p_plus=PrG_generate_prime();
    /**************************************/
    /*STEP2: compute p- as the least prime. p-=a--*p--+1 for some integer a--=2,4,6,..*/
    ZZZ p_sub;
    ZZZ a=(p_sub_sub<<1);
    p_sub=a+1;
    while(!PrG_check_prime(p_sub))
    {
        p_sub+=a;
    }
    /**************************************/
    /*STEP3: Compute po=((p+)^(p- -1)- (p-)^(p+ -1))mode (p-*p+)*/
    ZZZ po,p1,n,d;
    n=p_sub*p_plus;
    d=p_sub^1;
    mpz_powm(po.get_mpz_t(),p_plus.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    d=p_plus^1;
    mpz_powm(p1.get_mpz_t(),p_sub.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());
    mpz_powm_ui(po.get_mpz_t(),ZZZ(po-p1).get_mpz_t(),1,n.get_mpz_t());
    /*************************************/
    /*STEP4: find a- to p as the least prime of the form
    p=po+a*p-*p+ */
    ZZZ p;
    if((po&1)==0)
    {
        po=po+p_sub*p_plus;
    }
    a=(p_sub<<1)*p_plus;
    p=po+a;
    while(!PrG_check_prime(p))
    {
        p+=a;
    }
    return p;
}
bool PrGlib::PrG_check_prime(const ZZZ& n)
{
    /*check with the first 400 prime numbers*/
    if((n&1)==0)return false;
    int j=0;
    while (j<400) {
        if(n%PrG_prime_sample[j++]==0)return false;
    }
    /*check prime by rabin miller test algorithm*/
    if(PrG_prime_test(n))
        return true;
    return false;
}
void PrGlib::PrG_set_size_prime(unsigned long size_prime)
{
    this->PrG_length=size_prime;
    this->PrG_sieve_size=(int)(0.693*size_prime*2);
}
PrGlib::~PrGlib(void)
{
    delete this->PrG_bitGet;
}
