#include "laplace/core/md.h"
#include "laplace/core/neighbor_search.h"
#include "laplace/core/nonbonded_sr.h"
#include "laplace/core/integration.h"
#include <fmt/format.h>

using namespace laplace;
using namespace std;

void laplace::run_md(H5::H5File &outfile,
                     MmSystem &system,
                     const config::SimulationConfig &config) {

  // initialize temp data once - this is not necessary, but is done as optimization to avoid re-allocating memory each time
  vector<array<int, 2>> pairs;

  // allocate memory for the previous timestep's forces
  system.atoms.forces_old.resize(system.atoms.forces.size());

  // the cutoff is used as distance threshold, below which marks two atoms as interactable by the L-J potential
  // it must be larger than the rcut, so that we can optimize and not have to spend each timestep
  // performing the neighbor search
  auto cutoff2 = config.neighbor_search.rcut + config.neighbor_search.rshell;
  cutoff2 = cutoff2 * cutoff2;

  auto rcut2 = config.neighbor_search.rcut * config.neighbor_search.rcut;

  // initialize the 0.5 * dt / m component of the force calculation, which is constant throughout the simulation
  // this is purely an optimization for the integration, which has been set up to be runnable on SIMD
  vector<real> half_dt_mass_inv;
  for (const auto &m : system.atoms.masses) {
    auto x = 0.5 * config.run.dt / m;
    for (auto i=0; i < laplace::DIMS; i++)
      half_dt_mass_inv.emplace_back(x);
  }

  auto current_time = config.run.tinit;
  for (auto timestep=0; timestep < config.run.steps; timestep++, current_time += config.run.dt) {

    /*
      Recreate the neighbor lists at a certain interval.
      We don't need to run this at every timestep because the extra rshell distance
      allows atoms to travel a bit further outside rcut and still be counted for
      L-J potential calculations
    */
    if (timestep % config.neighbor_search.interval == 0) {
      laplace::perform_neighbor_search(
        pairs,
        system,
        config.box.L,
        config.box.celldims,
        cutoff2
      );
    }

    /*
      Update non-bonded forces according to the Lennard-Jones formulation
    */
    laplace::nonbonded_sr_forces_update(
      system,
      pairs,
      config.box.L,
      rcut2
    );

    /*
      INSERT BOND/ANGLE/DIHEDRAL FORCE CALCULATIONS HERE
    */

    /*
      Perform integration - compute velocities and positions at next time step.
      The function signature has been set up such that the underlying implementation
      can be re-done on SIMD
    */
    laplace::velocity_verlet_kernel(
      as_c_array(system.atoms.positions),
      as_c_array(system.atoms.velocities),
      as_c_array(system.atoms.forces),
      as_c_array(system.atoms.forces_old),
      &half_dt_mass_inv[0],
      system.atoms.positions.size() * laplace::DIMS,
      config.box.L,
      config.run.dt
    );

    /*
      Write snapshot of the current trajectory to output if needed
    */
    if (timestep % config.output.interval == 0) {
      fmt::print("STEP {:d}: WRITING TO FILE\n", timestep);
      system.atoms.write_trajectory_snapshot(outfile, timestep);
    }
  }
}
