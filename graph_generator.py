import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

def plot_rehash_timing():
    # Load data from the timing results file
    if not os.path.exists('data/timing_results.txt'):
        print("Error: data/timing_results.txt not found. Run the C++ program first.")
        return
    
    df = pd.read_csv('data/timing_results.txt')
    
    # Plot the three curves
    plt.figure(figsize=(10, 6))
    
    if 'NoRehashTime' in df.columns:
        plt.plot(df['q'], df['NoRehashTime'], 'b-', label='No Rehash')
    if 'RehashDoubleTime' in df.columns:
        plt.plot(df['q'], df['RehashDoubleTime'], 'r-', label='Rehash (Double)')
    if 'RehashAdd10000Time' in df.columns:
        plt.plot(df['q'], df['RehashAdd10000Time'], 'g-', label='Rehash (Add 10000)')
    
    plt.xlabel('q (log scale)')
    plt.ylabel('Time per operation (μs)')
    plt.title('Hash Table Performance: Time per Operation vs Table Size')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, which="both", ls="-")
    plt.savefig('data/rehash_timing.png')
    plt.close()
    print("Saved rehash timing graph to data/rehash_timing.png")

def plot_load_factor_performance():
    # Load data from the load factor results file
    if not os.path.exists('data/load_factor_results.txt'):
        print("Error: data/load_factor_results.txt not found. Run the C++ program first.")
        return
    
    df = pd.read_csv('data/load_factor_results.txt')
    
    # Plot successful and unsuccessful search times
    plt.figure(figsize=(10, 6))
    plt.plot(df['LoadFactor'], df['SuccessfulSearchTime'], 'b-', label='Successful Search')
    plt.plot(df['LoadFactor'], df['UnsuccessfulSearchTime'], 'r-', label='Unsuccessful Search')
    
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Average Time per Search (ns)')
    plt.title('Hash Table Search Performance vs Load Factor')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/search_performance.png')
    plt.close()
    print("Saved search performance graph to data/search_performance.png")

def plot_theoretical_comparison():
    if not os.path.exists('data/load_factor_results.txt'):
        print("Error: data/load_factor_results.txt not found. Run the C++ program first.")
        return
    
    df = pd.read_csv('data/load_factor_results.txt')
    
    # Calculate scaling factor Q
    lambda_half = 0.5
    idx = (df['LoadFactor'] - lambda_half).abs().idxmin()
    measured_time = df.loc[idx, 'SuccessfulSearchTime']
    
    # Theoretical formulas
    linear_probe_succ = lambda l: 0.5 + 1/(2*(1-l)**2)
    double_hash_succ = lambda l: -np.log(1-l)/l if l != 0 else 1
    
    Q = linear_probe_succ(lambda_half) / measured_time
    
    # Generate load factors for theoretical curves
    lambdas = np.linspace(0.01, 0.99, 100)
    
    # Plot 1: Successful search comparison
    plt.figure(figsize=(10, 6))
    plt.plot(lambdas, linear_probe_succ(lambdas), 'b-', label='Theoretical Linear Probing')
    plt.plot(lambdas, double_hash_succ(lambdas), 'g-', label='Theoretical Double Hashing')
    plt.plot(df['LoadFactor'], Q * df['SuccessfulSearchTime'], 'r--', label='Scaled Measured Time (Linear Probing)')
    
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Number of Probes')
    plt.title('Successful Search: Theoretical vs Measured Performance')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/time_succ.png')
    plt.close()
    print("Saved successful search comparison to data/time_succ.png")
    
    # Plot 2: Unsuccessful search comparison
    linear_probe_fail = lambda l: 0.5 + 1/(2*(1-l))
    double_hash_fail = lambda l: 1/(1-l)
    
    plt.figure(figsize=(10, 6))
    plt.plot(lambdas, linear_probe_fail(lambdas), 'b-', label='Theoretical Linear Probing')
    plt.plot(lambdas, double_hash_fail(lambdas), 'g-', label='Theoretical Double Hashing')
    plt.plot(df['LoadFactor'], Q * df['UnsuccessfulSearchTime'], 'r--', label='Scaled Measured Time (Linear Probing)')
    
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Number of Probes')
    plt.title('Unsuccessful Search: Theoretical vs Measured Performance')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/time_fail.png')
    plt.close()
    print("Saved unsuccessful search comparison to data/time_fail.png")

def main():
    # Create data directory if it doesn't exist
    if not os.path.exists('data'):
        os.makedirs('data')
    
    plot_rehash_timing()
    plot_load_factor_performance()
    plot_theoretical_comparison()

if __name__ == "__main__":
    main()