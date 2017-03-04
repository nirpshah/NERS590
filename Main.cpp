//============================================================================
// Name        : HW1_AC.cpp
// Author      : Austin Carter	
// Version     : 2
// Copyright   : February 2017
// Description : Monte Carlo Transport Code for 6 unique cases
//============================================================================

// Required Libraries
#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <stack>
#include <time.h>
#include <cmath>
#include <string>
#include <stack>
#include <utility>

// my class files
#include "Point.h"
#include "Surface.h"
#include "Cell.h"
#include "Nuclide.h"
#include "Random.h"
#include "Distribution.h"
#include "Particle.h"
#include "Material.h"
#include "Reaction.h"
#include "WriteToFile.h"


int main() {

	// This is the number of histories
	long long N_h = 1e7; 	
	
	// PROBLEM SET UP ********************************************************************
	// ***********************************************************************************
	
	std::cout << "Please Select the Problem you wish to perform "
			"(1a, 1b, 2a, 2b, 3a, 3b): " << std::endl;
	std::string selection;
	std::cin >> selection;	
	
	std::cout << "Calculating ... " << std::endl;
	
	// M_vec is a vector of materials with indices identifying the cell number.
	// i.e. M_vec[0] refers to the material in cell 0.
	std::vector<material> M_vec;

	// Geometry:  ////////////////////////////////////////////////////////////////////////
	// C_vec is the cell vector, which is modified by each case
	std::vector<cell> C_vec;
	// only 2 types of cells in this case which are modified in each problem accordingly
	cell reactor, reflector;
	// All the geometry for all 6 problem cases is dependent on planes in the x direction
	std::shared_ptr<surface> px;
	
	// Set up the shared pointers necessary for the collision distributions //////////////
	
	// scatterDist changes depending on the problem, but defaults to isotropic
	// problem 1: no scatter, problem 2: various scatterDist, problem 3: isotropic
	std::shared_ptr<distribution<double>> scatterDist = 
		std::make_shared<uniform_distribution> (-1.0 , 1.0);	
  
  	// capture distribution is always the exponential distribution, and is defined
	// in the transport loop
	// the fission distribution is default to the floor distribution,
	// which changes in the case of 3b.
	std::shared_ptr<distribution<int>> fissionDist = 
		std::make_shared<floor_distribution> (2.8);
	
	// two necessary distributions that are necessary for setting up sources:
	// default position source distribution is the delta source at x = 0;
	// default direction source distribution is the delta source at mu = 1;
	std::shared_ptr<distribution<point>> Pos_sourceDist = 
		std::make_shared<deltapoint_distribution> (std::numeric_limits<double>::epsilon(), 0.0,0.0);
	std::shared_ptr<distribution<point>> Dir_sourceDist = 
		std::make_shared<deltapoint_distribution> (1.0, 0.0, 0.0);
	
	// switch statement does not work for std::string, so I am using a series of if/else
	if (selection == "1a"){
		// Set up the Geometry ///////////////////////////////////////////////////////////
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, false);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 5.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// Set up the Material Cross Sections ////////////////////////////////////////////
		// There are multiple nuclides in the same material in the 0th cell
		material M("Reactor_M", 1.0);
		M_vec.push_back(M);
		
		std::shared_ptr< reaction > cap1 = std::make_shared< capture_reaction > ( 2 );
		std::shared_ptr< reaction > cap2 = std::make_shared< capture_reaction > ( 0.5 );
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		std::shared_ptr< nuclide > N2 = std::make_shared< nuclide > ( "nuclide 2" );
		
		N1 -> addReaction(cap1);
		N2 -> addReaction(cap2);
		M_vec[0].add_nuclide(N1, 0.25);
		M_vec[0].add_nuclide(N2, 0.75);
		
		// Set up the Source Distribution: Use Defaults //////////////////////////////////
		// Set up the Scatter and Fission Distributions: Use Defaults ////////////////////
		
	}else if (selection == "1b"){
		// Set up the Geometry ///////////////////////////////////////////////////////////
		// both cells are included this time: reactor and reflector
		// reactor setup
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, false);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 1.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// reflector setup
		// note px does not need to be remade because it is shared between cells
		reflector.addsurface(px, +1);
		px = std::make_shared<plane> ("px3", 1.0, 0.0, 0.0, 5.0 , false);
		reflector.addsurface(px, -1);
		C_vec.push_back(reflector);
		
		// Set up the Source Distribution: Use Defaults
		// Set up the Scatter and Fission Distributions: Use Defaults
		
		// Set up the Material Cross Sections ////////////////////////////////////////////
		// in this case, the number of cells increased, so we have to adjust the material
		// vector accordingly
		material M_reactor("Reactor_M", 1.0);
		M_vec.push_back(M_reactor);
		material M_reflector("Reflector_M", 1.0);
		M_vec.push_back(M_reactor);
		
		std::shared_ptr< reaction > cap1 = std::make_shared< capture_reaction > ( 2 );
		std::shared_ptr< reaction > cap2 = std::make_shared< capture_reaction > ( 0.5 );
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		std::shared_ptr< nuclide > N2 = std::make_shared< nuclide > ( "nuclide 2" );
		
		N1 -> addReaction(cap1);
		N2 -> addReaction(cap2);
		
		// each material only has 1 nuclide, therefore their atomic fraction is 1
		M_vec[0].add_nuclide(N1, 1.0);
		M_vec[1].add_nuclide(N2, 1.0);
		
	}else if (selection == "2a"){
		// Set up the Geometry ///////////////////////////////////////////////////////////
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, false);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 10.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// Set up the Source Distribution:
		Dir_sourceDist = std::make_shared<linearDirection_distribution> (0.0, 1.0);
		// Default Pos_sourceDist is correct
		
		// Set up the Scatter and Fission Distributions: Use Defaults
		
		// Set up the Material Cross Sections ////////////////////////////////////////////
		material M_reactor("Reactor_M", 1.0);
		M_vec.push_back(M_reactor);
		
		std::shared_ptr< reaction > cap  = std::make_shared< capture_reaction > ( 0.02 );
		std::shared_ptr< reaction > scat = std::make_shared< scatter_reaction > ( 0.98 , scatterDist);
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		
		N1 -> addReaction(cap);
		N1 -> addReaction(scat);
	
		M_vec[0].add_nuclide(N1, 1.0);
		
		
	}else if (selection == "2b"){
		// Set up the Geometry ///////////////////////////////////////////////////////////
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, false);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 10.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// Set up the Source Distribution:
		Dir_sourceDist = std::make_shared<linearDirection_distribution> (0.0, 1.0);
		// Default Pos_sourceDist is correct
		
		// Set up the Scatter and Fission Distributions
		scatterDist = std::make_shared<henyey_greenstein_distribution> (0.25);
		// Fission Distribution is unused/unchanged
		
		// Set up the Material Cross Sections ////////////////////////////////////////////
		material M_reactor("Reactor_M", 1.0);
		M_vec.push_back(M_reactor);
		
		std::shared_ptr< reaction > cap  = std::make_shared< capture_reaction > ( 0.02 );
		std::shared_ptr< reaction > scat = std::make_shared< scatter_reaction > ( 0.98, scatterDist);
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		
		N1 -> addReaction(cap);
		N1 -> addReaction(scat);
	
		M_vec[0].add_nuclide(N1, 1.0);
		
	}else if (selection == "3a"){
		
		// Set up the Geometry ///////////////////////////////////////////////////////////
		// note that in problem 3 the left bounding surface is a REFLECTING xPlane
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, true);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 4.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// Set up the Source Distribution
		Pos_sourceDist = std::make_shared<uniformXpoint_distribution> (0.0, 4.0);
		Dir_sourceDist = std::make_shared<isotropicDirection_distribution> (-1.0, 1.0);
		
		// Set up the Scatter and Fission Distributions: Use Defaults
		// Default Scatter Distribution is good
		
		/// Set up the Material Cross Sections ////////////////////////////////////////////
		material M_reactor("Reactor_Fuel", 0.1);
		M_vec.push_back(M_reactor);
		
		std::shared_ptr< reaction > cap     = std::make_shared< capture_reaction > ( 0.3 );
		std::shared_ptr< reaction > scat    = std::make_shared< scatter_reaction > ( 1.5, scatterDist );
		std::shared_ptr< reaction > fission = std::make_shared< fission_reaction > ( 0.2, fissionDist );
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		
		N1 -> addReaction(cap);
		N1 -> addReaction(scat);
		N1 -> addReaction(fission);
	
		M_vec[0].add_nuclide(N1, 1.0);
		
	}else if (selection == "3b"){
		
		// Set up the Geometry ///////////////////////////////////////////////////////////
		// note that in problem 3 the left bounding surface is a REFLECTING xPlane
		px = std::make_shared<plane> ("px0", 1.0, 0.0, 0.0, 0.0, true);
		reactor.addsurface(px, +1);
		px = std::make_shared<plane> ("px1", 1.0, 0.0, 0.0, 4.0, false);
		reactor.addsurface(px, -1);
		C_vec.push_back(reactor);
		
		// Set up the Source Distribution
		Pos_sourceDist = std::make_shared<uniformXpoint_distribution> (0.0, 4.0);
		Dir_sourceDist = std::make_shared<isotropicDirection_distribution> (-1.0, 1.0);
		
		// Set up the Scatter and Fission Distributions
		fissionDist = std::make_shared<terrell_distribution> (2.8, 1.1, 1.41e-3);
		// Default Scatter Distribution is good		
		
		// Set up the Material Cross Sections ////////////////////////////////////////////
		material M_reactor("Reactor_Fuel", 0.1);
		M_vec.push_back(M_reactor);
		
		std::shared_ptr< reaction > cap     = std::make_shared< capture_reaction > ( 0.3 );
		std::shared_ptr< reaction > scat    = std::make_shared< scatter_reaction > ( 1.5 , scatterDist );
		std::shared_ptr< reaction > fission = std::make_shared< fission_reaction > ( 0.2, fissionDist );
	
		std::shared_ptr< nuclide > N1 = std::make_shared< nuclide > ( "nuclide 1" );
		
		N1 -> addReaction(cap);
		N1 -> addReaction(scat);
		N1 -> addReaction(fission);
	
		M_vec[0].add_nuclide(N1, 1.0);
		
	} else{
		std::cout << "Invalid Selection" << std::endl;
		return 0;
	}	

	// initialize a bank of particles. Type: stack. Particles added at beginning of each 
	// simulation and if there is a multiplication event (fission)
	std::stack<particle> bank;

	long long leftcounter = 0;
	
	long long right_counter_history = 0;
	long long right_counter_total = 0;
	long long sq_right_counter_total = 0;
		
	long long active_cell = 0;
	
	//probability mass vector is length 30, initialized with 0's
	int prob_mesh = 60;
	std::vector<std::pair<double, double>> probability_mass (prob_mesh, std::make_pair(0.0,0.0));

	// SIMULATION LOOP ///////////////////////////////////////////////////////////////////
	for (long long i = 0; i < N_h; i++){

		// sample an initial Point for the particle
		particle p1(Pos_sourceDist->sample(), Dir_sourceDist->sample());
		bank.push(p1);
		// all cells are initialized in the first cell (index 0)
		active_cell = 0;
		
		//for the probability mass function of the number of neutrons that leak out
		right_counter_history = 0;
		
		// HISTORY LOOP //////////////////////////////////////////////////////////////
		while(!bank.empty()){
			// Working Particle is now p
			particle p = bank.top();
			bank.pop();
			
			// PARTICLE LOOP /////////////////////////////////////////////////////////
			while(p.alive()){
				// Sample the distance to collision with material in p's cell
				double SigT = M_vec.at(active_cell).macro_xs();
				exponential_distribution collisionDist(SigT);
				double distColl = collisionDist.sample();
				
				// check for nearest surface interaction
				
				std::pair<std::shared_ptr<surface>, double> Surf = 
					C_vec.at(active_cell).surfaceIntersect(p.p_ray());
				std::shared_ptr< surface > hitSurf = Surf.first;
				double distSurf = Surf.second;
				
				// compare distance to surface to distance to collision
				if(distSurf < distColl){

					p.move(distSurf);
					active_cell = hitSurf->cross_surface(&p, active_cell);
					
					if (active_cell < 0 && hitSurf -> reflects () == false){
						//left leakage
						p.kill();
						leftcounter++;
					} else if (active_cell >= C_vec.size() && hitSurf -> reflects () == false){
						//right leakage
						p.kill();
						right_counter_history++;
						
						// reset the cell number for the next particle (for fission)
						active_cell = 0;
					}
				} else{
					// no cell boundary interaction, there was a collision with 
					// the cell material
					p.move(distColl);
					M_vec.at(active_cell).sample_collision(&p, &bank);
				}
			}
		}
		//for the probability mass function of the number of neutrons that leak out
		right_counter_total    += right_counter_history;
		sq_right_counter_total += right_counter_history * right_counter_history;
		if(right_counter_history > prob_mesh - 1){
			right_counter_history = prob_mesh - 1;
		}
		probability_mass.at(right_counter_history).first += 1.0;
	}
	// finalize the probability mass distribution vector
	for (int i = 0; i < probability_mass.size(); i++){
		probability_mass.at(i).first = probability_mass.at(i).first/(double)N_h;
		probability_mass.at(i).second = 
			std::sqrt(probability_mass.at(i).first * (1-probability_mass.at(i).first)/(N_h-1));
	}
	// finalize estimators
	double mean_right_leakage        = (double)right_counter_total/(double)N_h;
	double mean_sq_right_leakage     = (double)sq_right_counter_total/(double)N_h;
	double var_distribution 		 = mean_sq_right_leakage - mean_right_leakage*mean_right_leakage;
	double uncertainty_right_leakage = std::sqrt(var_distribution/ N_h);
	std::cout << "The mean fraction of neutrons that leak out the right side of the slab" << std::endl;
	std::cout << "per source neutron is:" << std::endl;
	std::cout << mean_right_leakage << " +/- " << uncertainty_right_leakage << std::endl;
	std::cout << "See attached file 'results.txt' for a probability mass function of" << std::endl;
	std::cout << "neutron leakeage from right boundary per source neutron." << std::endl;
	if (selection == "3a" || selection == "3b"){
		std::cout << "The variance of the probability mass distribution is: " << std::endl;
		std::cout << var_distribution << std::endl;
	}
	write_to_file(prob_mesh, probability_mass);
	return 0;
}
