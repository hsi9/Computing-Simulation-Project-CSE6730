import os, sys, getopt, yaml
import h5py
import numpy as np
import Bio.PDB as bp

def write_trajectory(coords, h5file, path, compression=None):
	num_atoms = len(coords)
	if num_atoms < 1:
		return
	group = h5file.create_group(path)
	positions_dset = group.create_dataset("rvf", (num_atoms, 9), dtype='d', compression=compression)
	traj = np.zeros((num_atoms,9))
	traj[:,0:3] = np.array(coords)
	positions_dset[...] = traj

def get_random_position(bounds):
	return [np.random.uniform(bounds[0],bounds[1]),
			np.random.uniform(bounds[2],bounds[3]),
			np.random.uniform(bounds[4],bounds[5])]

def get_coordinates(pdbfile):
	parser = bp.PDBParser(QUIET=True)
	structure = parser.get_structure('x', pdbfile)
	atoms = structure.get_atoms()
	coords = []
	for atom in atoms:
		coords.append(atom.get_coord())
	return coords
	
def readlinec(f):
	line = f.readline()
	while line:
		if line[0] != ';': return line
		line = f.readline()
	return ''

def seek_in_file(f,header,molecule):
	f.seek(0)
	line = ' '
	while line:
		if header in line:
			line = f.readline()
			if molecule in line:
				return readlinec(f)
		line = f.readline()
	return ''
	
def read_atoms(filename, moleculeName):
	file = open(filename,"r")
	line = seek_in_file(file, "ATOM", moleculeName)
	
	atoms = []
	while len(line) > 1:
		listOfStrings = line.replace(';',' ;').split()
		vals = [float(s) for s in listOfStrings[5:8]]
		
		line = readlinec(file)
		while '<' == line[0] or ' ' == line[0]:
			listOfStrings = line.replace('<',' ').split()
			if '.' in line:
				# intvals = [int(s) for s in listOfStrings[0:4]]
				# floatvals = [float(s) for s in listOfStrings[4:]]
				line = readlinec(file)
				break
			else:
				# intvals = [int(s) for s in listOfStrings]
				line = readlinec(file)
		atom = {'mass':vals[0], 'radius':vals[1], 'charge':vals[2]}
		atoms.append(atom)
	file.close()
	return atoms

def read_bonds(filename, moleculeName, ao=0):
	file = open(filename,"r")
	line = seek_in_file(file, "BOND", moleculeName)
	bonds = []
	while len(line) > 1:
		listOfStrings = line.replace(';',' ;').split()
		c = [int(s) for s in listOfStrings[0:2]]
		v = [float(s) for s in listOfStrings[2:4]]
		bond = {'i':c[0]+ao, 'j':c[1]+ao, 'r0':v[0], 'konst':v[1]}
		bonds.append(bond)
		line = readlinec(file)
	file.close()
	return bonds

def read_angles(filename, moleculeName, ao=0):
	file = open(filename,"r")
	line = seek_in_file(file, "ANGLES", moleculeName)
	angles = []
	while len(line) > 1:
		listOfStrings = line.replace(';',' ;').split()
		c = [int(s) for s in listOfStrings[0:3]]
		v = [float(s) for s in listOfStrings[3:5]]
		angle = {'i':c[0]+ao, 'j':c[1]+ao, 'k':c[2]+ao, 'theta0':v[0], 'konst':v[1]}
		angles.append(angle)
		line = readlinec(file)
	file.close()
	return angles
	
def read_torsions(filename, header, moleculeName, ao=0):
	file = open(filename,"r")
	line = seek_in_file(file, header, moleculeName)
	torsions = []
	while len(line) > 1:
		listOfStrings = line.replace(';',' ;').split()
		c = [int(s) for s in listOfStrings[0:4]]
		k = float(listOfStrings[4])
		m = [int(s) for s in listOfStrings[5:7]]
		p = float(listOfStrings[7])
		v = [int(s) for s in listOfStrings[8]]
		torsion = {'i':c[0]+ao, 'j':c[1]+ao, 'k':c[2]+ao, 'l':c[2]+ao, 'phi0':p, 'konst':k, 'multiplicity':m[0]}
		torsions.append(torsion)
		line = readlinec(file)
	file.close()
	return torsions

def atoms_to_array(atoms):
	data = []
	for i, atom in enumerate(atoms):
		data.append((i,atom['mass'],atom['radius'],atom['charge'],0.0)) # I don't know what q is.
	return data
	
def bonds_to_array(bonds):
	data = []
	for i, bond in enumerate(bonds):
		data.append((i,bond['i'],bond['j'],bond['r0'],bond['konst']))
	return data
		
