/* 
 *  Copyright 2010-2011 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 *  
 *  This file is part of OpenVoronoi.
 *
 *  OpenVoronoi is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenVoronoi is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with OpenVoronoi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "numeric.hpp"

#include <qd/qd_real.h> 
#include <cmath>
#include <cassert>

namespace ovd {

namespace numeric {
    
    /*
    double chop8(double a) {
        double eps = 1e-8;
        if (fabs(a) < eps)
            return 0.0;
        else
            return a;
    }*/
    double chop(double val, double tol) {
        double _epsilon = tol;
        if (fabs(val) < _epsilon) 
            return 0;
        else
            return val;
    }
    
    double chop(double val) {
        double _epsilon = 1e-10;
        if (fabs(val) < _epsilon) 
            return 0;
        else
            return val;
    }
    qd_real chop(qd_real val) {
        qd_real _epsilon = 1e-10;
        if (fabs(val) < _epsilon) 
            return qd_real(0);
        else
            return val;
    }


    double diangle(double x, double y) {
        if (y >= 0)
            return (x >= 0 ? y/(x+y) : 1-x/(-x+y));
        else
            return (x < 0 ? 2-y/(-x-y) : 3+x/(x-y));
    }
    double diangle_x(double a) {
        return (a < 2 ? 1-a : a-3);
    }
    double diangle_y(double a) {
        return (a < 3 ? ((a > 1) ? 2-a : a) : a-4);
    }


    std::pair<double,double> diangle_xy(double a) {
        double x = diangle_x(a);
        double y = diangle_y(a);
        double norm = sqrt(x*x+y*y);
        return std::make_pair(x/norm,y/norm);
    }
    // return true if a lies in [less,more]
    bool diangle_bracket(double less, double a, double more) {
        if (less==more) {
            return false;
        }else if (less<=more) { // normal case..
            if ( (less<=a) && (a<more) )
                return true;
        } else if (less>more) { // we cross zero
            if ( ((less<=a) && (a<=4)) || ((0<=a) && (a<more)) )
                return true;
        } else {
            assert(0);
            return false;
        }
        
        return false;
    }
    // return average of input angles
    double diangle_mid(double alfa1, double alfa2) {
        if (alfa1<=alfa2)
            return (alfa1+alfa2)/2;
        else {
            double opposite_mid = alfa2 + (alfa1-alfa2)/2;
            double mid = opposite_mid + 2;
            if (mid>4)
                mid=mid-4;
            assert( (0<=mid) && (mid<=4) );
            return mid;
        }
    }

    //template <class T> Ac<T>::Ac(const Ac<T> &a): p(a.p), n(a.n) {}
    //template <class T> Ac<T>::Ac() {}
    //template <class T> Ac<T>::Ac(const T &d0)
    //{ add(d0); }
    //template <class T> Ac<T>::Ac(const T &d0, const T &d1)
    //{ add(d0); add(d1); }
    //template <class T> Ac<T>::Ac(const T &d0, const T &d1, const T &d2)
    //{ add(d0); add(d1); add(d2); }

    //template <> Ac<float>::Ac(const Ac<float> &a)
    //: p(a.p), n(a.n) {}
    //template <> Ac<float>::Ac() {}
    //template <> Ac<float>::Ac(
    //    const float &d0
    //){ add(d0); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1
    //){ add(d0); add(d1); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1,
    //    const float &d2
    //){ add(d0); add(d1); add(d2); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1,
    //    const float &d2,
    //    const float &d3
    //){ add(d0); add(d1); add(d2); add(d3); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1,
    //    const float &d2,
    //    const float &d3,
    //    const float &d4
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1,
    //    const float &d2,
    //    const float &d3,
    //    const float &d4,
    //    const float &d5
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); }
    //template <> Ac<float>::Ac(
    //    const float &d0,
    //    const float &d1,
    //    const float &d2,
    //    const float &d3,
    //    const float &d4,
    //    const float &d5,
    //    const float &d6
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); add(d6); }

    //template <> Ac<double>::Ac(const Ac<double> &a)
    //: p(a.p), n(a.n) {}
    //template <> Ac<double>::Ac() {}
    //template <> Ac<double>::Ac(
    //    const double &d0
    //){ add(d0); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1
    //){ add(d0); add(d1); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1,
    //    const double &d2
    //){ add(d0); add(d1); add(d2); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1,
    //    const double &d2,
    //    const double &d3
    //){ add(d0); add(d1); add(d2); add(d3); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1,
    //    const double &d2,
    //    const double &d3,
    //    const double &d4
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1,
    //    const double &d2,
    //    const double &d3,
    //    const double &d4,
    //    const double &d5
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); }
    //template <> Ac<double>::Ac(
    //    const double &d0,
    //    const double &d1,
    //    const double &d2,
    //    const double &d3,
    //    const double &d4,
    //    const double &d5,
    //    const double &d6
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); add(d6); }

    //template <> Ac<long double>::Ac(const Ac<long double> &a)
    //: p(a.p), n(a.n) {}
    //template <> Ac<long double>::Ac() {}
    //template <> Ac<long double>::Ac(
    //    const long double &d0
    //){ add(d0); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1
    //){ add(d0); add(d1); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1,
    //    const long double &d2
    //){ add(d0); add(d1); add(d2); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1,
    //    const long double &d2,
    //    const long double &d3
    //){ add(d0); add(d1); add(d2); add(d3); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1,
    //    const long double &d2,
    //    const long double &d3,
    //    const long double &d4
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1,
    //    const long double &d2,
    //    const long double &d3,
    //    const long double &d4,
    //    const long double &d5
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); }
    //template <> Ac<long double>::Ac(
    //    const long double &d0,
    //    const long double &d1,
    //    const long double &d2,
    //    const long double &d3,
    //    const long double &d4,
    //    const long double &d5,
    //    const long double &d6
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); add(d6); }

    //template <> Ac<qd_real>::Ac(const Ac<qd_real> &a)
    //: p(a.p), n(a.n) {}
    //template <> Ac<qd_real>::Ac() {}
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0
    //){ add(d0); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1
    //){ add(d0); add(d1); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1,
    //    const qd_real &d2
    //){ add(d0); add(d1); add(d2); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1,
    //    const qd_real &d2,
    //    const qd_real &d3
    //){ add(d0); add(d1); add(d2); add(d3); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1,
    //    const qd_real &d2,
    //    const qd_real &d3,
    //    const qd_real &d4
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1,
    //    const qd_real &d2,
    //    const qd_real &d3,
    //    const qd_real &d4,
    //    const qd_real &d5
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); }
    //template <> Ac<qd_real>::Ac(
    //    const qd_real &d0,
    //    const qd_real &d1,
    //    const qd_real &d2,
    //    const qd_real &d3,
    //    const qd_real &d4,
    //    const qd_real &d5,
    //    const qd_real &d6
    //){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); add(d6); }

    //template<> void Ac<float>::clear() {
    //    p = std::vector<float>();
    //    n = std::vector<float>();
    //}
    //template<> void Ac<double>::clear() {
    //    p = std::vector<double>();
    //    n = std::vector<double>();
    //}
    //template<> void Ac<long double>::clear() {
    //    p = std::vector<long double>();
    //    n = std::vector<long double>();
    //}
    //template<> void Ac<qd_real>::clear() {
    //    p = std::vector<qd_real>();
    //    n = std::vector<qd_real>();
    //}

    //template<> void Ac<float>::add(const float &d) {
    //    if (d<0) n.push_back(d);
    //    else p.push_back(d);
    //}
    //template<> void Ac<double>::add(const double &d) {
    //    if (d<0) n.push_back(d);
    //    else p.push_back(d);
    //}
    //template<> void Ac<long double>::add(const long double &d) {
    //    if (d<0) n.push_back(d);
    //    else p.push_back(d);
    //}
    //template<> void Ac<qd_real>::add(const qd_real &d) {
    //    if (d<0) n.push_back(d);
    //    else p.push_back(d);
    //}
} // numeric
} // ovd

