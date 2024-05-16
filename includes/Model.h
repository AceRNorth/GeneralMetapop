#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <array>
#include "constants.h"
#include "Params.h"
#include "Patch.h"
#include "Dispersal.h"
#include "Aestivation.h"
#include "GDRelease.h"
#include "Seasonality.h"
#include "Point.h"

using namespace constants;

class Patch;
class Aestivation;
class Dispersal;
class GDRelease;
class Seasonality;

// Runs the model.
class Model {
public:
	Model(ModelParams *params, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &inher_frac, SineRainfallParams *season, double a0_mean, double a0_var,
	 BoundaryType boundary = BoundaryType::Toroid, DispersalType disp_type = DispersalType::DistanceKernel, std::vector<Point> coords = {});
	Model(ModelParams *params, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &inher_frac, InputRainfallParams *season, double a0_mean, double a0_var,
	 BoundaryType boundary = BoundaryType::Toroid, DispersalType disp_type = DispersalType::DistanceKernel, std::vector<Point> coords = {});
	~Model();
	void initiate();
	void run(int day);

	long long int calculate_tot_J(); 
	long long int calculate_tot_M();
	long long int calculate_tot_V();
	long long int calculate_tot_F();
	std::array<long long int, num_gen> calculate_tot_M_gen();
	std::vector<Patch*> get_sites() const;
	int get_day() const;
	double get_alpha(double alpha0);

private:
	std::vector<Patch*> sites;
	Dispersal* dispersal;
	Aestivation* aestivation;
	GDRelease* gd_release;
	Seasonality* seasonality;

	int day_sim; // current day of the simulation

	// simulation area parameters
	int num_pat; // number of population sites chosen for the simulation
	double side; // size of the square simulation area (side x side) (km)

	// initial population values - common for all Patches
	InitialPopsParams *initial_pops;

	// juvenile development parameters - common for all Patches
	int min_dev; // minimum development time for a juvenile (in days)
	std::array<double, max_dev+1> dev_duration_probs; // array of probabilities of juvenile development duration for a new juvenile
	// (index indicates the number of days to develop or, equivalently, the age class the new juvenile starts at)

	// inheritance
	// f_ijk is the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
	std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> inher_fraction;

	// Patch construction parameters
	double alpha0_mean; // mean of the baseline contribution to the carrying capacity
    double alpha0_variance; // variance of the baseline contribution to the carrying capacity
	double alpha0();

	// initiation methods
	void populate_sites();
	void set_dev_duration_probs(int min_time, int max_time);

	// life-processes - interface with Patch
	void run_step(int day);
	void juv_get_older();
	void adults_die();
	void virgins_mate();
	void lay_eggs();
	void juv_eclose();
};

#endif //MODEL_H