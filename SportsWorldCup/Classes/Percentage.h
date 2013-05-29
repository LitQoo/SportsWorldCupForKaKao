//
//  Percentage.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 8..
//
//

#ifndef __SportsWorldCup__Percentage__
#define __SportsWorldCup__Percentage__

#include <cstdlib>
#include <vector>

class Percentage
{
private:
	int r2;
	int lastp;
	std::vector<int> ps;
public:
	Percentage()
	{
		lastp = 0;
		r2 = rand()%100 + 1; // [1, 100];
	}
	void seed();
	void add(int p);
	bool prob(int index);
};

#endif /* defined(__SportsWorldCup__Percentage__) */
