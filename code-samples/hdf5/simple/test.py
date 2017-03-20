import h5py
import numpy as np


def generate_bonds_dsets(h5file, path, num_bonds, compression=None):
    group = h5file.create_group(path)

    gids_dset = group.create_dataset("gids", (num_bonds,), dtype='i', compression=compression)
    gids_dset[...] = np.array([ i for i in range(num_bonds) ], dtype='i')

    ids_dset = group.create_dataset("ids", (num_bonds, 2), dtype='i', compression=compression)
    ids_dset[...] = np.random.randint(1000, size=(num_bonds, 2))
    ids_dset.attrs['column_names'] = [b'i',b'j']

    params_dset = group.create_dataset("params", (num_bonds,3), dtype='d', compression=compression)
    params_dset[...] = np.random.uniform(1000, size=(num_bonds, 3))
    params_dset.attrs['column_names'] = [b'alpha',b'beta',b'gamma']

def generate_angles_dsets(h5file, path, num_angles, compression=None):
    group = h5file.create_group(path)

    gids_dset = group.create_dataset("gids", (num_angles,), dtype='i', compression=compression)
    gids_dset[...] = np.array([ i for i in range(num_angles) ], dtype='i')

    ids_dset = group.create_dataset("ids", (num_angles, 3), dtype='i', compression=compression)
    ids_dset[...] = np.random.randint(1000, size=(num_angles, 3))
    ids_dset.attrs['column_names'] = [b'i',b'j',b'k']

    params_dset = group.create_dataset("params", (num_angles,3), dtype='d', compression=compression)
    params_dset[...] = np.random.uniform(1000, size=(num_angles, 3))
    params_dset.attrs['column_names'] = [b'alpha',b'beta',b'gamma',b'delta']

def generate_dihedral_dsets(h5file, path, num_dihedrals, compression=None):
    group = h5file.create_group(path)

    gids_dset = group.create_dataset("gids", (num_dihedrals,), dtype='i', compression=compression)
    gids_dset[...] = np.array([ i for i in range(num_dihedrals) ], dtype='i')

    ids_dset = group.create_dataset("ids", (num_dihedrals, 4), dtype='i', compression=compression)
    ids_dset[...] = np.random.randint(1000, size=(num_dihedrals, 4))
    ids_dset.attrs['column_names'] = [b'i',b'j',b'k',b'l']

    params_dset = group.create_dataset("params", (num_dihedrals, 4), dtype='d', compression=compression)
    params_dset[...] = np.random.uniform(1000, size=(num_dihedrals, 4))
    params_dset.attrs['column_names'] = [b'alpha',b'beta',b'gamma',b'delta',b'epsilon']


def generate_trajectory(h5file, path, num_atoms, compression=None):
    group = h5file.create_group(path)
    ids_dset = group.create_dataset("ids", (num_atoms, 1), dtype='i', compression=compression)
    ids_dset[...] = np.random.uniform(1000, size=(num_atoms, 1))

    positions_dset = group.create_dataset("positions", (num_atoms, 3), dtype='d', compression=compression)
    positions_dset[...] = np.random.uniform(1000, size=(num_atoms, 3))

def create_h5file():
    h5file = h5py.File("simple.h5", "w")

    compression = 'gzip'
    generate_bonds_dsets(h5file, "topology/bonds", 10000, compression)
    generate_angles_dsets(h5file, "topology/angles", 8000, compression)
    generate_dihedral_dsets(h5file, "topology/dihedrals", 8000, compression)
    generate_dihedral_dsets(h5file, "topology/impropers", 8000, compression)
    generate_trajectory(h5file, "trajectory/0000", 100000, compression)


def main():
    create_h5file()

if __name__ == '__main__':
    main()
