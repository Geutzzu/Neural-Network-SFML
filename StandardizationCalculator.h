#pragma once

#include <cmath>

class StandardizationCalculator {
private:
    int n;
    double mean;
    double M2;

public:
    StandardizationCalculator() : n(0), mean(0), M2(0) {}

    void add(double x) {
        n++;
        double delta = x - mean;
        mean += delta / n;
        M2 += delta * (x - mean);
    }

    double getMean() const {
        return mean;
    }

    double getVariance() const {
        if (n < 2) {
            return 0;
        }
        return M2 / (n - 1);
    }

    double getStandardDeviation() const {
        return std::sqrt(getVariance());
    }

	void reset() {
		n = 0;
		mean = 0;
		M2 = 0;
	}

};