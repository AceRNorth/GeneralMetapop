
.. _program_listing_file_src_Simulation.cpp:

Program Listing for File Simulation.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_Simulation.cpp>` (``src\Simulation.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <array>
   #include <vector>
   #include <iostream> // for error messages
   #include <filesystem>
   #include <fstream>
   #include <sstream>
   #include <string>
   #include "Simulation.h"
   #include "constants.h"
   #include "Model.h"
   #include "Record.h"
   #include "inputval.h"
   
   Simulation::Simulation(InputParams input)
   { 
       num_runs = input.num_runs;
       max_t = input.max_t;
   
       model_params = new ModelParams;
       model_params->area = new AreaParams;
       model_params->area->num_pat = input.num_pat;
       model_params->life = new LifeParams;
       model_params->life->mu_j = input.mu_j;
       model_params->life->mu_a = input.mu_a;
       model_params->life->beta = input.beta;
       model_params->life->theta = input.theta;
       model_params->life->comp_power = input.comp_power;
       model_params->life->min_dev = input.min_dev;
       model_params->rel = new ReleaseParams;
       model_params->rel->release_times.push_back(input.driver_start);
       model_params->rel->num_driver_M = input.num_driver_M;
       model_params->rel->num_driver_sites = input.num_driver_sites;
       model_params->disp = new DispersalParams;
       model_params->disp->disp_rate = input.disp_rate;
       model_params->disp->max_disp = input.max_disp;
       model_params->aes = new AestivationParams;
       model_params->aes->psi = input.psi;
       model_params->aes->mu_aes = input.mu_aes;
       model_params->aes->t_hide1 = input.t_hide1;
       model_params->aes->t_hide2 = input.t_hide2;
       model_params->aes->t_wake1 = input.t_wake1;
       model_params->aes->t_wake2 = input.t_wake2;
       model_params->initial = new InitialPopsParams;
       model_params->initial->initial_WJ = input.initial_WJ;
       model_params->initial->initial_WM = input.initial_WM;
       model_params->initial->initial_WV = input.initial_WV;
       model_params->initial->initial_WF = input.initial_WF;
   
       sine_rainfall_params = new SineRainfallParams;
       sine_rainfall_params->alpha1 = input.alpha1;
       sine_rainfall_params->amp = input.amp;
       input_rainfall_params = new InputRainfallParams;
       input_rainfall_params->alpha1 = input.alpha1;
       input_rainfall_params->resp = input.resp;
       (input_rainfall_params->rainfall).clear();
       alpha0_mean = input.alpha0_mean;
       alpha0_variance = input.alpha0_variance;
   
       rec_params = new RecordParams;
       rec_params->rec_start = input.rec_start;
       rec_params->rec_end = input.rec_end;
       rec_params->rec_interval_global = input.rec_interval_global;
       rec_params->rec_interval_local = input.rec_interval_local;
       rec_params->rec_sites_freq = input.rec_sites_freq;
       rec_params->set_label = input.set_label;
       
       for (int i=0; i < constants::num_gen; ++i) {
           for (int j=0; j < constants::num_gen; ++j) {
               for (int k=0; k < constants::num_gen; ++k) {
                   inher_fraction[i][j][k] = 0;
               }
           }
       }
       InheritanceParams mendelian = {0, 0, 0};
       set_inheritance(mendelian);
   
       sites_coords.clear();
       release_sites.clear();
       boundary_type = BoundaryType::Toroid;
       disp_type = DispersalType::DistanceKernel;
   }
   
   Simulation::~Simulation() 
   {
       delete model_params->area;
       delete model_params->life;
       delete model_params->rel;
       delete model_params->disp;
       delete model_params->aes;
       delete model_params->initial;
       delete model_params;
       delete sine_rainfall_params;
       delete input_rainfall_params;
       delete rec_params;
   }
   
   void Simulation::set_coords(const std::filesystem::path& filepath) 
   {
       sites_coords.clear();
       release_sites.clear();
   
       if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
           std::cerr << "Invalid filename. To enter a filename, the file should be in the build directory. Otherwise, the filepath should be provided (either relative to 'build' or absolute)." << std::endl;
       }
       else {
           std::ifstream file(filepath);
           std::string line;
           std::vector<Point> temp_coords;
           std::vector<int> temp_rel_sites;
           if (file.is_open()) {
               for(int i=0; std::getline(file, line); ++i) {
                   std::stringstream linestream(line);
                   if (line.size() == 0) break;
   
                   double x, y;
                   char is_rel_site;
                   int err = 0;
                   if (!read_and_validate_type(linestream, x, "x" + std::to_string(i+1), "double")) err++;
                   if (!read_and_validate_type(linestream, y, "y" + std::to_string(i+1), "double")) err++;
                   if (!read_and_validate_type(linestream, is_rel_site, "is_rel_site" + std::to_string(i+1), "char")) err++;
                   
                   if (!(is_rel_site == 'y' || is_rel_site == 'n')) 
                   {
                       std::cerr << "Error: the parameter is_rel_site" << std::to_string(i+1) << " contains an invalid value. ";
                       std::cerr << "Allowed values are 'y' or 'n'." << std::endl;
                       err++;
                   } 
   
                   if (err == 0) {
                       temp_coords.push_back({x, y});
                       if (is_rel_site == 'y') {temp_rel_sites.push_back(i);}
                   }
               }
           }
           file.close();
   
           if (temp_coords.size() != model_params->area->num_pat) {
               std::cerr << "Error: the number of valid coordinates in the file does not match num_pat." << std::endl;
           }
           else {
               sites_coords = temp_coords;
               release_sites = temp_rel_sites;
           }   
       }
   }
   
   void Simulation::set_boundary_type(BoundaryType boundary) 
   {
       boundary_type = boundary;
   }
   
   void Simulation::set_dispersal_type(DispersalType disp)
   {
       disp_type = disp;
   }
   
   void Simulation::set_rainfall(const std::filesystem::path& filepath)
   {
       input_rainfall_params->rainfall.clear();
   
       if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
           std::cerr << "Invalid filename. To enter a filename, the file should be in the build directory. Otherwise, the filepath should be provided (either relative to 'build' or absolute)." << std::endl;
       }
       else {
           std::ifstream file(filepath);
           std::string line;
           std::vector<double> temp;
           if (file.is_open()) {
               for(int i=0; std::getline(file, line); ++i) {
                   std::stringstream linestream(line);
                   if (line.size() == 0) break;
   
                   double r_d;
                   int err = 0;
                   if (!read_and_validate_type(linestream, r_d, "rainfall_day" + std::to_string(i+1), "double")) err++;
                   if (!check_bounds("rainfall_day" + std::to_string(i+1), r_d, 0.0, true)) err++;
   
                   if (err == 0) {
                       temp.push_back(r_d);
                   }
               }
           }
           file.close();
   
           if (temp.size() == 365 || temp.size() == max_t) {
               input_rainfall_params->rainfall = temp;
           }
           else {
               std::cerr << "Error: the number of valid daily rainfall values in the file is not 365 or max_t." << std::endl;
           }   
       }
   }
   
   void Simulation::set_release_times(const std::filesystem::path& filepath) 
   {
       if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
           std::cerr << "Invalid filename. To enter a filename, the file should be in the build directory. Otherwise, the filepath should be provided (either relative to 'build' or absolute)." << std::endl;
       }
   
       else {
           std::ifstream file(filepath);
           std::string line;
           std::vector<int> temp;
           int tot_err = 0;
           if (file.is_open()) {
               for(int i=0; std::getline(file, line); ++i) {
                   std::stringstream linestream(line);
                   if (line.size() == 0) break;
   
                   int r_d;
                   int err = 0;
                   if (!read_and_validate_type(linestream, r_d, "release_day" + std::to_string(i+1), "int")) err++;
                   if (!check_bounds("release_day" + std::to_string(i+1), r_d, 0, true, max_t, true)) err++;
   
                   if (err == 0) {
                       temp.push_back(r_d);
                   }
                   else tot_err += 1;
               }
           }
           file.close();
   
           if (tot_err == 0) {
               model_params->rel->release_times = temp;
           }
           else {
               std::cerr << "There were errors in the file. The simulation will run with rel_times = driver_start" << std::endl;
           }   
       }
   }
   
   void Simulation::set_inheritance(InheritanceParams inher_params)
   {
       double gamma = inher_params.gamma;
       double xi = inher_params.xi;
       double e = inher_params.e;
   
       // fraction of genotypes with index 0: ww, 1: wd, 2: dd, 3: wr, 4: rr, 5: dr
       std::array<double, 6> f_ww_ww = {1, 0, 0, 0, 0, 0};
       std::array<double, 6> f_ww_wd = {(1 - e - gamma) * 0.5, (1 + e) * 0.5, 0, gamma * 0.5, 0, 0};
       std::array<double, 6> f_ww_dd = {0, 1, 0, 0, 0, 0};
       std::array<double, 6> f_ww_wr = {0.5, 0, 0, 0.5, 0, 0};
       std::array<double, 6> f_ww_rr = {0, 0, 0, 1, 0, 0};
       std::array<double, 6> f_ww_dr = {0, 0.5, 0, 0.5, 0, 0};
   
       std::array<double, 6> f_wd_ww = {(1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*(1 + e)*0.5, 0, (1 - xi)*gamma*0.5, 0, 0};
       std::array<double, 6> f_wd_wd = {(1 - xi)*(1 - e - gamma)*(1 - e - gamma)* 0.25, (1 - xi)*(1 - e - gamma)*(1 + e)*0.5, (1 - xi)*(1 + e)*(1 + e)*0.25, (1 - xi)*(1 - e - gamma)*gamma*0.5, (1 - xi)*gamma*gamma*0.25, (1 - xi)*(1 + e)*gamma*0.5};
       std::array<double, 6> f_wd_dd = {0, (1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*(1 + e)*0.5, 0, 0, (1-xi)*gamma*0.5};
       std::array<double, 6> f_wd_wr = {(1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*(1 + e)*0.25, 0, (1 - xi)*((1 - e - gamma)*0.25 + (gamma * 0.25)), (1 - xi)*gamma*0.25, (1 - xi)*(1 + e)*0.25};
       std::array<double, 6> f_wd_rr = {0, 0, 0, (1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*gamma*0.5, (1 - xi)*(1 + e)*0.5};
       std::array<double, 6> f_wd_dr = {0, (1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*(1 + e)*0.25, (1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*gamma*0.25, (1 - xi)*((1 + e)*0.25 + gamma*0.25)};
       
       std::array<double, 6> f_dd_ww = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dd_wd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dd_dd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dd_wr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dd_rr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dd_dr = {0, 0, 0, 0, 0, 0};
   
       std::array<double, 6> f_wr_ww = {0.5, 0, 0, 0.5, 0, 0};
       std::array<double, 6> f_wr_wd = {(1 - e - gamma)*0.25, (1 + e)*0.25, 0, (gamma * 0.25 + (1 - e - gamma) * 0.25), gamma*0.25, (1 + e)*0.25};
       std::array<double, 6> f_wr_dd = {0, 0.5, 0, 0, 0, 0.5};
       std::array<double, 6> f_wr_wr = {0.25, 0, 0, 0.5, 0.25, 0};
       std::array<double, 6> f_wr_rr = {0, 0, 0, 0.5, 0.5, 0};
       std::array<double, 6> f_wr_dr = {0, 0.25, 0, 0.25, 0.25, 0.25};
   
       std::array<double, 6> f_rr_ww = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_rr_wd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_rr_dd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_rr_wr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_rr_rr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_rr_dr = {0, 0, 0, 0, 0, 0};
   
       std::array<double, 6> f_dr_ww = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dr_wd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dr_dd = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dr_wr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dr_rr = {0, 0, 0, 0, 0, 0};
       std::array<double, 6> f_dr_dr = {0, 0, 0, 0, 0, 0};
   
       for (int k=0; k<6; ++k) {
           for (int i=0; i<6; ++i) {
               for (int j=0; j<6; ++j) {
                   if (i==0) {
                       if (j==0) inher_fraction[i][j][k] = f_ww_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_ww_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_ww_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_ww_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_ww_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_ww_dr[k];
                   }
                   else if (i==1) {
                       if (j==0) inher_fraction[i][j][k] = f_wd_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_wd_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_wd_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_wd_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_wd_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_wd_dr[k];
                   }
                   else if (i==2) {
                       if (j==0) inher_fraction[i][j][k] = f_dd_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_dd_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_dd_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_dd_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_dd_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_dd_dr[k];
                   }
                   else if (i==3) {
                       if (j==0) inher_fraction[i][j][k] = f_wr_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_wr_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_wr_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_wr_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_wr_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_wr_dr[k];
                   }
                   else if (i==4) {
                       if (j==0) inher_fraction[i][j][k] = f_rr_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_rr_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_rr_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_rr_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_rr_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_rr_dr[k];
                   }
                   else if (i==5) {
                       if (j==0) inher_fraction[i][j][k] = f_dr_ww[k];
                       else if (j==1) inher_fraction[i][j][k] = f_dr_wd[k];
                       else if (j==2) inher_fraction[i][j][k] = f_dr_dd[k];
                       else if (j==3) inher_fraction[i][j][k] = f_dr_wr[k];
                       else if (j==4) inher_fraction[i][j][k] = f_dr_rr[k];
                       else if (j==5) inher_fraction[i][j][k] = f_dr_dr[k];
                   }
               }
           }
       }   
   }
   
   void Simulation::run_reps() 
   {
       for (int rep=1; rep <= num_runs; ++rep) {
           Model* model;
           if (!((input_rainfall_params->rainfall).empty())) {
               model = new Model(model_params, inher_fraction, input_rainfall_params, alpha0_mean, alpha0_variance, release_sites, boundary_type, disp_type, sites_coords);
           }
           else {
               model = new Model(model_params, inher_fraction, sine_rainfall_params, alpha0_mean, alpha0_variance, release_sites, boundary_type, disp_type, sites_coords);
           }
           Record data(rec_params, rep);
           model->initiate();
           data.record_coords(model->get_sites());
   
           for (int tt=0; tt <= max_t; ++tt) { // current day of the simulation 
               model->run(tt);
   
               if (data.is_rec_global_time(tt)) {
                   data.output_totals(tt, model->calculate_tot_J(), model->calculate_tot_M(), model->calculate_tot_V(),
                    model->calculate_tot_F());
                   data.record_global(tt, model->calculate_tot_M_gen());
               }
               if (data.is_rec_local_time(tt)) {
                   data.record_local(tt, model->get_sites());
               }
           }
   
           delete model;
       }
   }
