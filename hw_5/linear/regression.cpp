#include "linear.h"

void LinearRegression::fit() {
    if (data.empty()) {
        std::cerr << "No data to fit.\n";
        return;
    }
    size_t n = data.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (const auto& p : data) {
        sumX += p.x;
        sumY += p.y;
        sumXY += p.x * p.y;
        sumX2 += p.x * p.x;
    }
    double denom = n * sumX2 - sumX * sumX;
    if (std::fabs(denom) < 1e-10) {
        slope = 0;
        intercept = sumY / n;
    } else {
        slope = (n * sumXY - sumX * sumY) / denom;
        intercept = (sumY - slope * sumX) / n;
    }
    calculateMetrics();
    std::cout << "Model: y = " << slope << "x + " << intercept << "\n";
}

void LinearRegression::calculateMetrics() {
    if (data.empty()) return;
    size_t n = data.size();
    for (auto& p : data) {
        p.predicted = slope * p.x + intercept;
    }
    double meanY = 0;
    for (const auto& p : data) meanY += p.y;
    meanY /= n;
    double ssTot = 0, ssRes = 0;
    for (const auto& p : data) {
        ssTot += (p.y - meanY) * (p.y - meanY);
        ssRes += (p.y - p.predicted) * (p.y - p.predicted);
    }
    rSquared = (ssTot < 1e-14) ? 0.0 : (1.0 - ssRes / ssTot);
}

double LinearRegression::calculateMSE() const {
    if (data.empty()) return 0;
    double sum = 0;
    for (const auto& p : data) {
        double err = p.y - p.predicted;
        sum += err * err;
    }
    return sum / data.size();
}

double LinearRegression::calculateMAE() const {
    if (data.empty()) return 0;
    double sum = 0;
    for (const auto& p : data) {
        sum += std::fabs(p.y - p.predicted);
    }
    return sum / data.size();
}

double LinearRegression::predict(double x) const {
    return slope * x + intercept;
}

void LinearRegression::saveAsImage(const std::string& filename) const {
    const int width = 400;
    const int height = 400;

    if (data.empty()) {
        std::ofstream file(filename);
        file << "P2\n" << width << " " << height << "\n255\n";
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                file << "255 ";
        return;
    }

    double minX = data[0].x, maxX = data[0].x;
    double minY = data[0].y, maxY = data[0].y;
    for (const auto& p : data) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
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

    std::vector<std::vector<int>> image(height, std::vector<int>(width, 255));

    for (int col = 0; col < width; col++) {
        double x = minX + (maxX - minX) * col / (width - 1);
        double y = predict(x);
        int px = col;
        int py = toPixelY(y);
        if (py >= 0 && py < height)
            image[py][px] = 180;
    }

    for (const auto& p : data) {
        int px = toPixelX(p.x);
        int py = toPixelY(p.y);
        int r = 2;
        for (int dx = -r; dx <= r; dx++)
            for (int dy = -r; dy <= r; dy++) {
                int nx = std::max(0, std::min(width - 1, px + dx));
                int ny = std::max(0, std::min(height - 1, py + dy));
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

int main() {
    LinearRegression lr;
    lr.generateSyntheticData();
    lr.fit();
    std::cout << "R² = " << lr.getRSquared() << ", MSE = " << lr.calculateMSE()
              << ", MAE = " << lr.calculateMAE() << "\n";
    lr.saveAsImage("regression.ppm");
    std::cout << "Saved visualization to regression.ppm\n";
    return 0;
}
