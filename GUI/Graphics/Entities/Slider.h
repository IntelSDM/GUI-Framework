#pragma once
#include "Entity.h"
// due to using templates in c++ classes it is illegal to use a c++ file so this will look rather odd to the other code.
template <typename T>
class Slider : public Entity
{
protected:
	T MinValue;
	T MaxValue;
	T* Value;
	bool Held;
	std::string Measurement;
public:
	void Update()
	{
	
	}
	void Draw()
	{
	
	}
	Slider(std::string name, std::string measurement, T minvalue, T maxvalue, T* value) : Measurement(measurement), MaxValue(maxvalue), MinValue(minvalue), Value(value)
	{
		Name = name;

	}

};