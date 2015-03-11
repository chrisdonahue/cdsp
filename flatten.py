# squashes src code to one directory

import fnmatch
import os
import shutil

# directory paths
src_dir_path = "./src"
src_flat_dir_path = "./src_flat"

# create output dir if it doesn't exist
if not os.path.exists(src_flat_dir_path):
	os.makedirs(src_flat_dir_path)

# traverse src dir finding .hpp and .cpp files
src_file_name_to_path = {}
for root, dirs, files in os.walk(src_dir_path):
	for file_name in fnmatch.filter(files, '*.*pp'):
		if file_name in src_file_name_to_path:
			raise Exception('Duplicate file name: {0}'.format(file_name))
		src_file_name_to_path[file_name] = os.path.join(root, file_name)

# copy files
for file_name, file_path in src_file_name_to_path.iteritems():
	shutil.copyfile(file_path, os.path.join(src_flat_dir_path, file_name))
