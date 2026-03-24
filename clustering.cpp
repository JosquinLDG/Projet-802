#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

struct Point {
    double x, y;
};

double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

vector<Point> Clustering(const vector<double>& X, const vector<double>& Y, double threshold) {
    int n = X.size();
    vector<Point> points(n);
    
    for (int i = 0; i < n; i++) {
        points[i] = {X[i], Y[i]};
    }

    vector<bool> visited(n, false);
    vector<Point> centers;

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;

        vector<Point> cluster;
        cluster.push_back(points[i]);
        visited[i] = true;

        // regroupe les points proches
        for (int j = 0; j < n; j++) {
            if (!visited[j] && distance(points[i], points[j]) < threshold) {
                cluster.push_back(points[j]);
                visited[j] = true;
            }
        }

        // calcule le centre
        double sumX = 0, sumY = 0;
        for (auto& p : cluster) {
            sumX += p.x;
            sumY += p.y;
        }

        centers.push_back({sumX / cluster.size(), sumY / cluster.size()});
    }

    return centers;
}
int main() {
    vector<double> X = {
    -0.49186552, -0.46013226, -0.46013226, -0.428399,   -0.07933315, -0.04759989,
 -0.01586663,  0.01586663,  0.04759989,  0.07933315,  0.11106641,  1.1265307,
  1.15826396,  1.09479744,  1.1265307,   1.15826396 , 1.06306418,  1.09479744 
};
    vector<double> Y = {
-2.32816779, -2.32816779, -1.39690068, -1.39690068, 13.5033732,  14.43464031,
 14.43464031, 15.36590743, 15.36590743 ,16.29717455, 16.29717455, 20.95351013,
 20.95351013 ,21.88477725 ,22.81604436, 22.81604436, 23.74731148, 23.74731148
};

    double threshold = 4.0; // à ajuster !

    vector<Point> centers = Clustering(X, Y, threshold);

    for (auto& c : centers) {
        cout << c.x << " , " << c.y << endl;
    }

    return 0;
}