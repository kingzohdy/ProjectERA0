
#ifndef __RandomNumber_H__
#define __RandomNumber_H__

#include "OgrePrerequisites.h"
#include "OgreVector3.h"

namespace Ogre
{
	//** Random number class and Random number pool class.  This acts as a
	//** replacement for the stdlib rand() function.  Why would you want to
	//** replace the rand() function?  Because you often want a deteriminstic
	//** random number generator that is exactly the same regardless of what
	//** machine or compiler you build your code on.  The stdlib rand() function
	//** is not guaraenteed to produce the same results on different stdlib
	//** implementations.
	//**
	//** You can also maintain any number of unique random number generators
	//** as state machines by instancing this rand class over and over again.
	//**
	//** The random number pool is a data structure which you allocate if you
	//** want to pull items from a data set, randomly, but without any
	//** duplications.  A simple example would be a deck of cards.  You would
	//** instantiate the random number pool as follows:
	//**
	//** RandPool deck(52);
	//**
	//** You would then pull cards from the deck as follows:
	//**
	//** bool shuffled;
	//** int card = deck.get(shuffled);
	//**
	//** This will return a number between 0-51 (representing a card in the deck)
	//** without ever reporting the same card twice until the deck has been
	//** exhausted.  If the boolean 'shuffled' is true, then the deck was
	//** re-shuffled on that call.  This data structure has lots of uses in
	//** computer games where you want to randomly select data from a fixed
	//** pool size.
	//**
	//** This code submitted to FlipCode.com on July 23, 2000 by John W. Ratcliff
	//** It is released into the public domain on the same date.

	class _OgreExport RandomGenerator
	{
	public:
		RandomGenerator( int seed=0 ) : m_CurVal(seed)
		{
		};

		void reset( int seed )
		{
			m_CurVal = seed;
		};

		void reset();

		// random number between 0 - 32767
		int get()
		{
			return (((m_CurVal = m_CurVal * 214013L + 2531011L) >> 16) & 0x7fff);
		};

		// random number between 0.0 and 1.0
		float getFloat()
		{
			return float(get())*(1.0f/32767.0f);
		};

		int get( int minvalue, int maxvalue )
		{
			return (get()%(maxvalue-minvalue+1)) + minvalue;
		}

		float getFloat( float minvalue, float maxvalue )
		{
			return getFloat()*(maxvalue-minvalue) + minvalue;
		}

		Vector3 getDirection();

	private:
		int m_CurVal;
	};

	class _OgreExport RandomPool
	{
	public:
		RandomPool(int size,int seed)  // size of random number bool.
		{
			mRand.reset(seed);       // init random number generator.
			mData = new int[size]; // allocate memory for random number bool.
			mSize = size;
			mTop  = mSize;
			for (int i=0; i<mSize; i++) mData[i] = i;
		}

		~RandomPool(void)
		{
			delete[] (mData);
		};

		// pull a number from the random number pool, will never return the
		// same number twice until the 'deck' (pool) has been exhausted.
		// Will set the shuffled flag to true if the deck/pool was exhausted
		// on this call.
		int get(bool &shuffled)
		{
			if ( mTop == 0 ) // deck exhausted, shuffle deck.
			{
				shuffled = true;
				mTop = mSize;
			}
			else
				shuffled = false;
			int entry = mRand.get()%mTop;
			mTop--;
			int ret      = mData[entry]; // swap top of pool with entry
			mData[entry] = mData[mTop];  // returned
			mData[mTop]  = ret;
			return ret;
		};

	private:
		RandomGenerator mRand;  // random number generator.
		int  *mData;  // random number bool.
		int   mSize;  // size of random number pool.
		int   mTop;   // current top of the random number pool.
	};


	// ** This class is a Gaussian Random Number Generator
	// ** A Gaussian number is expressed as having a mean and standard
	// ** deviation and generates random numbers which conform to the standard
	// ** bell curve distribution.  Additionally this class applies an optional
	// ** minimum and maximum clamping range for all numbers generated.
	// ** The Guassian class inherits the deterministic random number class.
	// ** Therefore all gaussian sequences can be deterministic given the same
	// ** initial random number seed.
	// **
	// ** The ASCII format for a gaussian number is as follows:
	// **
	// ** mean:std<min:max>
	// **
	// ** Examples of valid gaussian numbers.
	// **
	// **  30          (Means 30 with a standard deviation of 0)
	// **  30:5        (Means 30 with a standard deviation of +/- 5)
	// **  30:5<10>    (Means 30, stdev +/-5 minimum value of 10
	// **  30:5<10:40> (Means 30, stdev +/-5, min value 10 max value 40)

	// Implementation of gaussian numbers.
	class _OgreExport GaussGenerator : public RandomGenerator
	{
	public:
		GaussGenerator(int seed = 0);
		GaussGenerator(const char *str,int seed=0);
		~GaussGenerator(void);

		float get(void); // generate *deterministic* gaussian number.
		float getCurrent(void) const; // last generated value.
		float getMean(void) const;  // return mean.
		float getStandardDeviation(void) const; // return standard deviation.
		void getString(char *str) const; // get string representation


		void set(float m,float s); // set mean and standard deviation.
		void set(float m,float s,float min,float max); // set mean, standard deviation, min and max values.
		const char * set(const char *arg); // set from asciiz string, returns pointer past gauss number.

		void setMean(float m); // set just the mean.
		void setStandardDeviation(float s); // set just the standard deviation.

		float randGauss( void ); // construct and return gaussian number.

		// convert string to gaussian number.  Return code
		// indicates number of arguments found.
		int strtogmd(const char* spec,
			char** end,
			float& mean,
			float& deviation,
			float& min,
			float& max ) const;

		float getMin(void) const { return mMin; };
		float getMax(void) const { return mMax; };

	private:
		float mMean;               // gaussian number has mean and
		float mStandardDeviation;  // standard deviation, also
		float mMin;                // min/max clamping values
		float mMax;

		float mCurrent;           // last got value.
		float mGauss1;            // 1st gaussian
		float mGauss2;            // 2nd gaussian
		bool  mSecond;            // have a computed second available.
	};

	class _OgreExport PerlinNoise2D
	{
	public:
		/**
		* \param w0,h0  level-0的长宽, 每一层比前面一层的长宽加倍
		* \param nlevel 多少层noise的叠加，
		*/
		PerlinNoise2D( int w0, int h0 );
		~PerlinNoise2D();

		//生成各个level的noise
		void initNoise( int seed );
		void initNoise( int seed1, int seed2, float t );//interpolate noises generated from seed1 to seed2 with t

		/**合成各个level的noise，生成最终结果
		* \param pbuf 接收最终结果， 长宽为 (w0<<(nlevel-1)), (h0<<(nlevel-1))
		* \param nlevel 使用前面的nlevel的noise合成结果
		*/
		void calNoiseData( unsigned char *pbuf, int nlevel );
		void calNoiseDataRow( unsigned char *pbuf, int nlevel, int row ); //计算第几行的noise

		void calNoiseData( float *pbuf, int nlevel );

		static void makeNoiseSharp( unsigned char *pbuf, int width, int height, int cloudcover,	float cloudsharpness );
		static void makeNoiseSharpRow( unsigned char *pbuf, int width, int row, int cloudcover, float cloudsharpness );

	private:
		int m_Width0, m_Height0;
		unsigned char *m_ppNoise[2]; //每一level的noise
		RandomGenerator m_RandGen;

		unsigned char SampleNoise( int ilevel, int x, int y, int destw, int desth );
	};
}

#endif