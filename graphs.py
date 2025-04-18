import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# === Load Data ===
load_df = pd.read_csv("load_factor_times.csv")
rehash_df = pd.read_csv("rehash_times.csv")

lam = load_df["load_factor"]
succ = load_df["success_time"]
fail = load_df["fail_time"]

# === Theoretical Probe Functions ===
def linear_probe_success(lmbda): return 0.5 * (1 + 1/(1 - lmbda))
def double_probe_success(lmbda): return 1 / (-np.log(1 - lmbda))
def linear_probe_fail(lmbda): return 0.5 * (1 + 1/((1 - lmbda)**2))
def double_probe_fail(lmbda): return 1 / ((1 - lmbda)**2)

# === Scale Q for theoretical match ===
Q = np.mean(succ / linear_probe_success(lam))

# === Plot 1: Success Search Time vs Load Factor ===
plt.figure()
plt.plot(lam, Q * succ, label="Q × Measured Success Time")
plt.plot(lam, linear_probe_success(lam), label="Linear Probe Success (Theoretical)")
plt.plot(lam, double_probe_success(lam), label="Double Probe Success (Theoretical)")
plt.xlabel("Load Factor λ")
plt.ylabel("Average Search Time")
plt.title("Success Search Time vs Load Factor")
plt.legend()
plt.grid(True)
plt.savefig("time_succ.png")

# === Plot 2: Fail Search Time vs Load Factor ===
plt.figure()
plt.plot(lam, Q * fail, label="Q × Measured Fail Time")
plt.plot(lam, linear_probe_fail(lam), label="Linear Probe Fail (Theoretical)")
plt.plot(lam, double_probe_fail(lam), label="Double Probe Fail (Theoretical)")
plt.xlabel("Load Factor λ")
plt.ylabel("Average Search Time")
plt.title("Fail Search Time vs Load Factor")
plt.legend()
plt.grid(True)
plt.savefig("time_fail.png")

# === Plot 3: Rehash Cost per Element vs q ===
plt.figure()
plt.plot(rehash_df["q"], rehash_df["no_rehash"], label="No Rehash")
plt.plot(rehash_df["q"], rehash_df["doubling"], label="Doubling Resize")
plt.plot(rehash_df["q"], rehash_df["addition"], label="Add 10000 Resize")
plt.xlabel("q (initial size = 2^q)")
plt.ylabel("Average Time per Insertion (μs)")
plt.title("Rehash Cost per Element vs Initial Table Size")
plt.legend()
plt.grid(True)
plt.savefig("rehash.png")
