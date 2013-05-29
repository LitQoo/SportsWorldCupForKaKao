//
//  HWFromTo.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 4..
//
//

#ifndef __SportsWorldCup__HWFromTo__
#define __SportsWorldCup__HWFromTo__

#include <cmath>
class FromToWithDuration
{
private:
	float originFrom;
	float from;
	float to;
	float duration;
	float dx;
	bool positive;
public:
	FromToWithDuration(){}
	FromToWithDuration(float _from, float _to, float _duration) : originFrom(_from)
	{
		from = _from;
		to = _to;
		duration = _duration;
		dx = (to - from) / duration;
		// positive
		if(_from <= _to)
		{
			positive = true;
		}
		else
		{
			positive = false;
		}
	}
	void init(float _from, float _to, float _duration)
	{
		originFrom = _from;
		from = _from;
		to = _to;
		duration = _duration;
		if(duration != 0)
			dx = (to - from) / duration;
		// positive
		if(_from <= _to)
		{
			positive = true;
		}
		else
		{
			positive = false;
		}
	}
	bool step(float dt)
	{
		if(duration)
			from += dx * dt;
		else
			from = to;
		if(positive)
		{
			if(from >= to)
			{
				from = to;
				return false; // finish
			}
		}
		else
		{
			if(from <= to)
			{
				from = to;
				return false; // finish
			}
		}
		return true;
	}
	float getValue(){return from;}
	float getTo(){return to;}
};
class FromTo
{
private:
	float originFrom;
	float from;
	float to;
	float dx;
	bool positive;
public:
	FromTo(){}
	FromTo(float _from, float _to, float _dx) : originFrom(_from)
	{
		from = _from;
		to = _to;
		_dx = fabs(_dx);
		
		// positive
		if(_from <= _to)
		{
			dx = _dx;
			positive = true;
		}
		else
		{
			dx = -_dx;
			positive = false;
		}
		
	}
	void init(float _from, float _to, float _dx)
	{
		originFrom = _from;
		from = _from;
		to = _to;
		_dx = fabs(_dx);
		// positive
		if(_from <= _to)
		{
			dx = _dx;
			positive = true;
		}
		else
		{
			dx = -_dx;
			positive = false;
		}
		
	}
	bool step()
	{
		from += dx;
		if(positive)
		{
			if(from >= to)
			{
				from = to;
				return false; // finish
			}
		}
		else
		{
			if(from <= to)
			{
				from = to;
				return false; // finish
			}
		}
		return true;
	}
	float getValue()
	{
		return from;
	}
};

#endif /* defined(__SportsWorldCup__HWFromTo__) */
