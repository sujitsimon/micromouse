// bezier.h
#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <tuple>

class Bezier {
    private:
        const int LENGTH = 18;
        double distance_2d(std::tuple<double, double> point1, std::tuple<double, double> point2);

    public:
        std::vector<double> quad_bezier(double p0, double p1, double cp);
        void motor_control(const std::vector<double>& x_curve, const std::vector<double>& y_curve);
        std::tuple<double, double> process_tuple(std::tuple<double, double> p);
        void bezier_draw(std::tuple<double, double> p0, std::tuple<double, double> p1, std::tuple<double, double> cp);
}

#endif // BEZIER_H
