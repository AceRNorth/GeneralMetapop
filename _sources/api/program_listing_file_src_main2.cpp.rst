
.. _program_listing_file_src_main2.cpp:

Program Listing for File main2.cpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_main2.cpp>` (``src\main2.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <iostream>
   #include <string>
   #include "Simulation.h"
   #include "inputval.h"
   #include "constants.h"
   #include "Params.h"
   
   int main() {
   
       //std::string params_filename; // file needs to be in build directory
       //std::cin >> params_filename;
   
       char boundary_type;
       char disp_type;
       std::string coords_filename; // file needs to be in build directory
       std::string rainfall_filename; // file needs to be in build directory
       std::string rel_times_filename; // file needs to be in build directory
   
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
       double comp_power;
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
   
       // seasonality parameters
       double alpha0_mean;
       double alpha0_variance;
       double alpha1;
       double amp;
       double resp;
   
       // data-recording parameters
       int rec_start; 
       int rec_end;
       int rec_interval_global;
       int rec_interval_local;
       int rec_sites_freq; 
       int set_label;
   
       std::cin >> num_runs;
       std::cin >> max_t;
       std::cin >> num_pat;
       std::cin >> side;
       std::cin >> mu_j;
       std::cin >> mu_a;
       std::cin >> beta;
       std::cin >> theta;
       std::cin >> comp_power;
       std::cin >> min_dev;
       std::cin >> gamma;
       std::cin >> xi;
       std::cin >> e;
       std::cin >> driver_start;
       std::cin >> num_driver_M;
       std::cin >> num_driver_sites;
       std::cin >> disp_rate;
       std::cin >> max_disp;
       std::cin >> psi;
       std::cin >> mu_aes;
       std::cin >> t_hide1;
       std::cin >> t_hide2;
       std::cin >> t_wake1;
       std::cin >> t_wake2;
       std::cin >> alpha0_mean;
       std::cin >> alpha0_variance;
       std::cin >> alpha1;
       std::cin >> amp;
       std::cin >> resp;
       std::cin >> rec_start;
       std::cin >> rec_end;
       std::cin >> rec_interval_global;
       std::cin >> rec_interval_local;
       std::cin >> rec_sites_freq;
       std::cin >> set_label;
       std::cin >> boundary_type;
       std::cin >> disp_type;
       std::cin >> coords_filename;
       std::cin >> rainfall_filename; 
       std::cin >> rel_times_filename;
   
       InputParams params;
       params.num_runs = num_runs;
       params.max_t = max_t;
       params.num_pat = num_pat;
       params.side = side;
       params.mu_j = mu_j;
       params.mu_a = mu_a;
       params.beta = beta;
       params.theta = theta;
       params.comp_power = comp_power;
       params.min_dev = min_dev;
       params.gamma = gamma;
       params.xi = xi;
       params.e = e;
       params.driver_start = driver_start;
       params.num_driver_M = num_driver_M;
       params.num_driver_sites = num_driver_sites;
       params.disp_rate = disp_rate;
       params.max_disp = max_disp;
       params.psi = psi;
       params.mu_aes = mu_aes;
       params.t_hide1 = t_hide1;
       params.t_hide2 = t_hide2;
       params.t_wake1 = t_wake1;
       params.t_wake2 = t_wake2;
       params.alpha0_mean = alpha0_mean;
       params.alpha0_variance = alpha0_variance;
       params.alpha1 = alpha1;
       params.amp = amp;
       params.resp = resp;
       params.rec_start = rec_start;
       params.rec_end = rec_end;
       params.rec_interval_global = rec_interval_global;
       params.rec_interval_local = rec_interval_local;
       params.rec_sites_freq = rec_sites_freq;
       params.set_label = set_label;
   
       BoundaryType boundary;
       if (boundary_type == 't') {
           boundary = Toroid;
       }
       else if (boundary_type == 'e') {
           boundary = Edge;
       }
   
       DispersalType disp;
       if (disp_type == 'd') {
           disp = DistanceKernel;
       }
       else if (disp_type == 'r') {
           disp = Radial;
       }
   
       Simulation simulation(params);
       InheritanceParams inher;
       inher.gamma = params.gamma;
       inher.xi = params.xi;
       inher.e = params.e;
       simulation.set_inheritance(inher);
       simulation.set_boundary_type(boundary);
       simulation.set_dispersal_type(disp);
   
       if (coords_filename != "none") {
           simulation.set_coords(coords_filename);
       }
       if (rainfall_filename != "none") {
           simulation.set_rainfall(rainfall_filename);
       }
       if (rel_times_filename != "none") {
           simulation.set_release_times(rel_times_filename);
       }
       simulation.run_reps();
   
       return 0;
   }
