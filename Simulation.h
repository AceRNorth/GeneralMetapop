#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include <vector>
#include <string> // for error messages
#include "constants.h"
#include "Params.h"
#include "Point.h"

using namespace constants;

void out_of_bounds_msg(const std::string& par);
void invalid_interval_msg(const std::string& param1, const std::string& param2);

// Sets up and controls the flow of the simulation.
class Simulation {
public:
	Simulation(ProgressionParams &prog, AreaParams &area, LifeParams &life, ReleaseParams &rel, DispersalParams &disp,
	 AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec); 
	void set_coords(const std::string& coords_file);
	void set_boundary_type(BoundaryType boundary);
	void set_connec_type(ConnecType connec);
	void set_inheritance(InheritanceParams inher_params); 
	void run_reps();

private:
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)

	AreaParams *area_params; // model area parameters
	LifeParams *life_params; // model life-process parameters
	ReleaseParams *rel_params; // gene drive release model parameters
	DispersalParams *disp_params; // dispersal model parameters
	AestivationParams *aes_params; // aestivation model parameters
	InitialPopsParams *initial_params; // initial population values
	RecordParams *rec_params; // data-recording parameters

	// additional parameter options
	std::vector<Point> sites_coords; // 2D coordinates for the sites on the simulated square
	BoundaryType boundary_type;
	ConnecType connec_type;

	// inheritance
	// f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> inher_fraction;
};

#endif //SIMULATION_H