#include "kmeans.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

int main()
{
    KMeans kmeans(3, 1e-4, 100);       // k=3 clusters, tolerance=1e-4, max 100 iterations
    kmeans.generateSyntheticData(300); // Generate 300 random points
    kmeans.fit();                      // Run clustering
    kmeans.saveAsImage("output.ppm");  // Save visualization
    return 0;
}