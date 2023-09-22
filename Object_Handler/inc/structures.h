#pragma once
#include <iostream>
#include <cmath>

const static double EPSILON = 0.000001;

bool static IsEqual(double l, double r){
    return std::abs(l - r) < EPSILON;
}

struct Object{
    std::string name;
    double x;
    double y;
    std::string type;
    double time;
};

bool static operator<(const Object& l, const Object& r) {
    bool name = std::lexicographical_compare(l.name.begin(),l.name.end(),r.name.begin(),r.name.end());
    bool x = l.x < r.x;
    bool y = l.y < r.y;
    bool type = std::lexicographical_compare(l.type.begin(),l.type.end(),r.type.begin(),r.type.end());
    bool time = l.time > r.time;
    return name + x + y + type + time;
}

bool static operator==(const Object& l, const Object& r) {
    bool n = l.name == r.name;
    bool x = IsEqual(l.x, r.x);
    bool y = IsEqual(l.y,r.y);
    bool ty = l.type == r.type;
    bool ti = l.time == r.time;
    return n && x && y && ty && ti;
}
