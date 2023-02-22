#ifndef IMAGES_H
#define IMAGES_H

#include "color.h"
#include "coordinate.h"

#include <functional>
#include <type_traits>
#include <cmath>

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t) {
    return [t](const Point) {
        return t;
    };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
    return [image, phi](const Point p) {
        const Point q = (p.is_polar ? p : to_polar(p));
        return image(Point(q.first, q.second - phi, true));
    };
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
    return [image, v](const Point p) {
        const Point q = (!p.is_polar ? p : from_polar(p));
        return image(Point(q.first - v.first, q.second - v.second, false));
    };
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
    return [image, s](const Point p) {
        return (p.is_polar ? image(Point(p.first / s, p.second, true))
                           : image(Point(p.first / s, p.second / s, false)));
    };
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
    return [q, r, inner, outer](const Point p) {
        const Point s = (!q.is_polar ? q : from_polar(q));
        const Point t = (!p.is_polar ? p : from_polar(p));
        return distance(s, t) <= r ? inner : outer;
    };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
    return [d, this_way, that_way](const Point p) {
        const Point q = (!p.is_polar ? p : from_polar(p));
        return ((static_cast<int>(floor(q.first / d))
                + static_cast<int>(floor(q.second / d))) % 2 == 0
                ? this_way : that_way);
    };
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    return [d, n, this_way, that_way](const Point p) {
        const Point q = (p.is_polar ? p : to_polar(p));
        return checker(d, this_way, that_way)
            (Point(q.first, d * q.second / (2 * M_PI / n), false));
    };
}

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    return [q, d, this_way, that_way](const Point p) {
        const Point s = (!q.is_polar ? q : from_polar(q));
        const Point t = (!p.is_polar ? p : from_polar(p));
        return (static_cast<int>(floor(distance(s, t) / d)) % 2 == 0
                ? this_way : that_way);
    };
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [d, this_way, that_way](const Point p) {
        const Point q = (!p.is_polar ? p : from_polar(p));
        return (std::abs(2 * q.first) <= d ? this_way : that_way);
    };
}

inline Image cond(Region region, Image this_way, Image that_way) {
    return [region, this_way, that_way](const Point p) {
        return region(p) ? this_way(p) : that_way(p);
    };
}

inline Image lerp(Blend blend, Image this_way, Image that_way) {
    return [blend, this_way, that_way](const Point p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
}

inline Image darken(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::black));
}

inline Image lighten(Image image, Blend blend) {
    return lerp(blend, image, constant(Colors::white));
}

#endif //IMAGES_H
