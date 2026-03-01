#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <limits>
#include <algorithm>

struct Point
{
    double x, y; // 2D coordinates
    int cluster; // Assigned cluster ID (-1 if unassigned)
};

class KMeans
{
    std::vector<Point> points;  // All data points
    std::vector<Point> centers; // K cluster centers
    int k;                 // Number of clusters
    double tolerance;      // Convergence threshold
    int maxIterations;     // Maximum iterations
    int iterations;        // Actual iterations completed
    int width;             // Image width
    int height;            // Image height
    std::vector<std::vector<int>> image; // Image data

    /**
     * 1. distance(const Point& p1, const Point& p2) -> double

        Calculate the Euclidean distance between two points.

        2. initializeCenters() -> void

        Randomly select K points from the dataset as initial cluster centers.

        3. assignPoints() -> void

        For each data point, find the nearest cluster center and assign the point to that cluster.

        4. updateCenters() -> bool

        Calculate new cluster centers as the mean of all assigned points. Return true if converged, false otherwise.

        5. fit() -> void

        Main algorithm loop:

        Initialize centers
        For each iteration (up to maxIterations):
        Call assignPoints()
        Call updateCenters()
        Print iteration number and current inertia
        Check for convergence
        Print final status (converged or max iterations reached)
        6. calculateInertia() -> double

        Calculate total inertia: sum of squared distances from each point to its assigned center.

        Inertia = Σ (distance from point to its center)²
        7. saveAsImage(const string& filename) -> void

        Generate a PPM image visualization:

        Plot each data point as a colored dot (color = cluster ID)
        Mark cluster centers with crosses
        Save as PPM format file
     */

    public:
        /**
         * Constructor for the KMeans class.
         * @param k The number of clusters.
         * @param tolerance The tolerance for the convergence of the algorithm.
         * @param maxIterations The maximum number of iterations for the algorithm.
         * @return void
         */
        KMeans(int k, double tolerance, int maxIterations) {
            this->k = k;
            this->tolerance = tolerance;
            this->maxIterations = maxIterations;
            this->iterations = 0;
        }

        /**
         * Add a point to the clustering algorithm.
         * @param x The x coordinate of the point.
         * @param y The y coordinate of the point.
         * @return void
         */
        void addPoint(double x, double y) {
            points.push_back({x, y, -1});
        }

        /**
         * Generate synthetic data for the clustering algorithm.
         * @param numPoints The number of points to generate.
         * @return void
         */ 
        void generateSyntheticData(int numPoints)
        {

            std::random_device rd;

            std::mt19937 gen(rd());

            std::normal_distribution<> dis(0, 1);

            std::vector<std::pair<double, double>> clusterCenters = {

                {2, 2}, {8, 8}, {8, 2}

            };

            int pointsPerCluster = numPoints / k;

            for (int i = 0; i < k; i++)
            {

                for (int j = 0; j < pointsPerCluster; j++)
                {

                    double x = clusterCenters[i % clusterCenters.size()].first +

                               dis(gen) * 0.5;

                    double y = clusterCenters[i % clusterCenters.size()].second +

                               dis(gen) * 0.5;

                    addPoint(x, y);
                }
            }
        }

        /**
         * Run the clustering algorithm.
         * @return void
         */
        void fit() {
            initializeCenters();
            for (int i = 0; i < maxIterations; i++) {
                assignPoints();
                updateCenters();
            }
        }

        /**
         * Calculate the inertia of the clustering algorithm.
         * @return The inertia of the clustering algorithm.
         */
        double calculateInertia() {
            double inertia = 0;
            for (const auto& point : points) {
                inertia += (point.x - centers[point.cluster].x) * (point.x - centers[point.cluster].x) + (point.y - centers[point.cluster].y) * (point.y - centers[point.cluster].y);
            }
            return inertia;
        }

