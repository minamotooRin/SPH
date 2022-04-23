import json
from struct import unpack

import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation, writers

import sys
# from functools import partial

global FLOAT_BYTES
global DIM

fig = plt.figure(figsize=(8, 6), tight_layout=True) # do not need generate a new canvas every time
    
def draw2D(load_dict, data_file):

    size    = load_dict["paritcle_number"]
    print ("Number: ", size)

    size_per_batch  = size * DIM * FLOAT_BYTES
    data_type       = 'f'
    if FLOAT_BYTES == 8:
        data_type   = 'd'

    x,y = [],[]
    sc = plt.scatter(x,y, alpha=0.1)

    step_cnt = 0
    data_b          = data_file.read(size_per_batch)
    while size_per_batch == len(data_b) :
        step_cnt += 1 
        if step_cnt % 100 == 0 :
            print("Progress: {}/{}".format(step_cnt, load_dict["step"]))

        data_raw    = unpack(data_type * DIM * size , data_b) # tuple
        data        = np.asarray(data_raw)
        
        x           = data[0 : : 2]
        y           = data[1 : : 2]
        
        sc.set_offsets(np.c_[x,y])
        # sc = plt.scatter(x,y)
        yield sc,

        data_b      = data_file.read(size_per_batch)

    print("Done.")

def draw3D(load_dict, data_file):

    size    = load_dict["paritcle_number"]
    print ("Number: ", size)

    size_per_batch  = size * DIM * FLOAT_BYTES
    data_type = 'f'
    if FLOAT_BYTES == 8:
        data_type = 'd'

    x,y,z = [],[],[]
    ax3 = plt.axes(projection = '3d') # do not need generate a new canvas every time
    ax3.scatter(x, y, z, alpha=0.05)

    step_cnt = 0
    data_b          = data_file.read(size_per_batch)
    while size_per_batch == len(data_b) :

        step_cnt += 1 
        
        data_raw    = unpack(data_type * DIM * size , data_b) # tuple
        data        = np.asarray(data_raw)
        
        x           = data[0 : : 3]
        y           = data[1 : : 3]
        z           = data[2 : : 3]
        
        ax3.set_offsets(np.c_[x,y,z])
        yield ax3,
        
        data_b      = data_file.read(size_per_batch)

    print("Done.")

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

            data_raw    = (unpack('cc', data_file.read(2)))
            DIM         = int.from_bytes(data_raw[0], byteorder='big', signed=True)
            FLOAT_BYTES = int.from_bytes(data_raw[1], byteorder='big', signed=True)

            # selected coordinate
            x_range = (0, load_dict["volume"]["x"])
            y_range = (0, load_dict["volume"]["y"])
            plt.xlim(x_range)
            plt.ylim(y_range)

            if DIM == 2 :
                frames_iter = draw2D(load_dict, data_file)
            elif DIM == 3:
                z_range = (0, load_dict["volume"]["z"])
                plt.xlim(z_range)
                frames_iter = draw3D(load_dict, data_file)
                
            update = lambda f: next(frames_iter)

            # load_dict["dt"]
            ani = FuncAnimation(fig, update, frames=np.arange(0, load_dict["step"]), interval=1)
            # plt.show()

            Writer = writers['ffmpeg']#需安装ffmpeg
            writer = Writer(fps=30, metadata=dict(artist='Me'), bitrate=1800)
            ani.save("{}_{}.mp4".format(load_dict["paritcle_number"], load_dict["step"]), writer=writer)
