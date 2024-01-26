#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "lib/Vec2.h"


typedef unsigned long long ull;
typedef Vec2T<ull> Vec2ull;

ull F_common(const unsigned N);
ull F_coprime(const unsigned N);

unsigned gcd(unsigned a, unsigned b)
{
	while (b)
	{
		unsigned tmp = b;
		b = a%b;
		a = tmp;
	}
	return a;
}


int main(int argc, char* argv[])
{
	ull result = 0;
	for (unsigned N = 1; N <= 19; N++)
	{
		if (gcd(N,10) == 1)
			result += F_coprime(N);
		else
			result += F_common(N);
	}

	std::cout << result << std::endl;

	return 0;
}


ull F_common(const unsigned N)
{
	const unsigned G = gcd(N, 10);
	const unsigned K = N/G;
	unsigned pow3K = 1;
	for (unsigned i = 0; i < K; i++) pow3K *= 3;
 
	std::vector<Vec2ull> counts_src(pow3K);
	std::vector<Vec2ull> counts_dest(pow3K);
 
	counts_src[0][0] = 1;
	for (unsigned len = 0; len < N; len++)
	{
		std::vector<unsigned> digits(K, 0);		// ternary representation of m
		digits[0]--;
		for (unsigned m = 0; m < pow3K; m++)
		{
			// counting in base 3 (instead of decomposing m)
			unsigned tmp = 0;
			while (digits[tmp] == 2) digits[tmp++] = 0;
			digits[tmp]++;
 
			Vec2ull counts = counts_src[m];
			counts_src[m].reset();
			if (!(counts[0]|counts[1])) continue;
 
			// suffix from 1 if length=0, from 0 otherwise
			for (unsigned suffix = !len; suffix < 10; suffix++)
			{
				unsigned g = gcd(G, suffix);
				// we have a piece of a one-child number if the current suffix is 0 (or a multiple of N) 
				// XOR if we have seen exactly one longer substring that is 0 mod N with this suffix
				// if both conditions apply or we have more than one such substring, it is a bad suffix.
				if (g==G && (suffix%N == 0) + digits[((N - (suffix%N))%N) / G] > 1) continue;

				// build the ternary encoding for adding this suffix and appending the future 0
				std::vector<unsigned> new_digits(K, 0);
				new_digits[((suffix*10)%N) / G]++;
				for (unsigned i = 0; i < K; i++)
				{
					unsigned& dest = new_digits[(((i*G+suffix)*10)%N) / G];
					dest += digits[i];
					dest = std::min(dest, 2u);
				}
 
				// assemble the individual digits to an unsigned
				unsigned m_new = 0;
				for (unsigned i = 0, pow3 = 1; i < K; i++, pow3 *= 3)
				{
					m_new += pow3 * new_digits[i];
				}
 
				assert(m_new < pow3K);
				if (g==G && (suffix%N == 0) + digits[((N - (suffix%N))%N) / G] == 1)
				{
					// we have produced a substring with remainder 0 (cf. comment above):
					// the strings with no previous 0-remainder substring (counts[0]) are now potential
					// solutions;
					// if such a substring was found, it can't be a solution, so counts[1] is discarded
					assert(new_digits[0] >= 1);
					counts_dest[m_new][1] += counts[0];
				}
				else
				{
					// strings without/with previous 0-remainder substrings remain that way
					counts_dest[m_new] += counts;
				}
			}
		}
		std::swap(counts_src, counts_dest);
	}
 
	ull result = 0;
	for (unsigned m = 0; m < pow3K; m++)
		result += counts_src[m][1];
	return result;
}


ull F_coprime(const unsigned N)
{
	const unsigned pow2N = 1<<N;

	std::vector<Vec2ull> counts_src(pow2N);
	std::vector<Vec2ull> counts_dest(pow2N);

	counts_src[0][0] = 1;
	for (unsigned len = 0; len < N; len++)
	{
		for (unsigned m = 0; m < pow2N; m++)
		{
			Vec2ull counts = counts_src[m];
			counts_src[m].reset();
			if (!(counts[0]|counts[1])) continue;

			// generate the bit mask of remainders for n*10 (without the suffix added)
			// this shuffles the bits of m
			unsigned m_new = 0;
			unsigned collisions = 0;
			for (unsigned i = 0, mm = m; mm; i++, mm >>= 1)
			{
				unsigned tmp = (mm&1) << ((i*10)%N);
				collisions |= m_new & tmp;
				m_new |= tmp;
			}
			assert(!collisions);
			// suffix from 1 if length=0, from 0 otherwise
			m_new <<= !len;		// for length=0, we start at 1, so increment all remainders
			for (unsigned suffix = !len; suffix < 10; suffix++)
			{
				// remainder of the new substring of length 1
				unsigned tmp = 1 << (suffix%N);
				// clean up the bits in m_new so they are in the N least significant positions
				m_new = (m_new & (pow2N-1)) | (m_new >> N);
				collisions = m_new & tmp;
				// set tmp to the bit mask of remainders of all substrings ending here
				tmp |= m_new;
				assert(collisions == (collisions & -collisions));	// at most one colliding bit
				if (collisions > 1)
				{
					// we have a collision in a bit with nonzero index, i.e.
					// at least two substrings ending here have the same nonzero remainder;
					// the difference therefore has remainder 0
					assert(!counts[0]);		// no strings without a previous remainder 0
					// this is ok, unless another substring with remainder 0 ends here
					//if (!(tmp&1)) counts_dest[tmp][1] += counts[1];
					// unconditional equivalent:
					counts_dest[tmp][1] += counts[!(tmp&1)];
				}
				else if (!collisions)
				{
					// did we generate a remainder 0 here?
					if (tmp&1) counts_dest[tmp][1] += counts[0];
					else counts_dest[tmp] += counts;
				}
				// increment all remainders for the next iteration
				m_new <<= 1;
			}
		}
		std::swap(counts_src, counts_dest);
	}

	ull result = 0;
	for (unsigned m = 0; m < pow2N; m++)
		result += counts_src[m][1];
	return result;
}