import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def plot_temperature_anomalies(file_path):
    # Read the CSV data and handle possible formatting issues
    data = pd.read_csv(file_path, header=0, names=["Year", "Anomaly"])
    
    # Ensure 'Year' and 'Anomaly' are numeric and drop any rows that can't be converted
    data['Year'] = pd.to_numeric(data['Year'], errors='coerce')
    data['Anomaly'] = pd.to_numeric(data['Anomaly'], errors='coerce')
    data = data.dropna(subset=['Year', 'Anomaly'])  # Drop rows where 'Year' or 'Anomaly' is NaN

    # Split data into historical and forecasted parts
    split_year = 2024  # Year to separate historical from forecasted data
    historical_data = data[data['Year'] < split_year]
    forecasted_data = data[data['Year'] >= split_year]

    # Concatenate data for better visualization
    combined_years = pd.concat([historical_data['Year'], forecasted_data['Year']])
    combined_anomalies = pd.concat([historical_data['Anomaly'], forecasted_data['Anomaly']])

    # Plot the combined data for a smoother representation
    plt.figure(figsize=(14, 7))
    plt.plot(combined_years, combined_anomalies, label="Historical and Forecasted Anomalies", color='blue', marker='o')

    # Highlight forecasted part with a different style
    if not forecasted_data.empty:
        plt.plot(forecasted_data['Year'], forecasted_data['Anomaly'], label="Forecasted Anomalies", color='red', linestyle='--')

    # Baseline reference
    plt.axhline(0, color='black', linewidth=1, linestyle='--', label="Baseline (1951-1980)")

    # Customize y-axis to show only 10 evenly spaced ticks
    min_y = combined_anomalies.min()
    max_y = combined_anomalies.max()
    plt.yticks(np.linspace(min_y, max_y, 10))  # Use np.linspace for 10 evenly spaced ticks

    # Labels and title
    plt.title("Ocean Temperature Anomalies Relative to the Baseline")
    plt.xlabel("Year")
    plt.ylabel("Temperature Anomaly (°F)")
    plt.xticks(rotation=45)  # Rotate x-axis labels for better visibility
    plt.legend()

    # Display the plot
    plt.grid(True)
    plt.tight_layout()  # Adjusts plot to make room for labels
    plt.show()

# Call the function with the path to the output file
plot_temperature_anomalies('output.csv')
