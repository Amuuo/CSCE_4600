import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn import linear_model


file_sizes = ['1', '2', '4', '8', '16', '32', '64', '128']

df = pd.read_csv("data.csv")

filestep, seconds = df[['MB', 'bytes per write']], df[['seconds']]


print(df.head())
#plt.plot(filestep, seconds)
#plt.scatter(filestep, seconds, label=None, c=np.log2(filestep))

plt.show()

#plt.title("File Copy Speeds")
#plt.xlabel("Bytes Per Write")
#plt.ylabel("Seconds")



#plt.legend(scatterpoints=1, frameon=False, labelspacing=1, title='File Size')
#plt.savefig("File_Copy_Speeds.png")