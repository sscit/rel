import os
import random
import string

global unique_number

def write_content(file_ptr):
	print("Writing content into file")
	global unique_number
	
	for j in range(1, random.randint(1580, 2660)):
		file_ptr.write("SystemRequirement {\n")
		uid = "uid" + str(unique_number)
		file_ptr.write("  Identifier : " + uid + ",\n")
		file_ptr.write("  Text : \"" + ''.join(random.choices(string.ascii_uppercase + string.digits, k=random.randint(238,472))) + "\",\n")
		file_ptr.write("  Parent_Requirement : " + uid + ",\n")
		file_ptr.write("  Owner : ProductTeam,\n")
		file_ptr.write("  Status : Accepted,\n")
		file_ptr.write("  Is_security_relevant : No,\n")
		file_ptr.write("}\n\n")
		
		unique_number = unique_number+1
	

#main	
unique_number=1	
for i in range(1,20):
	suffix = ''.join(random.choices(string.ascii_uppercase + string.digits, k=6))
	folder_name = str(i) + "_" + suffix
	print("Creating folder: " + folder_name)
	os.makedirs(folder_name)
	
	for j in range(1, random.randint(20, 35)):
		suffix = ''.join(random.choices(string.ascii_uppercase + string.digits, k=6))
		file_name = str(j) + "_" + suffix + ".rd"
		filepath = folder_name + "/" + file_name
		print("Creating file:" + filepath)
		file1 = open(filepath, "w")
		write_content(file1)
		file1.close()