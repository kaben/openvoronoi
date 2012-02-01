/*  
 *  Copyright 2010 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 *  
 *  This file is part of OpenVoronoi.
 *
 *  OpenCAMlib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCAMlib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenCAMlib.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath> // sqrt, sin, cos, fabs
#include <cassert>
#include <sstream>

#include "point.hpp"
#include "numeric.hpp"

using namespace ovd::numeric;
namespace ovd {

double Point::norm() const {
    return sqrt( x*x + y*y  );
}

double Point::norm_sq() const {
    return x*x + y*y;
}


double Point::dot(const Point &p) const {
    return x * p.x + y * p.y ; 
}

double Point::cross(const Point &p) const {
    return x * p.y - y * p.x ; 
}

void Point::normalize() {
    if (this->norm() != 0.0)
        *this *=(1/this->norm());
}

Point Point::xy_perp() const {
    return Point(-y, x);
    // 2D rotation matrix:
    //   cos   -sin
    //   sin   cos
    // for theta = 90
    //   0   -1   ( x )
    //   1    0   ( y )  = ( -y  x )
    
}

double Point::distance_to_line(const Point &p1, const Point &p2) const {
    // see for example
    // http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
    if ((p1.x == p2.x) && (p1.y == p2.y)) {// no line in xy plane
        std::cout << "point.cpp: xyDistanceToLine ERROR!: can't calculate distance from \n";
        std::cout << "point.cpp: xyDistanceToLine ERROR!: *this ="<<*this <<" to line through\n";
        std::cout << "point.cpp: xyDistanceToLine ERROR!: p1="<<p1<<" and \n";
        std::cout << "point.cpp: xyDistanceToLine ERROR!: p2="<<p2<< "\n";
        std::cout << "point.cpp: xyDistanceToLine ERROR!: in the xy-plane\n";
        return -1;
    }
    else {
        Point v = Point(p2.y-p1.y, -(p2.x-p1.x) );
        v.normalize();
        Point r = Point(p1.x - x, p1.y - y );
        return fabs( v.dot(r));
    }
}

bool Point::is_right(const Point &p1, const Point &p2) const {
    // is Point right of line through points p1 and p2 ?, in the XY plane.
    // this is an ugly way of doing a determinant
    // should be prettyfied sometime...
    /// \todo FIXME: what if p1==p2 ? (in the XY plane)

    //double a1 = p2.x - p1.x;
    //double a2 = p2.y - p1.y;
    //double t1 = a2;
    //double t2 = -a1;
    //double b1 = x - p1.x;
    //double b2 = y - p1.y;

    //double t = t1 * b1 + t2 * b2;
    //if (t > 0.0) 
    //    return true;
    //else
    //    return false;    

    // <comment 20120130 by kaben>
    // We use the cross product b x a, with vectors b:=(*this-p1) and
    // a:=(p2-p1) in the XY plane of R^3, to decide whether *this is to
    // the right of (p2-p1); this is equivalent to determining whether
    // t, defined as the Z coordinate of b x a, is positive.
    //
    // Note: if p1==p2 then the cross product is the zero vector, in
    // which case is_right() should return false (and if there were an
    // is_left(), it should also return false).
    //
    // I believe that in the code above, subtraction following rounding
    // errors in multiplication can lead to loss of significance through
    // subtractive cancellation when *this, p1, and p2 are very
    // close-together, and (*this-p1) and (p2-p1) are nearly parallel.
    //
    // The code below addresses this in two ways:
    // - it avoids subtractive cancellation by avoiding subtraction;
    // - it preserves as much significance as possible by summing
    //   positives from smallest to largest in magnitude, and similarly
    //   but separately summing negatives.
    //
    // Caveat: I am not an expert in numerical analysis, so if I'm
    // talking nonsense, please say so.
    // </comment 20120130 by kaben>

    // Notes:
    // double bx(x-p1.x), by(y-p1.y);
    // double ax(p2.x-p1.x), ay(p2.y-p1.y);
    // double t(bx*ay-by*ax);
    // so t == (x-p1.x)*(p2.y-p1.y)-(y-p1.y)*(p2.x-p1.x)
    //      ==  x*p2.y +p1.x*p1.y +p1.y*p2.x +y*p1.x 
    //        - y*p2.x -p1.y*p1.x -p1.x*p2.y -x*p1.y.

    // collect coordinate scalars into pos and neg partitions; in both
    // partitions we just store abs values to make them easier to sort,
    // sum, and compare.

    typedef Ac<double> Acd;
    Acd t(Acd(x, -p1.x)*Acd(p2.y, -p1.y) - Acd(y, -p1.y)*Acd(p2.x, -p1.x));
    double pos_sum, neg_sum;
    t.get_sum(pos_sum, neg_sum);
    return (-neg_sum < pos_sum);
}

/* **************** Operators ***************  
 *  see
 *  http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
*/

Point& Point::operator=(const Point &p) {
    if (this == &p)
        return *this;
    x=p.x;
    y=p.y;
    return *this;
}

// Point*scalar multiplication
Point& Point::operator*=(const double &a) {
    x*=a;
    y*=a;
    return *this;
}

Point& Point::operator+=(const Point &p) {
    x+=p.x;
    y+=p.y;
    return *this;
}

Point& Point::operator-=(const Point &p) {
    x-=p.x;
    y-=p.y;
    return *this;
}

const Point Point::operator+(const Point &p) const {
    return Point(*this) += p;
}

const Point Point::operator-(const Point &p) const {
    return Point(*this) -= p;
}

const Point Point::operator*(const double &a) const {
    return Point(*this) *= a;
}

// scalar*Point
const Point operator*(const double &a, const Point &p) {
    return Point(p) *= a;
}

bool Point::operator==(const Point &p) const {
    return (this == &p) || (x==p.x && y==p.y ); 
}

bool Point::operator!=(const Point &p) const {
    return !(*this == p);
}

std::string Point::str() const {
    std::ostringstream o;
    o << *this;
    return o.str();
}

std::ostream& operator<<(std::ostream &stream, const Point& p) {
  stream << "(" << p.x << ", " << p.y << ")"; 
  return stream;
}

} // end namespace
// end file point.cpp
