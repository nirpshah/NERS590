#ifndef _CELL_HEADER_
#define _CELL_HEADER_

#include <vector>
#include <utility>
#include <memory>

#include "Point.h"
#include "Surface.h"

class cell {
private:
	// vector of: pair of: surface and sense
    std::vector< std::pair< std::shared_ptr< surface >, int > > surfaces;
public:
    cell() {};
    ~cell() {};

    void addsurface( std::shared_ptr< surface > S, int sense );
    bool testpoint( point p );
    std::pair< std::shared_ptr< surface >, double > surfaceIntersect( ray r );
};

#endif
