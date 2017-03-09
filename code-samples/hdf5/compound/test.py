import h5py
import numpy as np
import random

def generate_bonds(h5file, path, num_bonds, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, 10000), random.randint(0, 10000),
                         random.uniform(0, 1000), random.uniform(0, 1000), random.uniform(0, 1000)))
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
        ('alpha', 'd'),
        ('beta', 'd'),
        ('gamma', 'd')
    ])

    bonds_dset = group.create_dataset("bonds", (num_bonds,), dtype=compound_type, compression=compression)
    bonds_dset[...] = np.array(generate_random_data(num_bonds), dtype = compound_type)


def generate_angles(h5file, path, num_angles, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, 10000), random.randint(0, 10000), random.randint(0, 10000),
                         random.uniform(0, 1000), random.uniform(0, 1000),
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
        ('k', 'l'),
        # use double instead of float
        ('alpha', 'd'),
        ('beta', 'd'),
        ('gamma', 'd'),
        ('delta', 'd')
    ])

    angles_dset = group.create_dataset("angles", (num_angles,), dtype=compound_type, compression=compression)
    angles_dset[...] = np.array(generate_random_data(num_angles), dtype = compound_type)


def generate_torsions(h5file, path, dsetname, num_angles, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.randint(0, 10000), random.randint(0, 10000),
                         random.randint(0, 10000), random.randint(0, 10000),
                         random.uniform(0, 1000), random.uniform(0, 1000), random.uniform(0, 1000),
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
        ('k', 'l'),
        ('l', 'l'),
        # use double instead of float
        ('alpha', 'd'),
        ('beta', 'd'),
        ('gamma', 'd'),
        ('delta', 'd'),
        ('epsilon', 'd')
    ])

    torsions_dset = group.create_dataset(dsetname, (num_angles,), dtype=compound_type, compression=compression)
    torsions_dset[...] = np.array(generate_random_data(num_angles), dtype = compound_type)


def generate_trajectory(h5file, path, num_atoms, compression=None):
    group = h5file.create_group(path)
    ids_dset = group.create_dataset("gids", (num_atoms, 1), dtype='l', compression=compression)
    ids_dset[...] = np.random.uniform(1000, size=(num_atoms, 1))

    positions_dset = group.create_dataset("positions", (num_atoms, 3), dtype='d', compression=compression)
    positions_dset[...] = np.random.uniform(1000, size=(num_atoms, 3))


def generate_trajectory2(h5file, path, num_atoms, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((i, random.uniform(0, 1000), random.uniform(0, 1000), random.uniform(0, 1000)))
        return data

    compound_type = np.dtype([
        # use long instead of int
        ('gid', 'l'),
        # use double instead of float
        ('x', 'd'),
        ('y', 'd'),
        ('z', 'd')
    ])

    group = h5file.create_group(path)
    positions_dset = group.create_dataset("positions", (num_atoms,), dtype=compound_type, compression=compression)
    positions_dset[...] = np.array(generate_random_data(num_atoms), dtype = compound_type)


def create_h5file():
    h5file = h5py.File("compound.h5", "w")

    compression = 'gzip'

    # NOTE: When we increase the number of bonds, angles, torsions, atoms by 100x
    # (10M atoms is reasonable), the size of the snapshot is increased by ~40MB
    # If we remove all topology info and include only the trajectory, then the
    # file size diff is ~5MB (for 10000000 atoms)

    generate_bonds(h5file, "topology", 10000, compression)
    generate_angles(h5file, "topology", 8000, compression)
    generate_torsions(h5file, "topology", "dihedrals", 8000, compression)
    generate_torsions(h5file, "topology", "impropers", 8000, compression)
    generate_trajectory2(h5file, "trajectory/0000", 100000, compression)

def main():
    create_h5file()

if __name__ == '__main__':
    main()