        /**
         * Save the clustering algorithm as a PPM image.
         * @param filename The name of the file to save the image to.
         * @return void
         */
        void saveAsImage(const std::string& filename) {
            width = 400;
            height = 400;
            image.assign(height, std::vector<int>(width, 255));

            if (points.empty() && centers.empty()) {
                std::ofstream file(filename);
                file << "P2\n" << width << " " << height << "\n255\n";
                for (int i = 0; i < height; i++)
                    for (int j = 0; j < width; j++)
                        file << image[i][j] << " ";
                return;
            }

            double minX = points.empty() ? centers[0].x : points[0].x;
            double maxX = minX;
            double minY = points.empty() ? centers[0].y : points[0].y;
            double maxY = minY;
            for (const auto& p : points) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }
            for (const auto& c : centers) {
                minX = std::min(minX, c.x);
                maxX = std::max(maxX, c.x);
                minY = std::min(minY, c.y);
                maxY = std::max(maxY, c.y);
            }
            double padX = (maxX - minX) * 0.1 + 1e-6;
            double padY = (maxY - minY) * 0.1 + 1e-6;
            minX -= padX; maxX += padX;
            minY -= padY; maxY += padY;

            auto toPixelX = [&](double x) {
                int px = static_cast<int>((x - minX) / (maxX - minX) * (width - 1));
                return std::max(0, std::min(width - 1, px));
            };
            auto toPixelY = [&](double y) {
                int py = static_cast<int>((y - minY) / (maxY - minY) * (height - 1));
                py = height - 1 - py;
                return std::max(0, std::min(height - 1, py));
            };

            for (const auto& p : points) {
                int px = toPixelX(p.x);
                int py = toPixelY(p.y);
                int gray = (p.cluster >= 0 && p.cluster < k) ? (p.cluster * 80) % 256 : 128;
                image[py][px] = gray;
            }

            for (const auto& c : centers) {
                int cx = toPixelX(c.x);
                int cy = toPixelY(c.y);
                int r = 3;
                for (int dx = -r; dx <= r; dx++)
                    for (int dy = -r; dy <= r; dy++)
                        if (dx == 0 || dy == 0) {
                            int nx = std::max(0, std::min(width - 1, cx + dx));
                            int ny = std::max(0, std::min(height - 1, cy + dy));
                            image[ny][nx] = 0;
                        }
            }

            std::ofstream file(filename);
            file << "P2\n" << width << " " << height << "\n255\n";
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    file << image[i][j] << " ";
                }
            }
        }

    private:
        void initializeCenters() {
            if (points.empty()) return;
            centers.clear();
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, static_cast<int>(points.size()) - 1);
            for (int i = 0; i < k; i++) {
                Point c = points[dis(gen)];
                c.cluster = -1;
                centers.push_back(c);
            }
        }

        void assignPoints() {
            for (auto& point : points) {
                double minDistance = std::numeric_limits<double>::max();
                int minCluster = -1;
                for (int i = 0; i < k; i++) {
                    double distance = (point.x - centers[i].x) * (point.x - centers[i].x) + (point.y - centers[i].y) * (point.y - centers[i].y);
                    if (distance < minDistance) {
                        minDistance = distance;
                        minCluster = i;
                    }
                }
                point.cluster = minCluster;
            }
        }

        void updateCenters() {
            for (int i = 0; i < k; i++) {
                double sumX = 0;
                double sumY = 0;
                int count = 0;
                for (const auto& point : points) {
                    if (point.cluster == i) {
                        sumX += point.x;
                        sumY += point.y;
                        count++;
                    }
                }
                if (count > 0) {
                    centers[i].x = sumX / count;
                    centers[i].y = sumY / count;
                }
            }
        }

        bool converged() {
            for (int i = 0; i < k; i++) {
                if (centers[i].x != centers[i].x || centers[i].y != centers[i].y) {
                    return false;
                }
            }
            return true;
        }
};

#endif