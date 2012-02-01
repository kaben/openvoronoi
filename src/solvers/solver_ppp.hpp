/*  
 *  Copyright 2010-2011 Anders Wallin (anders.e.e.wallin "at" gmail.com)
 * 
 *  Idea and code for point/line/arc voronoi-vertex positioning code by
 *  Andy Payne, andy "at" payne "dot" org, November, 2010
 *  see: http://www.payne.org/index.php/Calculating_Voronoi_Nodes
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

#ifndef PPP_SOLVER_HPP
#define PPP_SOLVER_HPP

#include <algorithm>
#include <vector>
#include <cassert>

#include "solver.hpp"
#include "common/numeric.hpp"

using namespace ovd::numeric; // sq() chop()

namespace ovd {

// templated point-class, so we can use qd_real as the coordinate type.
template<class Scalar>
struct scalar_pt {
    scalar_pt<Scalar>() : x(0), y(0) {}
    scalar_pt<Scalar>(Scalar xi, Scalar yi) : x(xi), y(yi) {}
    
    Scalar x;
    Scalar y;
    double getx() {
        return x;
    }
    double gety() {
        return y;
    }
    
    scalar_pt<Scalar> &operator=(const Point& p) {
        x = p.x;
        y = p.y;
        return *this;
    }
};

// with the qd_real number-type we need to_double() to get the solution coordinates as double type.

template<>
struct scalar_pt<qd_real> {
    scalar_pt<qd_real>() : x(qd_real(0)), y(qd_real(0)) {}
    scalar_pt<qd_real>(qd_real xi, qd_real yi) : x(xi), y(yi) {}
    
    qd_real x;
    qd_real y;
    double getx() {
        return to_double(x);
    }
    double gety() {
        return to_double(y);
    }
    scalar_pt<qd_real> &operator=(const Point& p) {
        x = p.x;
        y = p.y;
        return *this;
    }
};

template <class Scalar>
inline void pn_partition(std::vector<Scalar> &pos, std::vector<Scalar> &neg, Scalar d) {
    // The "pos" partition will contain all nonnegative numbers, and
    // the "neg" partition all negative numbers.
    if(d<0) neg.push_back(-d);
    else pos.push_back(d);
}

/// point-point-point vertex positioner based on Sugihara & Iri paper
template<class Scalar>
class PPPSolver : public Solver {
public:
int solve( Site* s1, Site* s2, Site* s3,  std::vector<Solution>& slns ) {
    assert( s1->isPoint() && s2->isPoint() && s3->isPoint() );
    Point pi = s1->position();
    Point pj = s2->position();
    Point pk = s3->position();
    
    if ( pi.is_right(pj,pk) ) 
        std::swap(pi,pj);
    assert( !pi.is_right(pj,pk) );
    // 2) point pk should have the largest angle. largest angle is opposite longest side.
    double longest_side = (pi - pj).norm();
    while (  ((pj - pk).norm() > longest_side) || (((pi - pk).norm() > longest_side)) ) { 
        std::swap(pi,pj); // cyclic rotation of points until pk is opposite the longest side pi-pj
        std::swap(pi,pk);  
        longest_side = (pi - pj).norm();
    }
    assert( !pi.is_right(pj,pk) );
    assert( (pi - pj).norm() >=  (pj - pk).norm() );
    assert( (pi - pj).norm() >=  (pk - pi).norm() );
    
    // we now convert to a higher precision number-type to do the calculations
    scalar_pt<Scalar> spi,spj,spk;
    spi = pi;
    spj = pj;
    spk = pk;

    std::vector<Scalar> pos, neg;
    //Scalar J2 = (spi.y-spk.y)*( sq(spj.x-spk.x)+sq(spj.y-spk.y) )/2.0 - 
    //            (spj.y-spk.y)*( sq(spi.x-spk.x)+sq(spi.y-spk.y) )/2.0;

    //Scalar J2 = (spi.y-spk.y)*( sq(spj.x-spk.x)+sq(spj.y-spk.y) )/2.0 - 
    //sq(spj.x-spk.x)+sq(spj.y-spk.y) )
    //(spj.x-spk.x)*(spj.x-spk.x)

    //+spj.x*spj.x -2.*spj.x*spk.x +spk.x*spk.x
    //+spj.y*spj.y -2.*spj.y*spk.y +spk.y*spk.y

    //            +spj.x*spj.x
    //            -2.*spj.x*spk.x
    //            +spk.x*spk.x
    //            +spj.y*spj.y
    //            -2.*spj.y*spk.y
    //            +spk.y*spk.y

    //            +spi.y*spj.x*spj.x
    //            -2.*spi.y*spj.x*spk.x
    //            +spi.y*spk.x*spk.x
    //            +spi.y*spj.y*spj.y
    //            -2.*spi.y*spj.y*spk.y
    //            +spi.y*spk.y*spk.y
    //            -spk.y*spj.x*spj.x
    //            +2.*spk.y*spj.x*spk.x
    //            -spk.y*spk.x*spk.x
    //            -spk.y*spj.y*spj.y
    //            +2.*spk.y*spj.y*spk.y
    //            -spk.y*spk.y*spk.y

    //            +spi.y*spj.x*spj.x*0.5
    //            -spi.y*spj.x*spk.x
    //            +spi.y*spk.x*spk.x*0.5
    //            +spi.y*spj.y*spj.y*0.5
    //            -spi.y*spj.y*spk.y
    //            +spi.y*spk.y*spk.y*0.5
    //            -spk.y*spj.x*spj.x*0.5
    //            +spk.y*spj.x*spk.x
    //            -spk.y*spk.x*spk.x*0.5
    //            -spk.y*spj.y*spj.y*0.5
    //            +spk.y*spj.y*spk.y
    //            -spk.y*spk.y*spk.y*0.5

    //Scalar J2 = 
    //            +spi.y*spj.x*spj.x*0.5
    //            -spi.y*spj.x*spk.x
    //            +spi.y*spk.x*spk.x*0.5
    //            +spi.y*spj.y*spj.y*0.5
    //            -spi.y*spj.y*spk.y
    //            +spi.y*spk.y*spk.y*0.5
    //            -spk.y*spj.x*spj.x*0.5
    //            +spk.y*spj.x*spk.x
    //            -spk.y*spk.x*spk.x*0.5
    //            -spk.y*spj.y*spj.y*0.5
    //            +spk.y*spj.y*spk.y
    //            -spk.y*spk.y*spk.y*0.5
    //            -spj.y*spi.x*spi.x*0.5
    //            +spj.y*spi.x*spk.x
    //            -spj.y*spk.x*spk.x*0.5
    //            -spj.y*spi.y*spi.y*0.5
    //            +spj.y*spi.y*spk.y
    //            -spj.y*spk.y*spk.y*0.5
    //            +spk.y*spi.x*spi.x*0.5
    //            -spk.y*spi.x*spk.x
    //            +spk.y*spk.x*spk.x*0.5
    //            +spk.y*spi.y*spi.y*0.5
    //            -spk.y*spi.y*spk.y
    //            +spk.y*spk.y*spk.y*0.5

    //Scalar J2 = 
    //            +spi.y*spj.x*spj.x*0.5
    //            +spi.y*spk.x*spk.x*0.5
    //            +spi.y*spj.y*spj.y*0.5
    //            +spi.y*spk.y*spk.y*0.5
    //            -spk.y*spj.x*spj.x*0.5
    //            -spk.y*spk.x*spk.x*0.5
    //            -spk.y*spj.y*spj.y*0.5
    //            -spk.y*spk.y*spk.y*0.5
    //            -spj.y*spi.x*spi.x*0.5
    //            -spj.y*spk.x*spk.x*0.5
    //            -spj.y*spi.y*spi.y*0.5
    //            -spj.y*spk.y*spk.y*0.5
    //            +spk.y*spi.x*spi.x*0.5
    //            +spk.y*spk.x*spk.x*0.5
    //            +spk.y*spi.y*spi.y*0.5
    //            +spk.y*spk.y*spk.y*0.5
    //            -spi.y*spj.x*spk.x
    //            -spi.y*spj.y*spk.y
    //            +spk.y*spj.x*spk.x
    //            +spk.y*spj.y*spk.y
    //            +spj.y*spi.x*spk.x
    //            +spj.y*spi.y*spk.y
    //            -spk.y*spi.x*spk.x
    //            -spk.y*spi.y*spk.y

    //Scalar J3 = 
    //            +spi.x*spj.x*spj.x*0.5
    //            +spi.x*spk.x*spk.x*0.5
    //            +spi.x*spj.y*spj.y*0.5
    //            +spi.x*spk.y*spk.y*0.5
    //            -spk.x*spj.x*spj.x*0.5
    //            -spk.x*spk.x*spk.x*0.5
    //            -spk.x*spj.y*spj.y*0.5
    //            -spk.x*spk.y*spk.y*0.5
    //            -spj.x*spi.x*spi.x*0.5
    //            -spj.x*spk.x*spk.x*0.5
    //            -spj.x*spi.y*spi.y*0.5
    //            -spj.x*spk.y*spk.y*0.5
    //            +spk.x*spi.x*spi.x*0.5
    //            +spk.x*spk.x*spk.x*0.5
    //            +spk.x*spi.y*spi.y*0.5
    //            -spi.x*spj.x*spk.x
    //            -spi.x*spj.y*spk.y
    //            +spk.x*spj.x*spk.x
    //            +spk.x*spj.y*spk.y
    //            +spj.x*spi.x*spk.x
    //            +spj.x*spi.y*spk.y
    //            -spk.x*spi.x*spk.x
    //            -spk.x*spi.y*spk.y

    //Scalar J4 = (spi.x-spk.x)*(spj.y-spk.y) - (spj.x-spk.x)*(spi.y-spk.y);
    //            (spi.x-spk.x)*(spj.y-spk.y) - (spj.x-spk.x)*(spi.y-spk.y);
    //            (spi.x-spk.x)*(spj.y-spk.y)
    //            +spi.x*spj.y -spk.x*spj.y -spi.x*spk.y +spk.x*spk.y

    //            +spi.x*spj.y
    //            -spk.x*spj.y
    //            -spi.x*spk.y
    //            +spk.x*spk.y

    //Scalar J4 =
    //            +spi.x*spj.y
    //            -spk.x*spj.y
    //            -spi.x*spk.y
    //            +spk.x*spk.y
    //            -spj.x*spi.y
    //            +spk.x*spi.y
    //            +spj.x*spk.y
    //            -spk.x*spk.y

    //Scalar J2 = (spi.y-spk.y)*( sq(spj.x-spk.x)+sq(spj.y-spk.y) )/2.0 - 
    //            (spj.y-spk.y)*( sq(spi.x-spk.x)+sq(spi.y-spk.y) )/2.0;
    //Scalar J3 = (spi.x-spk.x)*( sq(spj.x-spk.x)+sq(spj.y-spk.y) )/2.0 - 
    //            (spj.x-spk.x)*( sq(spi.x-spk.x)+sq(spi.y-spk.y) )/2.0;
    //Scalar J4 = (spi.x-spk.x)*(spj.y-spk.y) - (spj.x-spk.x)*(spi.y-spk.y);
    //assert( J4 != 0.0 );
    //if (J4==0.0)
    //    std::cout << " PPPSolver: Warning divide-by-zero!!\n";
    //scalar_pt<Scalar> pt( -J2/J4 + spk.x, J3/J4 + spk.y );

    typedef Ac<Scalar> A;
    A A4(A(spi.x,-spk.x)*A(spj.y,-spk.y) - A(spj.x,-spk.x)*A(spi.y,-spk.y));
    Scalar J4(A4.get_sum());
    assert( J4 != 0.0 );
    Scalar c(0.5/J4);

    A A2(
        c*A(spj.y, -spk.y)*(A(spi.x*spi.x, -2*spi.x*spk.x, spk.x*spk.x)+A(spi.y*spi.y, -2*spi.y*spk.y, spk.y*spk.y))
      - c*A(spi.y, -spk.y)*(A(spj.x*spj.x, -2*spj.x*spk.x, spk.x*spk.x)+A(spj.y*spj.y, -2*spj.y*spk.y, spk.y*spk.y))
    );
    A2 += spk.x;
    A A3(
        c*A(spi.x, -spk.x)*(A(spj.x*spj.x, -2*spj.x*spk.x, spk.x*spk.x)+A(spj.y*spj.y, -2*spj.y*spk.y, spk.y*spk.y))
      - c*A(spj.x, -spk.x)*(A(spi.x*spi.x, -2*spi.x*spk.x, spk.x*spk.x)+A(spi.y*spi.y, -2*spi.y*spk.y, spk.y*spk.y))
    );
    A3 += spk.y;

    scalar_pt<Scalar> pt( A2.get_sum(), A3.get_sum());

    Point sln_pt = Point( pt.getx(), pt.gety());
    double dist = (sln_pt-pi).norm();
    slns.push_back( Solution(  sln_pt , dist , +1) );
    return 1;
}

};


} // ovd
#endif
