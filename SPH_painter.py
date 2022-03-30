from struct import unpack
import numpy as np
from matplotlib import pyplot as plt
plt.ion()

f = open("data.bin", 'rb')

dt = 0.001
x_range = (0, 12)
y_range = (0, 9)
z_range = (0, 9)

size_b          = f.read(4)
size            = (unpack('I', size_b))[0] # int

size_per_batch  = size * 3 * 8
data_b          = f.read(size_per_batch)

x,y,z = [],[],[]
ax3 = plt.axes(projection = '3d') # do not need generate a new canvas every time

step_cnt = 0
while size_per_batch == len(data_b) :

    print(step_cnt)
    step_cnt += 1 
    
    data        = unpack('ddd' * size , data_b) # tuple
    data        = np.asarray(data)
    
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
    
    data_b      = f.read(size_per_batch)

print("Done.")

plt.ioff()    
ax3.scatter(x,y,z)
plt.show()

f.close()
