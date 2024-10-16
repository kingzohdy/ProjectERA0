#pragma once


class CEndecrypt
{
public:
    CEndecrypt()
    {
        Reset();
    }

    void Reset()
    {
        seed = 0xDEADBEEF;
        rseed = 0xBEEFDEAD;
        sofs = 0;
    }

    void Encrypt(void *data, int bytes)
    {
        unsigned char *buf = (unsigned char *) data;
        
        while(--bytes)
        {
            *buf ^= (rand() + bseed[sofs]);
            bseed[sofs++] += *buf++;
            sofs &= 3;
        }
    }

    void Decrypt(void *data, int bytes)
    {
        unsigned char *buf = (unsigned char *) data;

        while(--bytes)
        {
            unsigned char value = *buf;
            *buf++ ^= (rand() + bseed[sofs]);
            bseed[sofs++] += value;
            sofs &= 3;
        }
    }

protected:

private:
    unsigned int rand()
    {
        return( ((rseed = rseed * 214013L 
            + 2531011L) >> 16) & 0x7fff );
    }

    int sofs;
    union {
        unsigned int seed;
        unsigned char bseed[4];
    };
    unsigned int rseed;
};
