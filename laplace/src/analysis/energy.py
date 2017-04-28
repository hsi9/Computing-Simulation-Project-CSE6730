import h5py
import numpy as np
import random, math, sys

def compute_energies(topo_file, traj_file):
    h5topo = h5py.File(topo_file, "r")
    masses = np.array([ a[1] for a in  h5topo.get("topology/atoms")[:] ])

    h5traj = h5py.File(traj_file, "r")
    traj = h5traj.get("trajectory")

    for step in sorted(traj.keys()):
        rvf = traj.get("{}/rvf".format(step))
        velocities = rvf[:,3:6]

        energy = 0
        for i in range(velocities.shape[0]):
            x, y, z = velocities[i][0], velocities[i][1], velocities[i][2]
            energy += 0.5 * masses[i] * (x*x + y*y + z*z)

        print("{}: {}".format(step, energy))

def main():
    compute_energies(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
    main()
