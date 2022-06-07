#include <iostream>
using namespace std;

#include "Max_Min.h"

int main()
{
	int nMax = 0;
	int nMin = 0;

	nMax = Max(10, 19);
	nMin = Min(2, 43);

	cout<<"The max number is "<<nMax<<endl;
	cout<<"The min number is "<<nMin<<endl;

	return 0;
}
