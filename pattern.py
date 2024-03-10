import matplotlib.pyplot as plt
# import numpy as np
import sympy as sp

# Prime factorization function
def prime_factors(n):
    return list(sp.factorint(n).keys())

# Define a range for n values and initialize lists to hold the scatter plot data
n_values = range(2, 20)  # Adjust as needed
scatter_x = []  # Prime factors
scatter_y = []  # Corresponding n values

# Prime factorization and data collection
for n in n_values:
    value = 2**n - 1
    factors = prime_factors(value)
    for factor in factors:
        scatter_x.append(factor)
        scatter_y.append(n)

# Plotting
plt.figure(figsize=(12, 8))  # Adjust size as needed
plt.scatter(scatter_x, scatter_y, alpha=0.6)  # Set alpha for better visibility if points overlap
plt.xlabel('Prime Factors')
plt.ylabel('n (for 2^n - 1)')
plt.title('Distribution of Prime Factors of 2^n - 1')
plt.grid(True)  # Enable grid for easier reading
plt.show()