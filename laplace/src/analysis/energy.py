import h5py
import numpy as np
import random, math, sys

def compute_energies(topo_file, traj_file):
    h5topo = h5py.File(topo_file, "r")
    masses = np.array([ a[1] for a in  h5topo.get("topology/atoms")[:] ])

    h5traj = h5py.File(traj_file, "r")
    traj = h5traj.get("trajectory")

    perform_binning = True
    spike_at = -1
    for step in sorted(traj.keys()):
        rvf = traj.get("{}/rvf".format(step))
        velocities = rvf[:,3:6]

        total_kinetic_energy = 0
        bins = [0,0,0,0,0]
        partitions = [1e-10, 1e-5, 1e-3, 1, 1e3]
        for i in range(velocities.shape[0]):
            x, y, z = velocities[i][0], velocities[i][1], velocities[i][2]
            particle_kinetic_energy = 0.5 * masses[i] * (x*x + y*y + z*z)
            total_kinetic_energy += particle_kinetic_energy
            if perform_binning:
                binned = False
                new_bins = []
                for bucket, border in zip(bins, partitions):
                    if particle_kinetic_energy < border:
                        bucket += 1
                        binned = True
                    new_bins.append(bucket)
                if not binned:
                    if spike_at == -1:
                        spike_at = step
                bins = new_bins

        print("{}: {}".format(step, total_kinetic_energy))

    if perform_binning:
        print("BINS: {}".format(bins))
        print("SPIKED: {}".format(spike_at))

def main():
    compute_energies(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
    main()
