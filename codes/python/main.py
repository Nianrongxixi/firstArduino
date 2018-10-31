import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('../../data/LOG000.csv')
# print(df.describe())
# print(df.head())
df.replace({'People is coming': 1}, inplace=True)
df.replace({'Watching': 0}, inplace = True)
# print(df.head())

# sns.lineplot(data=df['loudness'][:9000])
# plt.show()
# sns.lineplot(range(14999), abs(np.fft.fft(df[:30000]['loudness']))[1:15000])
# plt.show()
# sns.lineplot(range(14999), abs(np.fft.fft(df[38000:68000]['loudness']))[1:15000])
# plt.show()
# sns.lineplot(range(14999), abs(np.fft.fft(df[60000:90000]['loudness']))[1:15000])
# plt.show()
for i in range(0,len(df),1000):
    sns.lineplot(range(499), abs(np.fft.fft(df[i:i+1000]['loudness']))[1:500])
    plt.show()
