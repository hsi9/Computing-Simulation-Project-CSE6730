import h5py, random, string
import numpy as np

def generate_airplanes(h5file, path, num_airplanes, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((
                i,
                ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(4)),
                ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(3)),
                random.uniform(0, 1000),
                random.uniform(0, 1000)
            ))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        ('id', 'i'),
        ('name', 'S10'),
        ('manufacturer', 'S10'),
        ('speed', 'd'),
        ('capacity', 'i'),
    ])

    airplanes_dset = group.create_dataset("table", (num_airplanes,), dtype=compound_type, compression=compression)
    airplanes_dset[...] = np.array(generate_random_data(num_airplanes), dtype = compound_type)

def generate_airports(h5file, path, num_airports, compression=None):
    def generate_random_data(n):
        data = []
        for i in range(n):
            data.append((
                i,
                ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(5)),
                ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(5)),
                ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(5)),
                ''.join(random.choice(string.ascii_uppercase) for _ in range(3)),
                ''.join(random.choice(string.ascii_uppercase) for _ in range(3)),
                random.uniform(0, 1000),
                random.uniform(0, 1000),
                random.uniform(0, 1000)
            ))
        return data

    if path not in h5file:
        group = h5file.create_group(path)
    else:
        group = h5file[path]

    compound_type = np.dtype([
        ('id', 'i'),
        ('name', 'S10'),
        ('city', 'S10'),
        ('country', 'S10'),
        ('iata', 'S10'),
        ('icao', 'S10'),
        ('latitude', 'd'),
        ('longitude', 'd'),
        ('altitude', 'd'),
    ])

    airports_dset = group.create_dataset("table", (num_airports,), dtype=compound_type, compression=compression)
    airports_dset[...] = np.array(generate_random_data(num_airports), dtype = compound_type)

def create_h5file():
    h5file = h5py.File("airportsim.h5", "w")
    generate_airplanes(h5file, "airplanes", 43, None)
    generate_airports(h5file, "airports", 13, None)


def main():
    create_h5file()

if __name__ == '__main__':
    main()
