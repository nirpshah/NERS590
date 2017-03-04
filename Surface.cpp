#include <cmath>
#include <limits>

#include "Point.h"
#include "Surface.h"
#include "Particle.h"

// evaluates the surface equation w.r.t. to point p
double plane::eval( point p ) {
  return a * p.x  +  b * p.y  +  c * p.z  - d;
}

// determines the mininum positive distance to intersection for a ray r
// (returns a very large number if no intersection along ray for ease of calculation down the line)
double plane::distance( ray r ) {
	point p = r.pos;
	point u = r.dir;

	double denom = a * u.x  +  b * u.y  +  c * u.z;
	// epsilon is the smallest double precision number
	if ( std::fabs( denom ) > std::numeric_limits<double>::epsilon() ) {
		double dist = ( d - a * p.x - b * p.y - c * p.z ) / denom;
		if ( dist > 0.0 ) { return dist; }
		// essentially return infinity because the ray is parallel to the plane
		else { return std::numeric_limits<double>::max(); }
	} else {
		// moving in a direction that is (or is very close to) parallel to the surface
		return std::numeric_limits<double>::max();
	}
}

long long plane::cross_surface (particle * p, long long active_cell){

	
	if(reflecting){
		p->reflect(outward_normal);
		p->x_0(); //prevent any leakage
	} else{
		if (p -> dir().x < 0){
			active_cell--;
		}else{
			active_cell ++;
		}
	}
	
	p -> move(100*std::numeric_limits<double>::epsilon());
	return active_cell;
}


