#include <cmath>
#include <limits>

#include "Distribution.h"
#include "Point.h"
#include "Random.h"

double uniform_distribution::sample() {
  return a + Urand() * ( b - a );
}

point isotropicDirection_distribution::sample() {
	// sample polar cosine and azimuthal angle uniformly
	double mu  = (b-a) * Urand() + a;
	double azi = twopi * Urand();

	// convert to Cartesian coordinates
	double c = std::sqrt( 1.0 - mu * mu );
	point p;
	p.x = mu;
	p.y = std::sin( azi ) * c;
	p.z = std::cos( azi ) * c;

	return p;
}

point uniformXpoint_distribution::sample(){
	double x = (b-a) * Urand() + a;
	double y = 0.0;
	double z = 0.0;
	p.x      = x;
	p.y		 = y;
	p.z 	 = z;
	return p;
}

double exponential_distribution::sample() {
  return -std::log( Urand() ) * a;
}

double henyey_greenstein_distribution::sample() {
	// The fastest way to sample the Henyey_Greenstein_Distribution is the direct method
	double s = 2.0*Urand()-1.0;
	double mu = 1/(2*g)*(1+g*g - ((1-g*g)/(1+g*s))*((1-g*g)/(1+g*s)));
	return mu;
}

int floor_distribution::sample(){
	return std::floor(nu_bar + Urand());
}

point deltapoint_distribution::sample(){
	return p;
}

int terrell_distribution::sample(){
	double u = Urand();
	double s = 0.0;
	
	// sample the cumulative distribution based on the error function
	
	//TEST INDEPEDENTLY
	for (int i = 0; i < std::numeric_limits<int>::max(); i++){
		double a = ((double)i -nu_bar + 0.5 + b)/gamma;

		s = 0.5 * (std::erf(a/std::sqrt(2.0))+1.0);

		if (u <= s){
			return i;
		}
	}

	assert( false ); // should never reach here
	return 0;
}

point linearDirection_distribution::sample(){
	// step 1: sample the cosine of the polar angle
	double mu_0 = std::sqrt(2.0*Urand()/slope + a*a);
	
	// step 2: sample the azimuthal angle
	double azi = 2.0 * std::acos(-1.0) * Urand();
	
	// convert to Cartesian coordinates
	double c = std::sqrt( 1.0 - mu_0 * mu_0 );
	point p;
	p.x = mu_0;
	p.y = std::sin( azi ) * c;
	p.z = std::cos( azi ) * c;
	
	return p;
}
