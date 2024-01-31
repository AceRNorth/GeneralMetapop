#ifndef PARAMS_H
#define PARAMS_H

// Model progression parameters
struct ProgressionParams {
	const int num_runs; // number of simulation replicates to run
	const int max_t; // maximum simulated time (in days)
};

// Model area parameters
struct AreaParams {
	const int num_pat; // number of population sites chosen for the simulation
	const double side; // size of the square simulation area (side x side) (km)
};

// Model life-process parameters
struct LifeParams {
	const double mu_j; // juvenile density independent mortality rate per day
	const double mu_a; // adult mortality rate per day
	const double beta; // parameter that controls mating rate
	const double theta; // average egg laying rate of wildtype females (eggs per day)
	const double alpha0; // baseline contribution to carrying capacity
	const double mean_dev; // mean juvenile development time (in days)
	const int min_dev; // minimum development time for a juvenile (in days)
};

// Gene drive inheritance parameters
struct InheritanceParams {
	const double gamma; // rate of r2 allele formation from W/D meiosis
	const double xi; // somatic Cas9 expression fitness cost
	const double e; // homing rate in females
};

// Gene drive release model parameters
struct ReleaseParams {
	const int driver_start; // time to start releasing drive alleles into the mosquito population
	const int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	const int num_driver_sites; // number of gene drive release sites per year
};

// Dispersal model parameters
struct DispersalParams {
	const double disp_rate; // adult dispersal rate
	const double max_disp; // maximum distance at which two sites are connected (km)
};

// Aestivation model parameters
struct AestivationParams {
	const double psi; // aestivation rate
	const double mu_aes; // aestivation mortality
	const int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	const int t_hide2; // end day of aestivation-entering period (day number of the year)
	const int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	const int t_wake2; // end day of aestivation-waking period (day number of the year)
};

// Initial population values for the model
struct InitialPopsParams {
	const int initial_WJ = 10000; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	const int initial_WM = 50000; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	const int initial_WV = 10000; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	const int initial_WF = 40000; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
};

// Data-recording parameters
struct RecordParams {
	// recording window and intervals
	const int rec_start; // start time for the data recording window (in days) (non-inclusive)
	const int rec_end; // end time for the data recording window (in days) (inclusive)
	const int rec_interval_global; // time interval for global data recording/output
	const int rec_interval_local; // time interval at which to collect/record local data (in days)
	const int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)

	// output filename labels
	const int set_label; // 'set of runs' index label for output files
};

#endif //PARAMS_H