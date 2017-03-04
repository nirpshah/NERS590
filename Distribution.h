#ifndef _DISTRIBUTION_HEADER_
#define _DISTRIBUTION_HEADER_

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <cassert>

#include "Random.h"
#include "Point.h"

template <class T>
class distribution {
  private:

  protected:

  public:
     distribution() {};         // constructor
    ~distribution() {};   		// destructor
    virtual T sample() = 0;  	// dummy function that must be implemented in each case
};


class uniform_distribution : public distribution<double> {
private:
    double a, b;
public:
    uniform_distribution( double p1, double p2 ) : a(p1), b(p2) {};
    ~uniform_distribution() {};
    double sample();
};

class isotropicDirection_distribution : public distribution<point> {
private:
    const double twopi = 2.0 * std::acos(-1.0);
	double a, b;
public:
    isotropicDirection_distribution(double mu_min, double mu_max) : a(mu_min), b(mu_max){};
    ~isotropicDirection_distribution() {};
    point sample();
};

class uniformXpoint_distribution : public distribution<point> {
private:
	double a, b;
	point p;
public:
	uniformXpoint_distribution(double x_min, double x_max) : a(x_min), b(x_max) {}
	~uniformXpoint_distribution() {};
	point sample();
};

class exponential_distribution : public distribution<double> {
private:
    double a;   // see constructor: this is 1/lambda to save time with division
public:
    exponential_distribution( double lambda ) { a = 1.0 / lambda; };
    ~exponential_distribution() {};
    double sample();
};

class henyey_greenstein_distribution : public distribution<double> {
private:
	double g, A, B;
public:
	henyey_greenstein_distribution( double g_in ) : g(g_in) {};
    ~henyey_greenstein_distribution() {};
    double sample();
};

class floor_distribution : public distribution<int> {
private:
	double nu_bar;
public:
	floor_distribution(double nu_in) : nu_bar(nu_in) {};
	~floor_distribution(){};
	int sample();
};

class deltapoint_distribution : public distribution<point> {
private:
	// a is the position of the dirac delta pulse i.e. del(x-a)
	point p;
public:
	deltapoint_distribution(double x_in, double y_in, double z_in){
		p.x = x_in;
		p.y = y_in;
		p.z = z_in;
	};
	~deltapoint_distribution(){};
	point sample();
};

class terrell_distribution : public distribution<int> {
private:
	double nu_bar, gamma, b;
public:
	terrell_distribution(double nu_in, double g_in, double b_in) : nu_bar(nu_in), gamma(g_in), b(b_in) {};
	~terrell_distribution(){};
	int sample();
};

class linearDirection_distribution : public distribution<point> {
private:
	double a, b;
	double slope;
public:
	linearDirection_distribution(double a_in, double b_in) : a(a_in), b(b_in){
		slope = 2.0/(b*b - a*a);
	};
	~linearDirection_distribution(){};
	point sample();
};
 
#endif
