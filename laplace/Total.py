# This is the script that I used to split the text file
# Below is the code needed to do the split operation
import re
import numpy as np  # http://www.numpy.org
import csv

def main():

	csv_file=open('atoms.txt')
	csv_reader=csv.reader(csv_file, delimiter=',')

	mass=[]
	for row in csv_file:
		massdata=re.split('{|, |}',row)
		mass.append(massdata[2])

# mmass is the array for the mass
	mmass=np.array(mass, dtype = float)
#	print mmass
#	print len(mmass)
	csv_file.close()

# establish energy array
	energy=[];

# name=np.zeros(shape=(50))

# Below is the trajectory calculation
	trajectory=np.zeros(shape=(100,50,4))
	print trajectory;

	str_name=list()
	i=1;
	j=1;
	k=1;
	for x in range(0, 50):
		name=str(x*20)
		with open(name) as file:
			data = file.read()

# Now we have the temporary variable aa
		aa=[]
# Below is the data
		mydata=re.split(', |,\n',data)
# then append temp to aa	
		for temp in mydata:
			temp.rstrip('\n')
			aa.append(temp)

		print ('length of aa')
		print (len(aa))
# aa is the array with 900 elements
		aa = np.array(aa, dtype = float)
		bb=aa.reshape(100,9);

# This is to calculate the energy
		for j in range(0,100):
			ienergy=0.5*mmass[j-1]*(bb[j-1][3]*bb[j-1][3]+bb[j-1][4]*bb[j-1][4]+bb[j-1][5]*bb[j-1][5])
			energy.append(ienergy)
# Below is the record for the trajectory and energy
			trajectory[j-1][x-1][0]=bb[j-1][0]
			trajectory[j-1][x-1][1]=bb[j-1][1]
			trajectory[j-1][x-1][2]=bb[j-1][2]

			trajectory[j-1][x-1][3]=ienergy; 

# This is the most important part of this script

		myenergy=np.array(energy, dtype = float)


		namee=name+" energy"+".csv";
		np.savetxt(namee, myenergy, delimiter=",")
#		print "length of myenergy"
#		print myenergy;

#		print name
		print len(aa)

#		print bb
		name1=name+".csv";
#		print len(bb)

		np.savetxt(name1, bb, delimiter=",")

	print "length of trajectory"
	print len(trajectory)
	print len(trajectory[0])
	print len(trajectory[0][1])

	for k in range(0,100):
		Finally="trajectory"+str(k)+".csv"
		np.savetxt(Finally, trajectory[k-1], delimiter=",")

if __name__ == "__main__":
    main()