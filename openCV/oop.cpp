#include "stdafx.h"
#include <iostream>
using namespace std;

struct figure
{
	double size;
	virtual double area()=0;
	figure(double fsize=10){
	  size=fsize;
	}
};


struct circle: public figure
{
	double area()
	{
		return size*size/4*3.1415;
	}
	circle():figure(){
	
	}
	double radius(){return size/2;}
};

struct square: public figure
{
	double area()
	{
		return size*size;
	}
	square():figure(){}
};

struct triangle: public figure
{
	double area()
	{
		return size*size*0.5*0.866;
	}
	triangle():figure(){}
};

int main()
{
	figure* shapes[3] = {new circle, new square, new triangle};
	shapes[0] = new circle;
	for(int i = 0; i < 3; i++)
	{
		circle *p = dynamic_cast<circle*>(shapes[i]);
		if(p!=NULL)
			cout<<p->radius()<<' ';
		cout<<shapes[i]->area()<<' ';
	}
	getchar();
}