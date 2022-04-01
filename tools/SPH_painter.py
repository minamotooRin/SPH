import sys
import json
import numpy as np
from struct import unpack
from matplotlib import pyplot as plt

DIM = 3
FLOAT_BYTES = 8

def run(para_file, data_file):
    load_dict = json.load(para_file)

    dt      = load_dict["dt"]
    x_range = (0, load_dict["volume"]["x"])
    y_range = (0, load_dict["volume"]["y"])
    z_range = (0, load_dict["volume"]["z"])

    # size_b          = data_file.read(4)
    # size            = (unpack('I', size_b))[0] # int
    size    = load_dict["paritcle_number"]

    size_per_batch  = size * DIM * FLOAT_BYTES
    data_b          = data_file.read(size_per_batch)

    x,y,z = [],[],[]

    plt.ion()
    ax3 = plt.axes(projection = '3d') # do not need generate a new canvas every time

    step_cnt = 0
    while size_per_batch == len(data_b) :

        # print(step_cnt)
        step_cnt += 1 
        
        data_raw    = unpack('ddd' * size , data_b) # tuple
        data        = np.asarray(data_raw)
        
        x           = data[0 : : 3]
        y           = data[1 : : 3]
        z           = data[2 : : 3]
        
        plt.cla()
        # selected coordinate
        ax3.set_xlim(x_range)
        ax3.set_ylim(y_range)
        ax3.set_zlim(z_range)
        ax3.scatter(x, y, z)
        plt.pause(dt)
        
        data_b      = data_file.read(size_per_batch)

    print("Done.")

    plt.ioff()    
    ax3.scatter(x,y,z)
    plt.show()

if __name__ == "__main__":

    para_file_path = "input.json"
    data_file_path = "ouput.bin"
    if len(sys.argv) > 1:
        para_file_path =  sys.argv[1]
    if len(sys.argv) > 2:
        data_file_path =  sys.argv[2]

    with open( para_file_path, 'r' ) as para_file:
        with open( data_file_path, 'rb' ) as data_file:
            run(para_file, data_file)
