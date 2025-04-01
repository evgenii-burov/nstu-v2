import matplotlib.pyplot as plt
import plotly.express as px
import numpy as np
import math
import pandas as pd


def GetSample(file_name: str) -> list:
    input_values = open(rf"C:\Users\evgen\Documents\НГТУ\ООП\OOPlab1\{file_name}")
    sample = []
    for line in input_values:
        sample = line.removesuffix(' ').split(sep=' ')
    input_values.close()
    for i in range(len(sample)):
        sample[i] = float(sample[i])
    return sample

def StandartTheoreticalDensityFunction(nu: float, mu: float, lambd: float, sample: list)->list:
    a = math.pi * math.tan(math.pi * nu / 2);
    K = math.sin(math.pi * nu) * (1 / math.pi + math.pi / (a * a)) + nu
    density_function = []
    for sample_entry in sample:
        x = (sample_entry - mu) / lambd
        # x=sample_entry*lambd + mu
        # x=sample_entry
        if abs(x) < nu: # <=
            # density_function.append((math.cos(math.pi * x / 2) ** 2) / K)
            density_function.append((math.cos(math.pi * x / 2) ** 2) / (K*lambd))
        else:
            density_function.append(math.exp(-1 * a * (abs(x) - nu)) * (math.cos(math.pi * x / 2) ** 2) / (K*lambd))
    return density_function

def StandartEmpiricalDensityFunction(freq: list, sample: list) -> list:
    density_function = []
    k = len(freq)
    h = (max(sample) - min(sample)) / k
    for i in range(k):
        for j in range(int(freq[i])):
            density_function.append(freq[i] / (len(sample) * h))
    return density_function

def MixtureTheoreticalDensityFunction(nu1: float, mu1: float, lambd1: float,nu2: float, mu2: float, lambd2: float, p: float, sample: list)->list:
    a1 = math.pi * math.tan(math.pi * nu1 / 2);
    K1 = math.sin(math.pi * nu1) * (1 / math.pi + math.pi / (a1 * a1)) + nu1
    a2 = math.pi * math.tan(math.pi * nu2 / 2);
    K2 = math.sin(math.pi * nu2) * (1 / math.pi + math.pi / (a2 * a2)) + nu2
    density_function = []
    for sample_entry in sample:
        f_of_x1 = 0;
        f_of_x2 = 0;
        x1 = (sample_entry - mu1) / lambd1
        if abs(x1) < nu1:  # <=
            f_of_x1=(math.cos(math.pi * x1 / 2) ** 2) / (K1*lambd1)
        else:
            f_of_x1 = math.exp(-1 * a1 * (abs(x1) - nu1)) * (math.cos(math.pi * x1 / 2) ** 2) / (K1*lambd1)
        x2 = (sample_entry - mu2) / lambd2
        if abs(x2) < nu2:  # <=
            f_of_x2 = (math.cos(math.pi * x2 / 2) ** 2) / (K2*lambd2)
        else:
            f_of_x2 = math.exp(-1 * a2 * (abs(x2) - nu2)) * (math.cos(math.pi * x2 / 2) ** 2) / (K2*lambd2)

        # density_function.append((1-p)*f_of_x1/lambd1 + p*f_of_x2/lambd2)
        density_function.append((1 - p) * f_of_x1 + p * f_of_x2)
    return density_function


def MixtureEmpiricalDensityFunction(freq: list, sample: list) -> list:
    density_function = []
    k = len(freq)
    h = (max(sample) - min(sample)) / k
    for i in range(k):
        for j in range(int(freq[i])):
            density_function.append(freq[i] / (len(sample) * h))
    return density_function

def EmpiricEmpiricalDensityFunction(freq: list, sample: list) -> list:
    density_function = []
    k = len(freq)
    h = (max(sample) - min(sample)) / k
    for i in range(k):
        for j in range(int(freq[i])):
            density_function.append(freq[i] / (len(sample) * h))
    return density_function


params_file = open(rf"C:\Users\evgen\Documents\НГТУ\ООП\OOPlab1\params.txt")
params_all = []
for line in params_file:
    params = line.removesuffix('\n').split(sep='\t')
    for i in range(len(params)):
        params[i] = float(params[i])
    params_all.append(params)
print(params_all)
nu = params_all[0][0]
mu = params_all[0][1]
lambd = params_all[0][2]
N = int(params_all[0][3])
nu1 = params_all[1][0]
mu1 = params_all[1][1]
lambd1 = params_all[1][2]
p = params_all[1][3]
nu2 = params_all[2][0]
mu2 = params_all[2][1]
lambd2 = params_all[2][2]
print(params_all)
a = math.pi * math.tan(math.pi * nu / 2);
K = math.sin(math.pi * nu) * (1 / math.pi + math.pi / (a * a)) + nu

#Standart distribution
x=GetSample("standart_sample.txt") #sample.txt
x = sorted(x)
freq = GetSample("frequencies.txt")
k = len(freq)
yst=StandartTheoreticalDensityFunction(nu, mu, lambd, x)
yse = StandartEmpiricalDensityFunction(freq, x)
x_uniform = np.linspace(min(x),max(x),len(x))
y_uniform = StandartTheoreticalDensityFunction(nu, mu, lambd, x_uniform)
plt.figure("Standard")
plt.hist(x,bins=k, histtype="step", density=True, color="green")
plt.scatter(x, yst,s=7, color="blue")
plt.scatter(x, yse,s=7, color="red")
plt.plot(x_uniform, y_uniform, color="cyan")


#Distribution mixture
x_mix = GetSample("mixture_sample.txt")
x_mix = sorted(x_mix)
y_mixth = MixtureTheoreticalDensityFunction(nu1, mu1, lambd1, nu2, mu2, lambd2, p, x_mix)
mix_freq = GetSample("mixture_frequencies.txt")
y_mixem = MixtureEmpiricalDensityFunction(mix_freq, x_mix)
x_mix_uniform = np.linspace(min(x_mix),max(x_mix),len(x_mix))
y_mix_uniform = MixtureTheoreticalDensityFunction(nu1,mu1,lambd1,nu2,mu2,lambd2,p,x_mix_uniform)
plt.figure("Mixture")
plt.plot(x_mix_uniform, y_mix_uniform, color="cyan")
plt.hist(x_mix,bins=k, histtype="step", density=True, color="green")
plt.scatter(x_mix, y_mixth,s=7, color="blue")
plt.scatter(x_mix, y_mixem,s=7, color="red")


#Empirical distribution
xe = GetSample("empiric_sample.txt")
xe = sorted(xe)
empiric_freq = GetSample("empiric_frequencies.txt")
y_emem = EmpiricEmpiricalDensityFunction(empiric_freq, xe)
plt.figure("Empiric")
plt.hist(xe,bins=k, histtype="step", density=True, color="green")
plt.scatter(xe, y_emem,s=7, color="red")

plt.show()

# // int
# counter = 0;
# // int
# freq_i = 0;
# // for (int i = 0; i < k; i++)
#     // {
#        // freq_i = frequencies[i];
#     // for (int j = 0; j < freq_i; j++)
#            // {
#            // function[counter] = freq_i/ (N * h);
#     // counter++;
#     //}
#     //}
# // for (int i = 0; i < N; i++)
#     // {
#        // interval_index = int((sample[i] - x_min) / h);
#     // interval_index -= int(((sample[i] - x_min) / h) / k);; // interval_index -= int(((sample[i] - x_min) / h) / k);
#     // function[i] = frequencies[interval_index] / (N * h);

#x = np.linspace(min(x),max(x),len(x))

#fig1 = px.scatter(x=x, y=y)

