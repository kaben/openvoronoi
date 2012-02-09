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

#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include <algorithm>
#include <vector>
#include <qd/qd_real.h> 

namespace ovd {

// this namespace holds general numerical functions that are not specific
// to voronoi-diagrams and may be useful elsewhere too
namespace numeric {
    //double chop8(double a);
    double chop(double val);
    double chop(double val, double tolerance);
    qd_real chop(qd_real val);


    // accumulator class to collect scalars, and later sum them in an
    // order that tries to maintain as much precision as possible.

    // To help with testing I've added an addition policy for the accumulator
    // class. A testing-addition policy can be used to verify order of
    // summation.
    template <class T>
    struct SimpleAdd {
        void plus_eq(T& a, const T& b) { a += b; }
    };

    template <class Scalar, template <class> class AddPolicy = SimpleAdd>
    struct Ac : AddPolicy<Scalar> {
        std::vector<Scalar> p, n;
        // Copy constructor.
        Ac(const Ac &a): p(a.p), n(a.n) {}
        // Constructors taking up to six Scalar arguments to be summed.
        Ac()
        {}
        Ac(const Scalar &d0)
        { add(d0); }
        Ac(const Scalar &d0, const Scalar &d1)
        { add(d0); add(d1); }
        Ac(const Scalar &d0, const Scalar &d1, const Scalar &d2)
        { add(d0); add(d1); add(d2); }
        Ac(const Scalar &d0, const Scalar &d1, const Scalar &d2,
           const Scalar &d3
        ){ add(d0); add(d1); add(d2); add(d3); }
        Ac(const Scalar &d0, const Scalar &d1, const Scalar &d2,
           const Scalar &d3, const Scalar &d4
        ){ add(d0); add(d1); add(d2); add(d3); add(d4); }
        Ac(const Scalar &d0, const Scalar &d1, const Scalar &d2,
           const Scalar &d3, const Scalar &d4, const Scalar &d5
        ){ add(d0); add(d1); add(d2); add(d3); add(d4); add(d5); }

        void clear() {
            p = std::vector<Scalar>();
            n = std::vector<Scalar>();
        };
        void add(const Scalar &d) {
            if (d<0) n.push_back(d);
            else p.push_back(d);
        }
        void sum(Scalar &pos, Scalar &neg) {
            pos = 0.; neg = 0.;
            sort(p.begin(), p.end());
            sort(n.begin(), n.end());
            reverse(n.begin(), n.end());
            for (unsigned int i=0; i<p.size(); i++) { plus_eq(pos, p[i]); }
            for (unsigned int i=0; i<n.size(); i++) { plus_eq(neg, n[i]); }
        }
        Scalar sum() {
            Scalar pos(0), neg(0);
            sum(pos, neg);
            return pos + neg;
        }
        void operator +=(const Scalar &d) { add(d); }
        void operator +=(const Ac &a) {
            for (unsigned int i=0; i<a.p.size(); i++) p.push_back(a.p[i]);
            for (unsigned int i=0; i<a.n.size(); i++) n.push_back(a.n[i]);
        }
        void operator -=(const Scalar &d) { add(-d); }
        void operator -=(const Ac &a) {
            for (unsigned int i=0; i<a.p.size(); i++) n.push_back(-a.p[i]);
            for (unsigned int i=0; i<a.n.size(); i++) p.push_back(-a.n[i]);
        }
        void operator =(const Scalar &d) { clear(); add(d); }
        void operator =(const Ac &a) { p = a.p; n = a.n; }
    };

