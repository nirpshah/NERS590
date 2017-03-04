#ifndef _PARTICLE_HEADER_
#define _PARTICLE_HEADER_

#include "Point.h"

class particle {
  private:
    point p_pos, p_dir;
    bool  exist;
  public:
     particle( point p, point d );
    ~particle() {};

    point pos() { return p_pos; };    // return particle position
    point dir() { return p_dir; };    // return particle direction
	ray p_ray()	{
		ray r(p_pos,p_dir);
		return r;
	}; 
	// return the ray of the particle
    bool alive() { return exist; };   // return particle state flag

    void move( double s );
    void scatter( double mu0 );
    void kill();
	void reflect(point surf_norm);
	void x_0();
};

#endif
