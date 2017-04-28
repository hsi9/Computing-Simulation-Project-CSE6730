import h5py
import numpy as np
import random, math, sys

def compute_energies(traj_file):
    h5traj = h5py.File(traj_file, "r")
    traj = h5traj.get("trajectory")

    for step in sorted(traj.keys()):
        rvf = traj.get("{}/rvf".format(step))
        positions = rvf[:,0:3]

        print("ITEM: TIMESTEP\n{}".format(int(step)))
        print("ITEM: BOX BOUNDS\n{0} {1}\n{0} {1}\n{0} {1}".format(0, 60.1))
        print("ITEM: NUMBER OF ATOMS\n{}".format(positions.shape[0]))
        print("ITEM: ATOMS id x y z")

        for i in range(positions.shape[0]):
            x, y, z = positions[i][0], positions[i][1], positions[i][2]
            print("{} {} {} {}".format(i, x, y, z))

def main():
    compute_energies(sys.argv[1])

if __name__ == '__main__':
    main()
