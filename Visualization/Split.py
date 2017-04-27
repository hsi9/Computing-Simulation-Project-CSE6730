# This is the script that I used to split the text file
# Below is the code needed to do the split operation
import re
import numpy as np  # http://www.numpy.org

def main():

	name=np.zeros(shape=(50))
	str_name=list()
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

# aa is the array with 900 elements
		aa = np.array(aa, dtype = float)
		bb=aa.reshape(100,9);

		print name
		print len(aa)

	#print bb

		print len(bb)

		np.savetxt(name, bb, delimiter=",")

if __name__ == "__main__":
    main()
