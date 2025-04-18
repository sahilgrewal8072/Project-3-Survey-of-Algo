import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load Factor vs Time
df = pd.read_csv("load_factor_times.csv")
lam = df["load_factor"]
succ = df["success_time"]
fail = df["fail_time"]

# Theoretical curves
def linear_probe_success(lmbda): return 0.5 * (1 + 1/(1 - lmbda))
def linear_probe_fail(lmbda): return 0.5 * (1 + 1/((1 - lmbda)**2))
def double_probe_success(lmbda): return 1 / (-np.log(1 - lmbda))
def double_probe_fail(lmbda): return 1 / ((1 - lmbda)**2)

Q = np.mean(succ / linear_probe_success(lam))

plt.figure()
plt.plot(lam, Q * succ, label="Measured Success Time")
plt.plot(lam, linear_probe_success(lam), label="Linear Probes Success")
plt.plot(lam, double_probe_success(lam), label="Double Hash Success")
plt.legend(); plt.title("Success Search Time vs Load Factor")
plt.xlabel("Load Factor"); plt.ylabel("Avg Time (scaled)"); plt.grid()
plt.savefig("time_succ.png")

plt.figure()
plt.plot(lam, Q * fail, label="Measured Fail Time")
plt.plot(lam, linear_probe_fail(lam), label="Linear Probes Fail")
plt.plot(lam, double_probe_fail(lam), label="Double Hash Fail")
plt.legend(); plt.title("Fail Search Time vs Load Factor")
plt.xlabel("Load Factor"); plt.ylabel("Avg Time (scaled)"); plt.grid()
plt.savefig("time_fail.png")
