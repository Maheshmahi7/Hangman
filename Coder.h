#pragma once
#include<string>
#include<iostream>
#include "Logger4CPlus.h"
#include "Property.h"

using namespace std;

class Coder
{
public:
	string encoder(string Word);
	string decoder(string Word);
};

