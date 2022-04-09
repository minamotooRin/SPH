import sys
import json
import math
import numpy as np
from struct import unpack
from matplotlib import pyplot as plt

DIM = 3
FLOAT_BYTES = 8

def draw2D(load_dict, data_file):

    dt      = load_dict["dt"]
    x_range = (0, load_dict["volume"]["x"])
    y_range = (0, load_dict["volume"]["y"])

    size    = load_dict["paritcle_number"]
    print ("Number: ", size)

    size_per_batch  = size * DIM * FLOAT_BYTES
    data_b          = data_file.read(size_per_batch)

    x,y = [],[]

    # Anime
    # figure= plt.figure(figsize=(4, 1), dpi = 320)

    plt.ion()
    ax2 = plt.figure(figsize=(8, 6)) # do not need generate a new canvas every time

    step_cnt = 0
    while size_per_batch == len(data_b) :

        # print(step_cnt)
        step_cnt += 1 
        
        # data_raw    = unpack('ff' * size , data_b) # tuple
        data_raw    = unpack('dd' * size , data_b) # tuple
        data        = np.asarray(data_raw)
        
        x           = data[0 : : 2]
        y           = data[1 : : 2]
        
        plt.cla()
        # selected coordinate
        plt.xlim(x_range)
        plt.ylim(y_range)
        plt.scatter(x, y, alpha=0.1)
        plt.pause(dt)
        
        data_b      = data_file.read(size_per_batch)

    print("Done.")

    plt.ioff()    
    plt.scatter(x, y, alpha=0.1)
    plt.show()

def draw3D(load_dict, data_file):

    dt      = load_dict["dt"]
    x_range = (0, load_dict["volume"]["x"])
    y_range = (0, load_dict["volume"]["y"])
    z_range = (0, load_dict["volume"]["z"])

    size    = load_dict["paritcle_number"]
    print ("Number: ", size)

    size_per_batch  = size * DIM * FLOAT_BYTES
    data_b          = data_file.read(size_per_batch)

    x,y,z = [],[],[]

    # Anime
    # figure= plt.figure(figsize=(4, 1), dpi = 320)

    plt.ion()
    ax3 = plt.axes(projection = '3d') # do not need generate a new canvas every time

    step_cnt = 0
    while size_per_batch == len(data_b) :

        # print(step_cnt)
        step_cnt += 1 
        
        # data_raw    = unpack('fff' * size , data_b) # tuple
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
        ax3.scatter(x, y, z, alpha=0.05)
        plt.pause(dt)
        
        data_b      = data_file.read(size_per_batch)

    print("Done.")

    plt.ioff()    
    ax3.scatter(x,y,z)
    plt.show()

if __name__ == "__main__":

    para_file_path = "input.json"
    data_file_path = ""
    if len(sys.argv) > 1:
        para_file_path =  sys.argv[1]
    if len(sys.argv) > 2:
        data_file_path =  sys.argv[2]

    with open( para_file_path, 'r' ) as para_file:
        load_dict = json.load(para_file)

        if data_file_path == "":
            data_file_path = "{}_{}.bin".format(load_dict["paritcle_number"], load_dict["step"])

        with open( data_file_path, 'rb' ) as data_file:
            if DIM == 2 :
                draw2D(load_dict, data_file)
            elif DIM == 3:
                draw3D(load_dict, data_file)
