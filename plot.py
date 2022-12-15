import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def load_file(filename):
    data = np.loadtxt(filename,skiprows = 1)
    x = []
    y = []
    for i in range(len(data)):
        x.append(data[i][1])
        y.append(data[i][0])
    return x,y


if __name__ == "__main__":
    dir = './log/'
    filename1 = 'log.txt'
    filename2 = 'log2048.txt'
    filename3 = 'log4096.txt'
    x,y1 = load_file(dir+filename1)
    x,y2 = load_file(dir+filename2)
    x,y3 = load_file(dir+filename3)

    plt.rcParams['font.sans-serif'] = ['SimHei']

    plt.plot(x,y1,'-',color='darkorange',linewidth=1.0,label='size=1024')
    plt.plot(x,y2,'-',color='darkcyan',linewidth=1.0,label='size=2048')
    plt.plot(x,y3,'-',color='crimson',linewidth=1.0,label='size=4096')

    plt.xlim((0, 20000))

    plt.xlabel('times',fontsize=18)
    plt.ylabel('Hit rate',fontsize=18)
    plt.legend()
    plt.savefig('./figure/hit_rate.png')
    plt.show()



