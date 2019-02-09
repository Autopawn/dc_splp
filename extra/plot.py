import numpy as np
import matplotlib.pyplot as plt

vals = {}

vals['27FPP_17S'] = [
    831.053589,893.51709,528.038208,377.991119,308.576294,276.308655,253.988358,238.92067,227.728195,237.013504,227.323013]
vals['1011EuclS'] = [
    51.437443,54.446232,33.352222,25.014994,19.73213,17.063826,16.074484,17.405373,16.655872,16.063301,16.997032]
vals['1032GapAS'] = [
    159.104477,177.188675,109.999718,81.943275,67.086754,58.682655,53.141834,51.641567,49.177864,49.773876,49.593552]
vals['MS1'] = [
    255.094437,272.583954,161.012955,135.754105,111.701134,101.383369,93.671989,92.451836,91.808067,91.149406,93.900452]

fig = plt.figure()

ax = fig.gca()

plt.grid()
plt.title("Speedup vs. number of threads")
for nam in vals:
    speedups = vals[nam][0]/np.array(vals[nam][1:])
    plt.plot(range(1,len(speedups)+1),speedups,'o-',label=nam)
    ax.set_xticks(np.arange(1,len(speedups)+1,1))

plt.legend()
plt.show()
