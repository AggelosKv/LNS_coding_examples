#include <random>

// Function to generate a random floating-point number between 0 and 20000
double generateRandomDouble(double maxValue = 20000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<double> dist(1, maxValue);
    
    return dist(gen);
}
