#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

struct pt {
    int x, y;

    pt(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const pt& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    bool operator==(const pt& p) const {
        return x == p.x && y == p.y;
    }
};

// Orientation: 0 (COLL), 1 (CW), -1 (CCW)
int orientation(pt a, pt b, pt c) {
    int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

int distance(pt a, pt b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

pt anchor;
bool polarOrderCompare(pt a, pt b) {
    int o = orientation(anchor, a, b);
    if (o == 0) return (distance(anchor, a) < distance(anchor, b));
    return o == -1;
}

// Brute Force Algorithm
vector<pt> bruteForce(vector<pt>& pts) {
    set<pt> hull;
    int n = pts.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool valid = true;
            int side = 0;
            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;
                int o = orientation(pts[i], pts[j], pts[k]);
                if (o == 0) continue;
                if (side == 0) side = o;
                else if (side != o) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                hull.insert(pts[i]);
                hull.insert(pts[j]);
            }
        }
    }
    return vector<pt>(hull.begin(), hull.end());
}

// Graham's Scan Algorithm
vector<pt> grahamScanConvexHull(vector<pt>& pts) {
    int n = pts.size();
    if (n < 3) return {};

    // Find the anchor point (lowest y-coordinate)
    int minIdx = 0;
    for (int i = 1; i < n; i++) {
        if (pts[i].y < pts[minIdx].y || (pts[i].y == pts[minIdx].y && pts[i].x < pts[minIdx].x)) {
            minIdx = i;
        }
    }
    swap(pts[0], pts[minIdx]);
    anchor = pts[0];

    // Sort points by polar angle
    sort(pts.begin() + 1, pts.end(), polarOrderCompare);

    // Build the convex hull
    vector<pt> hull;
    for (auto p : pts) {
        while (hull.size() > 1 && orientation(hull[hull.size() - 2], hull.back(), p) != -1) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    return hull;
}

// Divide and Conquer Algorithm
void mergeHulls(vector<pt>& leftHull, vector<pt>& rightHull, vector<pt>& mergedHull) {
    int n1 = leftHull.size(), n2 = rightHull.size();

    int rightmost_leftHull = 0;
    for (int i = 1; i < n1; i++) {
        if (leftHull[i].x > leftHull[rightmost_leftHull].x)
            rightmost_leftHull = i;
    }

    int leftmost_rightHull = 0;
    for (int i = 1; i < n2; i++) {
        if (rightHull[i].x < rightHull[leftmost_rightHull].x)
            leftmost_rightHull = i;
    }

    // Find the upper tangent
    int upperLeft = rightmost_leftHull, upperRight = leftmost_rightHull;
    bool done = false;
    while (!done) {
        done = true;
        while (orientation(rightHull[upperRight], leftHull[upperLeft], leftHull[(upperLeft + 1) % n1]) == -1) {
            upperLeft = (upperLeft + 1) % n1;
        }
        while (orientation(leftHull[upperLeft], rightHull[upperRight], rightHull[(n2 + upperRight - 1) % n2]) == 1) {
            upperRight = (n2 + upperRight - 1) % n2;
            done = false;
        }
    }

    // Find the lower tangent
    int lowerLeft = rightmost_leftHull, lowerRight = leftmost_rightHull;
    done = false;
    while (!done) {
        done = true;
        while (orientation(leftHull[lowerLeft], rightHull[lowerRight], rightHull[(lowerRight + 1) % n2]) == -1) {
            lowerRight = (lowerRight + 1) % n2;
        }
        while (orientation(rightHull[lowerRight], leftHull[lowerLeft], leftHull[(n1 + lowerLeft - 1) % n1]) == 1) {
            lowerLeft = (n1 + lowerLeft - 1) % n1;
            done = false;
        }
    }

    // Merge the hulls
    mergedHull.clear();
    for (int i = 0; i <= upperLeft; i++) {
        mergedHull.push_back(leftHull[i]);
    }
    for (int i = upperRight; i != (lowerRight + 1) % n2; i = (i + 1) % n2) {
        mergedHull.push_back(rightHull[i]);
    }
    for (int i = (lowerLeft + 1) % n1; i != 0; i = (i + 1) % n1) {
        mergedHull.push_back(leftHull[i]);
    }
}

vector<pt> divideAndConquer(vector<pt> pts) {
    int n = pts.size();
    if (n <= 3) return bruteForce(pts);

    sort(pts.begin(), pts.end());

    vector<pt> leftHalf(pts.begin(), pts.begin() + n / 2);
    vector<pt> rightHalf(pts.begin() + n / 2, pts.end());

    vector<pt> leftHull = divideAndConquer(leftHalf);
    vector<pt> rightHull = divideAndConquer(rightHalf);

    vector<pt> mergedHull;
    mergeHulls(leftHull, rightHull, mergedHull);
    return mergedHull;
}

vector<pt> generateRandomPoints(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100);

    vector<pt> points;
    points.reserve(n);
    for (int i = 0; i < n; ++i) {
        points.emplace_back(dis(gen), dis(gen));
    }
    return points;
}

void writePointsToFile(const string& filename, const vector<pt>& original, const vector<pt>& hull) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    file << "Original Points:\n";
    for (const auto& p : original) {
        file << p.x << " " << p.y << "\n";
    }

    file << "\nConvex Hull:\n";
    for (const auto& p : hull) {
        file << p.x << " " << p.y << "\n";
    }
}

template<typename Func>
double measureTime(Func f, vector<pt>& points, int iterations = 100) {
    double totalTime = 0;
    for (int i = 0; i < iterations; i++) {
        vector<pt> points_copy = points;
        auto start = high_resolution_clock::now();
        f(points_copy);
        auto end = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(end - start).count();
    }
    return totalTime / iterations;
}

void performTimingAnalysis(const string& filename, const vector<pt>& points) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    file << fixed << setprecision(2);
    file << "Points BruteForce(us) DivideConquer(us) GrahamScan(us)\n";

    for (int n = 4; n <= 100; n++) {
        vector<pt> samplePoints(points.begin(), points.begin() + n);

        double timeBF = measureTime(bruteForce, samplePoints);
        double timeDC = measureTime(divideAndConquer, samplePoints);
        double timeGS = measureTime(grahamScanConvexHull, samplePoints);

        file << n << " " << timeBF << " " << timeDC << " " << timeGS << "\n";
    }
}

int main() {
    int NUM_POINTS = 100;

    vector<pt> points = generateRandomPoints(NUM_POINTS);
    vector<pt> hull = grahamScanConvexHull(points);

    writePointsToFile("points.txt", points, hull);
    performTimingAnalysis("timing.txt", points);

    cout << "Convex hull written to points.txt\n"
        << "Timing results written to timing.txt\n"
        << "Use timing.txt to plot complexity comparisons\n";


    return 0;
}