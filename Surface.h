#ifndef _SURFACE_HEADER_
#define _SURFACE_HEADER_

#include <string>

#include "Point.h"
#include "Particle.h"

class surface {
private:
	std::string my_name;
protected:
	point outward_normal;
public:

    surface( std::string label ) : my_name(label) {};
    ~surface() {};

	// final means that name cannot be changed... ever.
	// final means no derived class can modify name() method
    virtual std::string name() final { return my_name; };

    virtual double eval( point p )   = 0;
    virtual double distance( ray r ) = 0;
	virtual long long   cross_surface(particle *, long long active_cell) = 0;
	virtual bool reflects() =0;
	
};

class plane : public surface {
private:
    double a, b, c, d;
	bool reflecting;
public:
    plane( std::string label, double p1, double p2, double p3, double p4, bool r_in) : 
		surface(label), a(p1), b(p2), c(p3), d(p4), reflecting(r_in) {
			point p(a, b, c);
			outward_normal = p;
			
			// normalize the unit vector
			double norm = 1.0 / std::sqrt( outward_normal.x * outward_normal.x  +  
				outward_normal.y * outward_normal.y  +  outward_normal.z * outward_normal.z );
			outward_normal.x /= norm;
			outward_normal.y /= norm;
			outward_normal.z /= norm;
		};
    ~plane() {};

    double eval( point p );
    double distance( ray r );
	long long cross_surface(particle * p, long long active_cell);
	point normal() {return outward_normal;};
	bool reflects() { return reflecting;};
};


#endif
