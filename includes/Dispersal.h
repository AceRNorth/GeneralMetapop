#ifndef GENERALMETAPOP_DISPERSAL_H
#define GENERALMETAPOP_DISPERSAL_H

#include <vector>
#include <array>
#include <utility>
#include "constants.h"
#include "Params.h"
#include "Patch.h"
#include "Point.h"
#include "BoundaryStrategy.h"

class Patch;

/**
 * Dispersal base class. Defines the interface for dispersal classes. 
 * @details The Dispersal classes compute the connections between Patch objects with their respective connection weights, dependent on the maximum dispersal distance ``max_disp``, and carry out dispersal in and out of the patches, dependent on the dispersal rate ``disp_rate``. 
 * @see DispersalParams
 */
class Dispersal {
public:
	Dispersal(DispersalParams* params, BoundaryType boundary, double side_x, double side_y);
	virtual ~Dispersal() {};
	virtual void set_connecs(std::vector<Patch*> &sites) = 0;
	virtual void adults_disperse(std::vector<Patch*> &sites) = 0;

protected:
	double disp_rate; /**< Adult dispersal rate. */ 
	double max_disp; /**< Maximum distance at which two sites are connected. */ 

	std::vector<std::vector<int>> connec_indices; /**< Connected patch indices ordered by each patch in Model::sites, such that the first element contains the indices of all the patches connected to the first sites patch, etc. */
	std::vector<std::vector<double>> connec_weights; /**< Connection weights of the connected patches ordered by each patch in Model::sites, such that the first element contains the connection weights between the first patch in sites and all the patches connected to it, etc. */

	BoundaryStrategy* boundary_strategy;

	std::vector<std::array<long long int, constants::num_gen>> M_dispersing_out(const std::vector<Patch*> &sites);
	std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> F_dispersing_out(const std::vector<Patch*> &sites);
};

/**
 * Implements dispersive behaviour in the model for simple dispersal, where connection weights between patches are defined by a distance kernel. 
 * @details All dispersing individuals are assumed to survive dispersal, and are guaranteed a connected patch to disperse to. 
 */
class DistanceKernelDispersal: public Dispersal {
public:
	/**
	 * DistanceKernelDispersal constructor.
	 * @param[in] params 	dispersal parameters
	 * @param[in] boundary 	boundary type to use for calculating dispersal distances
 	 * @param[in] side_x 	size of one side of the simulation area (x-axis)
	 * @param[in] side_y 	size of one side of the simulation area (y-axis)
	 */
	DistanceKernelDispersal(DispersalParams* params, BoundaryType boundary, double side_x, double side_y): Dispersal(params, boundary, side_x, side_y) {};
	~DistanceKernelDispersal();
	void set_connecs(std::vector<Patch*> &sites) override;
	void adults_disperse(std::vector<Patch*> &sites) override;

private:
	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
};

/**
 * Implements dispersive behaviour for radial dispersal, where connection weights between patches are determined by the direction the patches are in relative to each other and other patches. 
 * @details Particularly, the connection weight of a focal patch to its neighbouring patch is determined by the angle of bisecting lines from the centre of the focal patch to the catchment of the receiving patch. More distant villages may also be directly connected but the connectivity will be reduced if there are closer villages along the same flight path. The individuals that disperse in an unconnected direction will die. 
 * @image xml radial_disp_diagram.png "Radial dispersal diagram"
 */
class RadialDispersal: public Dispersal {
public:
	RadialDispersal(DispersalParams* params, BoundaryType boundary, double side_x, double side_y);
	~RadialDispersal();
	void set_connecs(std::vector<Patch*> &sites) override;
	void adults_disperse(std::vector<Patch*> &sites) override;

private:
	std::vector<double> connec_weights_sum; /**< Sum of all the connection weights for each patch, ordered by Model::sites.*/

	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
	std::pair<std::vector<std::pair<double, double>>, double> compute_interval_union(const std::pair<double, double>& qq,
        const std::vector<std::pair<double, double>>& input);
	double wrap_around(double value, double range); 
	std::vector<int> get_sorted_positions(const std::vector<double>& numbers);
	std::pair<std::vector<double>,std::vector<int>> compute_distances_site(int,std::vector<Patch*> &sites);
};

#endif //GENERALMETAPOP_DISPERSAL_H
