import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

def plot_rehash_timing():
    df = pd.read_csv('data/timing_results.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df['q'], df['NoRehash'], 'b-', label='No Rehash')
    plt.plot(df['q'], df['Doubling'], 'r-', label='Doubling')
    plt.plot(df['q'], df['Add10000'], 'g-', label='Add 10000')
    plt.xlabel('q (log scale)')
    plt.ylabel('Time per operation (μs)')
    plt.title('Hash Table Insert Performance')
    plt.xscale('log')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/insert_performance.png')
    plt.close()

def plot_load_factor_performance():
    df = pd.read_csv('data/load_factor_results.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df['LoadFactor'], df['SuccessfulSearch'], 'b-', label='Successful Search')
    plt.plot(df['LoadFactor'], df['UnsuccessfulSearch'], 'r-', label='Unsuccessful Search')
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Time per search (ns)')
    plt.title('Search Performance vs Load Factor')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/search_performance.png')
    plt.close()

def plot_theoretical_comparison():
    df = pd.read_csv('data/load_factor_results.csv')
    lambdas = np.linspace(0.05, 0.95, 100)
    
    # Theoretical formulas
    linear_succ = lambda l: 0.5 + 1/(2*(1-l)**2)
    double_succ = lambda l: -np.log(1-l)/l if l != 0 else 1
    linear_fail = lambda l: 0.5 + 1/(2*(1-l))
    double_fail = lambda l: 1/(1-l)
    
    # Scale measured times to match theoretical at λ=0.5
    Q = linear_succ(0.5) / df[df['LoadFactor'] >= 0.5].iloc[0]['SuccessfulSearch']
    
    # Successful search comparison
    plt.figure(figsize=(10, 6))
    plt.plot(lambdas, linear_succ(lambdas), 'b-', label='Theoretical Linear')
    plt.plot(lambdas, double_succ(lambdas), 'g-', label='Theoretical Double')
    plt.plot(df['LoadFactor'], Q * df['SuccessfulSearch'], 'r--', label='Measured (scaled)')
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Probes (normalized)')
    plt.title('Successful Search Comparison')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/theoretical_success.png')
    plt.close()
    
    # Unsuccessful search comparison
    plt.figure(figsize=(10, 6))
    plt.plot(lambdas, linear_fail(lambdas), 'b-', label='Theoretical Linear')
    plt.plot(lambdas, double_fail(lambdas), 'g-', label='Theoretical Double')
    plt.plot(df['LoadFactor'], Q * df['UnsuccessfulSearch'], 'r--', label='Measured (scaled)')
    plt.xlabel('Load Factor (λ)')
    plt.ylabel('Probes (normalized)')
    plt.title('Unsuccessful Search Comparison')
    plt.legend()
    plt.grid(True)
    plt.savefig('data/theoretical_fail.png')
    plt.close()

if __name__ == "__main__":
    os.makedirs('data', exist_ok=True)
    plot_rehash_timing()
    plot_load_factor_performance()
    plot_theoretical_comparison()
    print("Graphs generated in data/ directory")