def angles_to_array(angles):
	data = []
	for i, angle in enumerate(angles):
		data.append((i,angle['i'],angle['j'],angle['k'],angle['theta0'],angle['konst']))
	return data
	
def torsions_to_array(torsions):
	data = []
	for i, torsion in enumerate(torsions):
		data.append((i,torsion['i'],torsion['j'],torsion['k'],torsion['l'],torsion['phi0'],torsion['konst'],torsion['multiplicity']))
	return data
	
def write_atoms(atoms, h5file, path, compression=None): #(tplfilename, moleculeName, h5file, path, compression=None):
	num_atoms = len(atoms)
	if num_atoms < 1:
		return
	
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
	bonds_dset[...] = np.array(atoms_to_array(atoms), dtype = compound_type)
	
def write_bonds(bonds, h5file, path, compression=None): # (tplfilename, moleculeName, h5file, path, compression=None):
	num_bonds = len(bonds)
	if num_bonds < 1:
		return
	
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
	bonds_dset[...] = np.array(bonds_to_array(bonds), dtype = compound_type)

def write_angles(angles, h5file, path, compression=None): # tplfilename, moleculeName, h5file, path, compression=None):
	num_angles = len(angles)
	if num_angles < 1:
		return
	
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
	angles_dset[...] = np.array(angles_to_array(angles), dtype = compound_type)

def write_torsions(torsions, h5file, path, dsetname, compression=None): # (tplfilename, moleculeName, h5file, path, dsetname, header, compression=None):
	num_torsions = len(torsions)
	if num_torsions < 1:
		return
	
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
	
	torsions_dset = group.create_dataset(dsetname, (num_torsions,), dtype=compound_type, compression=compression)
	torsions_dset[...] = np.array(torsions_to_array(torsions), dtype = compound_type)
	
def write_h5file(config):
	h5file = h5py.File(config['outputFilename'], "w")
	dataDir = config['dataDir'];
	atomCount = 0
	atoms, bonds, angles, dihedrals, impropers, allcoordinates = [], [], [], [], [], []
	for molecule in config['molecules']:
		pdbfilename = dataDir + os.sep + molecule['trajectoryFile']
		tplfilename = dataDir + os.sep + molecule['topologyFile']
		moleculeName = molecule['name']
		num_molecules = molecule['nMolecules']
		
		# coordinates
		coordinates = get_coordinates(pdbfilename)
		num_atoms = len(coordinates)
		coordinates = np.tile(coordinates,(num_molecules,1))
		
		if molecule['placement'] == 'FIXED':
			offset = molecule['offset']
			coordinates = np.add(coordinates, np.repeat([offset],len(coordinates),axis=0))
		elif molecule['placement'] == 'RANDOM':
			bounds = molecule['boundaries']
			offsets = []
			for i in range(num_molecules):
				offsets.append(get_random_position(bounds))
			coordinates = np.add(coordinates, np.repeat(offsets,num_atoms,axis=0))
		if len(allcoordinates) > 0:
			allcoordinates = np.concatenate((allcoordinates,coordinates))
		else:
			allcoordinates = coordinates
		
		for i in range(num_molecules):
			atoms = atoms + read_atoms(tplfilename, moleculeName)
			bonds = bonds + read_bonds(tplfilename, moleculeName, ao=atomCount)
			angles = angles + read_angles(tplfilename, moleculeName, ao=atomCount)
			dihedrals = dihedrals + read_torsions(tplfilename, "TORSIONS", moleculeName, ao=atomCount)
			impropers = impropers + read_torsions(tplfilename, "IMPROPER-TORSIONS", moleculeName, ao=atomCount)
			atomCount = atomCount + num_atoms

	compression = None # 'gzip'
	
	write_trajectory(allcoordinates, h5file, "trajectory/0000", compression)
	write_atoms(atoms, h5file, "topology" , compression)
	write_bonds(bonds, h5file, "topology" , compression)
	write_angles(angles, h5file, "topology" , compression)
	write_torsions(dihedrals, h5file, "topology" , "dihedrals", compression)
	write_torsions(impropers, h5file, "topology" , "impropers", compression)

def main(argv):
	configfile = ''
	try:
		opts, args = getopt.getopt(argv,"hc:",["config="])
	except getopt.GetoptError:
		print 'topology_builder.py -c <configfile>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'topology_builder.py -c <configfile>'
			sys.exit()
		elif opt in ("-c"):
			configfile = arg
	if len(configfile) <= 1:
		print 'topology_builder.py -c <configfile>'
	else:
		stream = file(configfile,'r')
		config = yaml.load(stream)
		# print config
		write_h5file(config)

if __name__ == '__main__':
	main(sys.argv[1:])