#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>

// Function to compute the mean of a vector
double mean(const std::vector<double>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

// Function to calculate the linear regression coefficients (slope and intercept)
std::pair<double, double> linearRegression(const std::vector<double>& x, const std::vector<double>& y) {
    double x_mean = mean(x);
    double y_mean = mean(y);
    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - x_mean) * (y[i] - y_mean);
        denominator += (x[i] - x_mean) * (x[i] - x_mean);
    }

    if (denominator == 0) {
        std::cerr << "Error: Division by zero detected in regression calculation." << std::endl;
        return {0, 0};
    }

    double slope = numerator / denominator;
    double intercept = y_mean - slope * x_mean;

    return {slope, intercept};
}

// Function to forecast future values
std::vector<double> forecast(double slope, double intercept, int startYear, int endYear) {
    std::vector<double> predictions;
    for (int year = startYear; year <= endYear; ++year) {
        predictions.push_back(slope * year + intercept);
    }
    return predictions;
}

int main() {
    // Read data from CSV file
    std::ifstream file("../data/sea-surface-temp_fig-1.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open CSV file." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<double> years;
    std::vector<double> anomalies;

    // Skip header line
    std::getline(file, line);

    // Parse the file and fill the vectors
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string yearStr, anomalyStr;
        std::getline(ss, yearStr, ',');
        std::getline(ss, anomalyStr, ',');

        if (!yearStr.empty() && !anomalyStr.empty()) {
            try {
                years.push_back(std::stod(yearStr));
                anomalies.push_back(std::stod(anomalyStr));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid data encountered - " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Data out of range - " << line << std::endl;
            }
        }
    }
    file.close();

    if (years.empty() || anomalies.empty()) {
        std::cerr << "Error: Vectors are empty. Check CSV parsing." << std::endl;
        return 1;
    }

    // Calculate the regression coefficients
    auto [slope, intercept] = linearRegression(years, anomalies);

    // Print baseline information
    std::cout << "Baseline Period: 1951-1980\n";
    std::cout << "Year,Anomaly\n";

    // Print historical data
    for (size_t i = 0; i < years.size(); ++i) {
        std::cout << years[i] << "," << anomalies[i] << "\n";
    }

    // Forecast future temperatures for the next 100 years
    int startYear = 2024;
    int endYear = 2124;
    std::vector<double> futureTemperatures = forecast(slope, intercept, startYear, endYear);

    // Print forecasted data
    std::cout << "\nYear,Forecasted Anomaly\n";
    for (int i = 0; i < futureTemperatures.size(); ++i) {
        std::cout << (startYear + i) << "," << futureTemperatures[i] << "\n";
    }

    return 0;
}
