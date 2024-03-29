#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <limits>
#include <chrono>
#include <vector>
#include "Simulation.h"
#include "Params.h"
#include "input.h"

int main()
{	
	std::vector<InputParams> sets = {set1, set2, set3, set4, set5, set6, set7, set8, set9, set10};

	ProgressionParams prog;
	AreaParams area;
	LifeParams life;
	InheritanceParams inher;
	ReleaseParams rel;
	DispersalParams disp;
	AestivationParams aes;
	InitialPopsParams initial;
	RecordParams rec;
	
	// input parameters
	// progression parameters
	int num_runs;
	int max_t; 

	// area parameters
	int num_pat;  
	double side;
	
	// model parameters
	double mu_j;
	double mu_a;
	double beta;
	double theta;
	double alpha0;
	double mean_dev;
	int min_dev;

	// gene drive inheritance parameters
	double gamma;
	double xi;
	double e;

	// gene drive release parameters
	int driver_start;
	int num_driver_M;
	int num_driver_sites; 

	// dispersal parameters 
	double disp_rate; 
	double max_disp; 

	// aestivation parameters
	double psi;
	double mu_aes;
	int t_hide1;
	int t_hide2;
	int t_wake1; 
	int t_wake2;

	// data-recording parameters
	int rec_start; 
	int rec_end;
	int rec_interval_global;
	int rec_interval_local;
	int rec_sites_freq; 
	int set_label; 

	bool continue_program = true;

	while (continue_program) {
		std::cout << "Welcome to GDSiMS: the Gene Drive Simulator of Mosquito Spread. \n";
		std::cout << "Set 1  - default \n";
		std::cout << "Set 2  - 1 population \n";
		std::cout << "Set 3  - no dispersal \n";
		std::cout << "Set 4  - full mixing \n";
		std::cout << "Set 5  - 1 day \n";
		std::cout << "Set 6  - small area \n";
		std::cout << "Set 7  - low aestivation \n";
		std::cout << "Set 8  - high aestivation \n";
		std::cout << "Set 9  - no gene drive \n";
		std::cout << "Set 10 - high gene drive \n";
		std::cout << "Set 100  - custom \n";
		std::cout << "Default parameter sets include gene drive and dispersal but don't include aestivation unless otherwise stated. \n \n";
		std::cout << "Please select which parameter set (1-10, 100) you'd like to preview, \n";
		std::cout << "or enter 0 to exit the program:" << std::endl;

		int option1;
		char option2;
		std::cin.clear();
		std::cin >> option1;

		if (std::cin.fail() || std::cin.peek() != '\n' || option1 < 0 || (option1 > sets.size() && option1 != 100)) {
			do {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Invalid option. Please enter a number between 0 and 10 (or 100):" << std::endl;
				std::cin >> option1;
			} while (std::cin.fail() || std::cin.peek() != '\n' || option1 < 0 || (option1 > sets.size() && option1 != 100));
		}
		if (option1 == 0) {
			continue_program = false;
		}
		else if (option1 == 100) {
			bool invalid_input = true;
			while (invalid_input) {
				std::cout << "\n" << "Custom set: \n";
				std::cout << "num_runs             " << "\n"; 
				std::cout << "max_t                " << "\n"; 
				std::cout << "num_pat              " << "\n";  
				std::cout << "side                 " << "\n"; 
				std::cout << "mu_j                 " << "\n"; 
				std::cout << "mu_a                 " << "\n"; 
				std::cout << "beta                 " << "\n"; 
				std::cout << "theta                " << "\n"; 
				std::cout << "alpha0               " << "\n"; 
				std::cout << "mean_dev             " << "\n"; 
				std::cout << "min_dev              " << "\n"; 
				std::cout << "gamma                " << "\n"; 
				std::cout << "xi                   " << "\n"; 
				std::cout << "e                    " << "\n"; 
				std::cout << "driver_start         " << "\n"; 
				std::cout << "num_driver_M         " << "\n"; 
				std::cout << "num_driver_sites     " << "\n"; 
				std::cout << "disp_rate            " << "\n"; 
				std::cout << "max_disp             " << "\n"; 
				std::cout << "psi                  " << "\n"; 
				std::cout << "mu_aes               " << "\n"; 
				std::cout << "t_hide1              " << "\n"; 
				std::cout << "t_hide2              " << "\n"; 
				std::cout << "t_wake1              " << "\n"; 
				std::cout << "t_wake2              " << "\n"; 
				std::cout << "rec_start            " << "\n"; 
				std::cout << "rec_end              " << "\n"; 
				std::cout << "rec_interval_global  " << "\n"; 
				std::cout << "rec_interval_local   " << "\n"; 
				std::cout << "rec_sites_freq       " << "\n"; 
				std::cout << "set_label            " << "\n"; 

				std::string params_filename;
				std::cout << "\n" << "Please enter the filename of the parameters file (e.g. 'params.txt'). ";
				std::cout << "This must be a .txt file with parameter values in the above order and format: " << std::endl;
				std::cin >> params_filename;

				auto params_filepath = std::filesystem::path(std::string("./")+params_filename);
				if (std::cin.fail() || !std::filesystem::exists(params_filepath) || !std::filesystem::is_regular_file(params_filepath)) {
					do {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid filename. Please make sure the file is in your current directory and enter the filename again:" << std::endl;
					std::cin >> params_filename;
					params_filepath = std::filesystem::path(std::string("./")+params_filename);
					} while (std::cin.fail() || !std::filesystem::exists(params_filepath) || !std::filesystem::is_regular_file(params_filepath));
				}

				std::ifstream file(params_filename);
				if (file.is_open()) {
					if (!read_and_validate_type(file, num_runs, "num_runs", "int")) continue;
					if (!read_and_validate_type(file, max_t, "max_t", "int")) continue;
					if (!read_and_validate_type(file, num_pat, "num_pat", "int")) continue;
					if (!read_and_validate_type(file, side, "side", "double")) continue;
					if (!read_and_validate_type(file, mu_j, "mu_j", "double")) continue;
					if (!read_and_validate_type(file, mu_a, "mu_a", "double")) continue;
					if (!read_and_validate_type(file, beta, "beta", "double")) continue;
					if (!read_and_validate_type(file, theta, "theta", "double")) continue;
					if (!read_and_validate_type(file, alpha0, "alpha0", "double")) continue;
					if (!read_and_validate_type(file, mean_dev, "mean_dev", "double")) continue;
					if (!read_and_validate_type(file, min_dev, "min_dev", "int")) continue;
					if (!read_and_validate_type(file, gamma, "gamma", "double")) continue;
					if (!read_and_validate_type(file, xi, "xi", "double")) continue;
					if (!read_and_validate_type(file, e, "e", "double")) continue;
					if (!read_and_validate_type(file, driver_start, "driver_start", "int")) continue;
					if (!read_and_validate_type(file, num_driver_M, "num_driver_M", "int")) continue;
					if (!read_and_validate_type(file, num_driver_sites, "num_driver_sites", "int")) continue;
					if (!read_and_validate_type(file, disp_rate, "disp_rate", "double")) continue;
					if (!read_and_validate_type(file, max_disp, "max_disp", "double")) continue;
					if (!read_and_validate_type(file, psi, "psi", "double")) continue;
					if (!read_and_validate_type(file, mu_aes, "mu_aes", "double")) continue;
					if (!read_and_validate_type(file, t_hide1, "t_hide1", "int")) continue;
					if (!read_and_validate_type(file, t_hide2, "t_hide2", "int")) continue;
					if (!read_and_validate_type(file, t_wake1, "t_wake1", "int")) continue;
					if (!read_and_validate_type(file, t_wake2, "t_wake2", "int")) continue;
					if (!read_and_validate_type(file, rec_start, "rec_start", "int")) continue;
					if (!read_and_validate_type(file, rec_end, "rec_end", "int")) continue;
					if (!read_and_validate_type(file, rec_interval_global, "rec_interval_global", "int")) continue;
					if (!read_and_validate_type(file, rec_interval_local, "rec_interval_local", "int")) continue;
					if (!read_and_validate_type(file, rec_sites_freq, "rec_sites_freq", "int")) continue;
					if (!read_and_validate_type(file, set_label, "set_label", "int")) continue;
				}
				file.close();

				int bound_errors = 0; // so only run interval checks if bounds on variables are valid 
				// (and can display all bound errors at once)
				if (!check_bounds("num_runs", num_runs, 0, false)) bound_errors++;
				if (!check_bounds("max_t", max_t, 0, false)) bound_errors++;
				if (!check_bounds("num_pat", num_pat, 0, false)) bound_errors++;
				if (!check_bounds("side", side, 0.0, false)) bound_errors++;
				if (!check_bounds("mu_j", mu_j, 0.0, true, 1.0, false)) bound_errors++;
				if (!check_bounds("mu_a", mu_a, 0.0, false, 1.0, false)) bound_errors++;
				if (!check_bounds("beta", beta, 0.0, false)) bound_errors++;
				if (!check_bounds("theta", theta, 0.0, false)) bound_errors++;
				if (!check_bounds("alpha0", alpha0, 0.0, false)) bound_errors++;
				if (!check_bounds("mean_dev", mean_dev, 0.0, false)) bound_errors++;
				if (!check_bounds("min_dev", min_dev, 0, false)) bound_errors++;
				if (!check_bounds("gamma", gamma, 0.0, true, 1.0, true)) bound_errors++;
				if (!check_bounds("xi", xi, 0.0, true, 1.0, true)) bound_errors++;
				if (!check_bounds("e", e, 0.0, true, 1.0, true)) bound_errors++;
				if (!check_bounds("driver_start", driver_start, 1)) bound_errors++;
				if (!check_bounds("num_driver_M", num_driver_M, 0)) bound_errors++;
				if (!check_bounds("num_driver_sites", num_driver_sites, 0)) bound_errors++;
				if (!check_bounds("disp_rate", disp_rate, 0.0, true, 1.0, true)) bound_errors++;
				if (!check_bounds("max_disp", max_disp, 0.0, false, side, true)) bound_errors++;
				if (!check_bounds("psi", psi, 0.0, true, 1.0, true)) bound_errors++;
				if (!check_bounds("mu_aes", mu_aes, 0.0, true, 1.0, true)) bound_errors++;
				if (psi > 0) {
					if (!check_bounds("t_hide1", t_hide1, 1, true, 365, true)) bound_errors++;
					if (!check_bounds("t_hide2", t_hide2, 1, true, 365, true)) bound_errors++;
					if (!check_bounds("t_wake1", t_wake1, 1, true, 365, true)) bound_errors++;
					if (!check_bounds("t_wake2", t_wake2, 1, true, 365, true)) bound_errors++;
				}
				if (!check_bounds("rec_start", rec_start, 0)) bound_errors++;
				if (!check_bounds("rec_end", rec_end, 0)) bound_errors++;
				if (!check_bounds("rec_interval_global", rec_interval_global, 1)) bound_errors++;
				if (!check_bounds("rec_interval_local", rec_interval_local, 1)) bound_errors++;
				if (!check_bounds("rec_sites_freq", rec_sites_freq, 1)) bound_errors++;
				if (!check_bounds("set_label", set_label, 0)) bound_errors++;

				if (bound_errors == 0) {
					int interval_errors = 0; // so can display all errors at once before continuing while loop
					// (only valid because variables don't overlap between different errors)
					if (psi > 0) {
						if (!check_interval("t_hide1", "t_hide2", t_hide1, t_hide2)) interval_errors++;
						if (!check_interval("t_wake1", "t_wake2", t_wake1, t_wake2)) interval_errors++;
					}
					if (!check_interval("rec_start", "rec_end", rec_start, rec_end)) interval_errors++;

					if (interval_errors != 0) continue;
				}
				else continue;

				invalid_input = false;
			}

			std::cout << "\n" << "Custom set: \n";
			std::cout << "num_runs             " << num_runs << "\n"; 
			std::cout << "max_t                " << max_t << "\n"; 
			std::cout << "num_pat              " << num_pat << "\n";  
			std::cout << "side                 " << side << "\n"; 
			std::cout << "mu_j                 " << mu_j << "\n"; 
			std::cout << "mu_a                 " << mu_a << "\n"; 
			std::cout << "beta                 " << beta << "\n"; 
			std::cout << "theta                " << theta << "\n"; 
			std::cout << "alpha0               " << alpha0 << "\n"; 
			std::cout << "mean_dev             " << mean_dev << "\n"; 
			std::cout << "min_dev              " << min_dev << "\n"; 
			std::cout << "gamma                " << gamma << "\n"; 
			std::cout << "xi                   " << xi << "\n"; 
			std::cout << "e                    " << e << "\n"; 
			std::cout << "driver_start         " << driver_start << "\n"; 
			std::cout << "num_driver_M         " << num_driver_M << "\n"; 
			std::cout << "num_driver_sites     " << num_driver_sites << "\n"; 
			std::cout << "disp_rate            " << disp_rate << "\n"; 
			std::cout << "max_disp             " << max_disp << "\n"; 
			std::cout << "psi                  " << psi << "\n"; 
			std::cout << "mu_aes               " << mu_aes << "\n"; 
			std::cout << "t_hide1              " << t_hide1 << "\n"; 
			std::cout << "t_hide2              " << t_hide2 << "\n"; 
			std::cout << "t_wake1              " << t_wake1 << "\n"; 
			std::cout << "t_wake2              " << t_wake2 << "\n"; 
			std::cout << "rec_start            " << rec_start << "\n"; 
			std::cout << "rec_end              " << rec_end << "\n"; 
			std::cout << "rec_interval_global  " << rec_interval_global << "\n"; 
			std::cout << "rec_interval_local   " << rec_interval_local << "\n"; 
			std::cout << "rec_sites_freq       " << rec_sites_freq << "\n"; 
			std::cout << "set_label            " << set_label << "\n"; 

			if (num_driver_sites > num_pat) {
				std::cout << "Warning: num_driver_sites > num_pat. ";
				std::cout << "This simulation will have reduced gene drive release sites num_driver_sites = num_pat." << std::endl;
			}
			if (driver_start > max_t || num_driver_sites == 0 || num_driver_M == 0) {
				std::cout << "Warning: num_driver_sites or num_driver_M = 0, or driver_start > max_t. ";
				std::cout << "This simulation will not include gene drive." << std::endl;
				std::cout << "Inheritance parameters xi, e and gamma will have no effect." << std::endl;
			} 
			if (disp_rate == 0 || max_disp == 0) {
				std::cout << "Warning: disp_rate or max_disp = 0. This simulation will not include dispersal." << std::endl;
			} 
			if (max_disp > side/2) {
				std::cout << "Warning: max_disp > side/2." << std::endl;
			}
			if (t_hide1 > max_t || t_hide2 > max_t || t_wake1 > max_t || t_wake2 > max_t) {
				std::cout << "Warning: the aestivation interval times are larger than max_t. ";
				std::cout << "This simulation will only run partly through the aestivation period." << std::endl;
			}
			if (psi == 0) {
				std::cout << "Warning: psi = 0. This simulation will not include aestivation." << std::endl;
			} 

			if (rec_start > max_t) {
				std::cout << "Warning: rec_start > max_t. This simulation will not include local recording." << std::endl;
			}
			if (max_t - rec_interval_local - rec_start < 0) {
				std::cout << "Warning: the interval between rec_start and max_t is larger than rec_interval_local. ";
				std::cout << "This simulation will only record local data for day 0." << std::endl;
			}
			std::cout << "\n" << "Would you like to run the simulation with this set of parameters? (y/n)" << std::endl;

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			bool invalid_option_custom = true;
			while (invalid_option_custom) {
				option2 = 'n';
				std::cin >> option2;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				switch (option2) {
					case 'y':
					{
						prog.num_runs = num_runs;
						prog.max_t = max_t;
						area.num_pat = num_pat;
						area.side = side;
						life.mu_j = mu_j;
						life.mu_a = mu_a;
						life.beta = beta;
						life.theta = theta;
						life.alpha0 = alpha0;
						life.mean_dev = mean_dev;
						life.min_dev = min_dev;
						inher.gamma = gamma;
						inher.xi = xi;
						inher.e = e;
						rel.driver_start = driver_start;
						rel.num_driver_M = num_driver_M;
						rel.num_driver_sites = num_driver_sites;
						disp.disp_rate = disp_rate;
						disp.max_disp = max_disp;
						aes.psi = psi;
						aes.mu_aes = mu_aes;
						aes.t_hide1 = t_hide1;
						aes.t_hide2 = t_hide2;
						aes.t_wake1 = t_wake1;
						aes.t_wake2 = t_wake2;
						rec.rec_start = rec_start;
						rec.rec_end = rec_end;
						rec.rec_interval_global = rec_interval_global;
						rec.rec_interval_local = rec_interval_local;
						rec.rec_sites_freq = rec_sites_freq;
						rec.set_label = set_label;

						auto start_1 = std::chrono::steady_clock::now();

						// run simulation
						Simulation simulation_1(prog, area, life, rel, disp, aes, initial, rec);
						simulation_1.set_inheritance(inher);
						simulation_1.run_reps();

						auto finish_1 = std::chrono::steady_clock::now();
						double elapsed_seconds_1 = std::chrono::duration_cast<std::chrono::duration<double>>(finish_1 - start_1).count();
						std::cout << "Program run time: " << std::endl;
						printf("%.10f\n", elapsed_seconds_1);

						invalid_option_custom = false;
						continue_program = false;
						break;
					}
					case 'n':
						std::cout << std::endl;
						invalid_option_custom = false;
						break;
					default:
						std::cout << "\n" << "Invalid option. Please select again:" << std::endl;
						break;
				}
			}
		}

		else {
			std::cout << "\n" << "Set " << option1 << ": \n";
			std::cout << "num_runs             " << sets[option1 - 1].num_runs << "\n"; 
			std::cout << "max_t                " << sets[option1 - 1].max_t << "\n"; 
			std::cout << "num_pat              " << sets[option1 - 1].num_pat << "\n";  
			std::cout << "side                 " << sets[option1 - 1].side << "\n"; 
			std::cout << "mu_j                 " << sets[option1 - 1].mu_j << "\n"; 
			std::cout << "mu_a                 " << sets[option1 - 1].mu_a << "\n"; 
			std::cout << "beta                 " << sets[option1 - 1].beta << "\n"; 
			std::cout << "theta                " << sets[option1 - 1].theta << "\n"; 
			std::cout << "alpha0               " << sets[option1 - 1].alpha0 << "\n"; 
			std::cout << "mean_dev             " << sets[option1 - 1].mean_dev << "\n"; 
			std::cout << "min_dev              " << sets[option1 - 1].min_dev << "\n"; 
			std::cout << "gamma                " << sets[option1 - 1].gamma << "\n"; 
			std::cout << "xi                   " << sets[option1 - 1].xi << "\n"; 
			std::cout << "e                    " << sets[option1 - 1].e << "\n"; 
			std::cout << "driver_start         " << sets[option1 - 1].driver_start << "\n"; 
			std::cout << "num_driver_M         " << sets[option1 - 1].num_driver_M << "\n"; 
			std::cout << "num_driver_sites     " << sets[option1 - 1].num_driver_sites << "\n"; 
			std::cout << "disp_rate            " << sets[option1 - 1].disp_rate << "\n"; 
			std::cout << "max_disp             " << sets[option1 - 1].max_disp << "\n"; 
			std::cout << "psi                  " << sets[option1 - 1].psi << "\n"; 
			std::cout << "mu_aes               " << sets[option1 - 1].mu_aes << "\n"; 
			std::cout << "t_hide1              " << sets[option1 - 1].t_hide1 << "\n"; 
			std::cout << "t_hide2              " << sets[option1 - 1].t_hide2 << "\n"; 
			std::cout << "t_wake1              " << sets[option1 - 1].t_wake1 << "\n"; 
			std::cout << "t_wake2              " << sets[option1 - 1].t_wake2 << "\n"; 
			std::cout << "rec_start            " << sets[option1 - 1].rec_start << "\n"; 
			std::cout << "rec_end              " << sets[option1 - 1].rec_end << "\n"; 
			std::cout << "rec_interval_global  " << sets[option1 - 1].rec_interval_global << "\n"; 
			std::cout << "rec_interval_local   " << sets[option1 - 1].rec_interval_local << "\n"; 
			std::cout << "rec_sites_freq       " << sets[option1 - 1].rec_sites_freq << "\n"; 
			std::cout << "set_label            " << sets[option1 - 1].set_label << "\n"; 

			std::cout << "\n" << "Would you like to run the simulation with this set of parameters? (y/n)" << std::endl;

			bool invalid_option_default = true;
			while (invalid_option_default) {
				option2 = 'n';
				std::cin >> option2;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				switch (option2) {
					case 'y':
					{
						InputParams sim_params = sets[option1 - 1];

						prog.num_runs = sim_params.num_runs;
						prog.max_t = sim_params.max_t;
						area.num_pat = sim_params.num_pat;
						area.side = sim_params.side;
						life.mu_j = sim_params.mu_j;
						life.mu_a = sim_params.mu_a;
						life.beta = sim_params.beta;
						life.theta = sim_params.theta;
						life.alpha0 = sim_params.alpha0;
						life.mean_dev = sim_params.mean_dev;
						life.min_dev = sim_params.min_dev;
						inher.gamma = sim_params.gamma;
						inher.xi = sim_params.xi;
						inher.e = sim_params.e;
						rel.driver_start = sim_params.driver_start;
						rel.num_driver_M = sim_params.num_driver_M;
						rel.num_driver_sites = sim_params.num_driver_sites;
						disp.disp_rate = sim_params.disp_rate;
						disp.max_disp = sim_params.max_disp;
						aes.psi = sim_params.psi;
						aes.mu_aes = sim_params.mu_aes;
						aes.t_hide1 = sim_params.t_hide1;
						aes.t_hide2 = sim_params.t_hide2;
						aes.t_wake1 = sim_params.t_wake1;
						aes.t_wake2 = sim_params.t_wake2;
						rec.rec_start = sim_params.rec_start;
						rec.rec_end = sim_params.rec_end;
						rec.rec_interval_global = sim_params.rec_interval_global;
						rec.rec_interval_local = sim_params.rec_interval_local;
						rec.rec_sites_freq = sim_params.rec_sites_freq;
						rec.set_label = sim_params.set_label;

						auto start = std::chrono::steady_clock::now();

						// run simulation
						Simulation simulation(prog, area, life, rel, disp, aes, initial, rec);
						simulation.set_inheritance(inher);
						simulation.run_reps();

						auto finish = std::chrono::steady_clock::now();
						double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
						std::cout << "Program run time: " << std::endl;
						printf("%.10f\n", elapsed_seconds);

						invalid_option_default = false;
						continue_program = false;
						break;
					}
					case 'n':
						std::cout << std::endl;
						invalid_option_default = false;
						break;
					default:
						std::cout << "\n" << "Invalid option. Please select again:" << std::endl;
						break;
				}
			}
		}
	}
	
	return 0;
}
