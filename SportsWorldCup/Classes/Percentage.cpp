//
//  Percentage.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 8..
//
//

#include "Percentage.h"


void Percentage::add(int p)
{	
	ps.push_back(lastp+p);
	lastp += p;
}

void Percentage::seed()
{
	r2 = rand() % 100 + 1;
}
bool Percentage::prob(int index)
{
	if(index == 0)
		return 0 <= r2 && r2 <= ps[index];
	else
		return ps[index - 1] <= r2 && r2 <= ps[index];
}