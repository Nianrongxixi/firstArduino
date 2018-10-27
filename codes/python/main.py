import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

df = pd.read_csv('../../data/LOG000.csv')
print(df.describe())
print(df.head())
df.replace({'People is coming': 1}, inplace=True)
df.replace({'Watching': 0}, inplace = True)
print(df.head())
distance_50 = df.loc[df['distance'] < 50]
print(distance_50.describe())
sns.lineplot(data=distance_50['distance'])
plt.show()

