import h5py
import numpy as np
import random, math

def generate_atoms(h5file, path, num_atoms, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.uniform(1, 5), random.uniform(0.5, 1.5),
                         random.uniform(0.5, 1.5), random.uniform(-1, 1)))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        # use double instead of float
        ('mass', 'd'),
        ('sigma', 'd'),
        ('epsilon', 'd'),
        ('q', 'd'),
    ])

    bonds_dset = group.create_dataset("atoms", (num_atoms,), dtype=compound_type, compression=compression)
    bonds_dset[...] = np.array(generate_random_data(num_atoms), dtype = compound_type)

def generate_bonds(h5file, path, num_atoms, num_bonds, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, num_atoms-1), random.randint(0, num_atoms-1),
                         random.uniform(0, 10), random.uniform(0, 10)))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        ('i', 'l'),
        ('j', 'l'),
        # use double instead of float
        ('r0', 'd'),
        ('konst', 'd')
    ])

    bonds_dset = group.create_dataset("bonds", (num_bonds,), dtype=compound_type, compression=compression)
    bonds_dset[...] = np.array(generate_random_data(num_bonds), dtype = compound_type)


def generate_angles(h5file, path, num_atoms, num_angles, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, num_atoms-1), random.randint(0, num_atoms-1), random.randint(0, num_atoms-1),
                         random.uniform(0, math.pi), random.uniform(0, 10)))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        ('i', 'l'),
        ('j', 'l'),
        ('k', 'l'),
        # use double instead of float
        ('theta0', 'd'),
        ('konst', 'd')
    ])

    angles_dset = group.create_dataset("angles", (num_angles,), dtype=compound_type, compression=compression)
    angles_dset[...] = np.array(generate_random_data(num_angles), dtype = compound_type)


def generate_torsions(h5file, path, dsetname, num_atoms, num_angles, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, num_atoms-1), random.randint(0, num_atoms-1),
                         random.randint(0, num_atoms-1), random.randint(0, num_atoms-1),
                         random.uniform(0, math.pi), random.uniform(0, 10), random.randint(0, 3)))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        ('i', 'l'),
        ('j', 'l'),
        ('k', 'l'),
        ('l', 'l'),
        # use double instead of float
        ('phi0', 'd'),
        ('konst', 'd'),
        ('multiplicity', 'i')
    ])

    torsions_dset = group.create_dataset(dsetname, (num_angles,), dtype=compound_type, compression=compression)
    torsions_dset[...] = np.array(generate_random_data(num_angles), dtype = compound_type)

def generate_nonbonded14(h5file, path, num_atoms, num_nonbonded, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, num_atoms-1), random.randint(0, num_atoms-1),
                         random.uniform(0, 1000), random.uniform(0, 1000)))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        ('i', 'l'),
        ('j', 'l'),
        # use double instead of float
        ('coeff_vdw', 'd'),
        ('coeff_ele', 'd'),
    ])

    torsions_dset = group.create_dataset("nonbonded14s", (num_nonbonded,), dtype=compound_type, compression=compression)
    torsions_dset[...] = np.array(generate_random_data(num_nonbonded), dtype = compound_type)


def generate_trajectory(h5file, path, num_atoms, compression=None):
    group = h5file.create_group(path)
    positions_dset = group.create_dataset("rvf", (num_atoms, 9), dtype='d', compression=compression)
    traj = np.random.uniform(5, size=(num_atoms, 9))
    traj[:,0:3] = np.random.uniform(60.1, size=(num_atoms, 3))
    traj[:,3:6] = np.random.normal(0, 2, size=(num_atoms, 3))
    traj[:,6:9] = np.random.normal(0, 1, size=(num_atoms, 3))
    positions_dset[...] = traj


def create_h5file():
    h5file = h5py.File("random_topology.h5", "w")

    # disable for now
    compression = None # 'gzip'

    # NOTE: When we increase the number of bonds, angles, torsions, atoms by 100x
    # (10M atoms is reasonable), the size of the snapshot is increased by ~40MB
    # If we remove all topology info and include only the trajectory, then the
    # file size diff is ~5MB (for 10000000 atoms)
    topologyDir = "topology"
    numAtoms = 1000
    generate_atoms(h5file, topologyDir, numAtoms, compression)
    generate_bonds(h5file, topologyDir, numAtoms, 100, compression)
    generate_angles(h5file, topologyDir, numAtoms, 80, compression)
    generate_torsions(h5file, topologyDir, "dihedrals", numAtoms, 80, compression)
    generate_torsions(h5file, topologyDir, "impropers", numAtoms, 80, compression)
    generate_nonbonded14(h5file, topologyDir, numAtoms, 3, compression)
    generate_trajectory(h5file, "trajectory/0000", numAtoms, compression)
    generate_trajectory(h5file, "trajectory/0001", numAtoms, compression)
    generate_trajectory(h5file, "trajectory/0002", numAtoms, compression)

def main():
    create_h5file()

if __name__ == '__main__':
    main()