    // For vectors "a" and "b", distributes multiplication of a and b,
    // storing the result in "result". Please make sure to clear() result
    // before using product().
    // 
    // To make this description a little less-opaque: after calling
    // product(a, b, result), result will "equal" a * b.  Like a and b,
    // result uses accumulator vectors to store intermediate results that
    // will be summed sometime later.
    template <class Scalar>
    void product(std::vector<Scalar> &a, std::vector<Scalar> &b, Ac<Scalar> &result) {
        for (unsigned int i=0; i<a.size(); i++)
            for (unsigned int j=0; j<b.size(); j++)
                result.add(a[i]*b[j]);
    }
    template <class Scalar>
    Ac<Scalar> operator *(Ac<Scalar> a, Scalar d) {
        Ac<Scalar> ac;
        for (unsigned int i=0; i<a.p.size(); i++) ac.add(a.p[i]*d);
        for (unsigned int i=0; i<a.n.size(); i++) ac.add(a.n[i]*d);
        return ac;
    }
    template <class Scalar>
    Ac<Scalar> operator *(Scalar d, Ac<Scalar> a) { return a*d; }
    template <class Scalar>
    Ac<Scalar> operator *(Ac<Scalar> a, Ac<Scalar> b) {
        Ac<Scalar> ac;
        product(a.p, b.p, ac); product(a.n, b.p, ac);
        product(a.p, b.n, ac); product(a.n, b.n, ac);
        return ac;
    }
    template <class Scalar>
    Ac<Scalar> operator /(Ac<Scalar> a, Scalar d) {
        Ac<Scalar> ac;
        Scalar d_inv(Scalar(1.)/d);
        for (unsigned int i=0; i<a.p.size(); i++) ac.add(a.p[i]*d_inv);
        for (unsigned int i=0; i<a.n.size(); i++) ac.add(a.n[i]*d_inv);
        return ac;
    }
    // Note: I'm holding-off implementation of operator /(Ac, Ac) until we
    // add error estimation to class Ac.
    /*
    template <class Scalar>
    Ac<Scalar> operator /(Ac<Scalar> a, Ac<Scalar> d);
    */
    template <class Scalar>
    Ac<Scalar> operator +(Ac<Scalar> a, Ac<Scalar> b) {
        Ac<Scalar> ac;
        for (unsigned int i=0; i<a.p.size(); i++) ac.p.push_back(a.p[i]);
        for (unsigned int i=0; i<a.n.size(); i++) ac.n.push_back(a.n[i]);
        for (unsigned int i=0; i<b.p.size(); i++) ac.p.push_back(b.p[i]);
        for (unsigned int i=0; i<b.n.size(); i++) ac.n.push_back(b.n[i]);
        return ac;
    }
    template <class Scalar>
    Ac<Scalar> operator -(Ac<Scalar> a, Ac<Scalar> b) {
        Ac<Scalar> ac;
        for (unsigned int i=0; i<a.p.size(); i++) ac.p.push_back(a.p[i]);
        for (unsigned int i=0; i<a.n.size(); i++) ac.n.push_back(a.n[i]);
        for (unsigned int i=0; i<b.p.size(); i++) ac.n.push_back(-b.p[i]);
        for (unsigned int i=0; i<b.n.size(); i++) ac.p.push_back(-b.n[i]);
        return ac;
    }
    template <class Scalar>
    Ac<Scalar> operator -(Ac<Scalar> a) {
        Ac<Scalar> ac;
        for (unsigned int i=0; i<a.p.size(); i++) ac.n.push_back(-a.p[i]);
        for (unsigned int i=0; i<a.n.size(); i++) ac.p.push_back(-a.n[i]);
        return ac;
    }


    template<class Scalar>
    Scalar sq( Scalar x) {return x*x;}
    
    /// solve quadratic eqn: a*x*x + b*x + c = 0
    /// returns real roots (0, 1, or 2) as vector
    template<class Scalar>
    std::vector<Scalar>  quadratic_roots(Scalar a, Scalar b, Scalar c) {
        std::vector<Scalar> roots;
        if ((a == 0) and (b == 0)) {
            //std::cout << " quadratic_roots() a == b == 0. no roots.\n";
            return roots;
        }
        if (a == 0) {
            roots.push_back( -c / b );
            return roots;
        }
        if (b == 0) {
            Scalar sqr = -c / a;
            if (sqr > 0) {
                roots.push_back( sqrt(sqr) );
                roots.push_back( -roots[0] );
                return roots;
            } else if (sqr == 0) {
                roots.push_back( Scalar(0) );
                return roots;
            } else {
                //std::cout << " quadratic_roots() b == 0. no roots.\n";
                return roots;
            }
        }
        Scalar disc = chop(b*b - 4*a*c); // discriminant, chop!
        if (disc > 0) {
            Scalar q;
            if (b > 0)
                q = (b + sqrt(disc)) / -2;
            else
                q = (b - sqrt(disc)) / -2;
            roots.push_back( q / a );
            roots.push_back( c / q ); 
            return roots;
        } else if (disc == 0) {
            roots.push_back( -b / (2*a) );
            return roots;
        }
        //std::cout << " quadratic_roots() disc < 0. no roots. disc= " << disc << "\n";
        return roots;
    }
    
    template <class Scalar>
    inline Scalar determinant( Scalar a, Scalar b, Scalar c,
                        Scalar d, Scalar e, Scalar f,
                        Scalar g, Scalar h, Scalar i ) {
        return a*(e*i-h*f)-b*(d*i-g*f)+c*(d*h-g*e);
    }
    
    double diangle(double x, double y);
    double diangle_x(double a);
    double diangle_y(double a);
    std::pair<double,double> diangle_xy(double a);
    bool diangle_bracket(double less, double a, double more);
    double diangle_mid(double alfa1, double alfa2);
    
} // numeric
} // ovd

#endif
