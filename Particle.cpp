#include <cmath>
#include <iostream>
#include <limits>

#include "Particle.h"
#include "Random.h"

// constructor for a new particle
particle::particle( point p, point d ) : p_pos(p), p_dir(d) {
	exist = true;

	// normalize the unit vector
	double norm = 1.0 / std::sqrt( p_dir.x * p_dir.x  +  p_dir.y * p_dir.y  +  p_dir.z * p_dir.z );
	p_dir.x /= norm;
	p_dir.y /= norm;
	p_dir.z /= norm;
	
	if (p_pos.x < 0){
		p_pos.x = 1e-15;
	}
}

// move the particle along its current trajectory
	void particle::move( double s ) {
	p_pos.x += s * p_dir.x;
	p_pos.y += s * p_dir.y;
	p_pos.z += s * p_dir.z;
}

// scatter particle given input direction cosine cos_t0 = mu0
void particle::scatter( double cos_t0 ) {

	//std::cout << "scatter from: " << std::endl;
	// std::cout << p_dir.x << "  " << p_dir.y << "  " << p_dir.z << std::endl;
  
	// sample a random azimuthal angle uniformly
	double azi = 2.0 * std::acos(-1.0) * Urand();
	double cos_azi = std::cos(azi);
	double sin_azi = std::sin(azi);

	// rotate the local particle coordinate system aligned along the incident direction
	// to the global problem (x,y,z) coordinate system 
	double sin_t  = std::sqrt( 1.0 - p_dir.z  * p_dir.z  );
	double sin_t0 = std::sqrt( 1.0 - cos_t0 * cos_t0 );
	double c = sin_t0 / sin_t;

	point q;
	q.x = p_dir.x * cos_t0 + ( p_dir.x * p_dir.z * cos_azi - p_dir.y * sin_azi ) * c;
	q.y = p_dir.y * cos_t0 + ( p_dir.y * p_dir.z * cos_azi + p_dir.x * sin_azi ) * c;
	q.z = p_dir.z * cos_t0 - cos_azi * sin_t0 * sin_t;

	p_dir.x = q.x;
	p_dir.y = q.y;
	p_dir.z = q.z;
	
	// std::cout << p_dir.x << "  " << p_dir.y << "  " << p_dir.z << std::endl;
}

// set the particles life flag to false
void particle::kill() {
	exist = false;

}

void particle::reflect(point surf_norm) {
	double omega_dot_norm = p_dir.x * surf_norm.x + p_dir.y * surf_norm.y + p_dir.z * surf_norm.z;
	p_dir.x = p_dir.x - 2.0 * omega_dot_norm * surf_norm.x;
	p_dir.y = p_dir.y - 2.0 * omega_dot_norm * surf_norm.y;
	p_dir.z = p_dir.z - 2.0 * omega_dot_norm * surf_norm.z;

}

void particle::x_0(){
	// I ran into floating point errors for the leakage out the left side in problem 
	// three. This method puts the particle just inside the x = 0 plane, and is only
	// called for the reflecting planes in problem 3.
	p_pos.x = std::numeric_limits<double>::epsilon();
}
