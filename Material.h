#ifndef _MATERIAL_HEADER_
#define _MATERIAL_HEADER_

#include <vector>
#include <string>
#include <utility>
#include <memory>

#include "Nuclide.h"

class material {
  private:
    std::string material_name;
    double      atom_density;
	// first is a shared pointer to a derived class of nuclide
	// second is a double that indicates the atomic fraction
    std::vector< std::pair< std::shared_ptr< nuclide >, double > > nuclides;

    double micro_xs();
  public:
     material( std::string label, double aden ) : material_name(label), atom_density(aden) {};
    ~material() {};

    std::string name() { return material_name; }

    void   add_nuclide( std::shared_ptr< nuclide >, double );
    double macro_xs();
    std::shared_ptr< nuclide > sample_nuclide();

    void sample_collision( particle* p, std::stack<particle>* bank );
};


#endif
