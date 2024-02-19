import matplotlib.pyplot as plt
import numpy as np

with open('cycle.txt', 'r') as file:
    data = file.readlines()

function1_speeds = [float(data[i].strip()) for i in range(0, len(data), 2)]
function2_speeds = [float(data[i].strip()) for i in range(1, len(data), 2)]

# Calculate the averages
average_function1 = np.mean(function1_speeds)
average_function2 = np.mean(function2_speeds)

# Rolling mean for smoothing
window_size = 50
function1_smooth = np.convolve(function1_speeds, np.ones(window_size)/window_size, mode='valid')
function2_smooth = np.convolve(function2_speeds, np.ones(window_size)/window_size, mode='valid')

# Set up the figure and axes for a prettier graph
fig, ax1 = plt.subplots(figsize=(14, 7), dpi=100)

# Create a second y-axis for the other metric
ax2 = ax1.twinx()

# Plot the smoothed data with distinct designs
ax1.plot(function1_smooth, color='dodgerblue', label='Total Cycles', alpha=0.9, linewidth=2, linestyle='-', marker='o', markevery=40)
ax2.plot(function2_smooth, color='crimson', label='Cycles per Word', alpha=0.9, linewidth=2, linestyle='-', marker='^', markevery=40)

# Add horizontal lines to indicate the average values
ax1.axhline(average_function1, color='dodgerblue', linestyle='dashdot', linewidth=2, label=f'Avg Total cycles: {average_function1:.2f} cycle')
ax2.axhline(average_function2, color='crimson', linestyle='dashdot', linewidth=2, label=f'Avg Cycles per word: {average_function2:.2f} cycle/word')

# Setting labels, titles, and legends
ax1.set_ylabel('Total Cycles', fontsize=16, fontweight='bold', color='dodgerblue')
ax2.set_ylabel('Cycles per Word', fontsize=16, fontweight='bold', color='crimson')
ax1.set_xlabel('Run Number', fontsize=16, fontweight='bold')
ax1.set_title('Measurement of Cycles: 64-bit Addition', fontsize=18, fontweight='bold')

# Combine legends
lines, labels = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines + lines2, labels + labels2, loc='upper right', frameon=True, framealpha=0.9, shadow=True, fancybox=True, fontsize=12)

# Customizing the grid and layout
ax1.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout()

# Enhancing the overall look
fig.patch.set_facecolor('white')
fig.patch.set_edgecolor('lightgrey')

# Show the plot
plt.show()