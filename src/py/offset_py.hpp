/*  
 *  Copyright 2010-2012 Anders Wallin (anders.e.e.wallin "at" gmail.com)
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
#ifndef OFFSET_PY_H
#define OFFSET_PY_H

#ifndef PYTHON_DWA2002810_HPP
#include <boost/python.hpp>
#endif

#ifndef OFFSET_H
#include "offset.hpp"
#endif
#ifndef VODI_PY_H
#include "voronoidiagram.hpp"
#endif

#include <iostream>

namespace ovd
{

class Offset_py : public Offset {
public:
    Offset_py(HEGraph& gi): Offset(gi) { }

    boost::python::list offset_py(double t) {
        offset(t);
        boost::python::list py_offsets;
        BOOST_FOREACH( Loop loop, offset_list2 ) { // loop through each loop
            boost::python::list py_loop;
            bool first = true;
            BOOST_FOREACH( Lpt lpt, loop ) { // loop through each line/arc
                boost::python::list py_lpt;
                if (first) {
                    first = false;
                    py_lpt.append( lpt.p );
                    py_lpt.append( -1 );
                } else {
                    py_lpt.append( lpt.p );
                    py_lpt.append( lpt.r );
                    py_lpt.append( lpt.c );
                    py_lpt.append( lpt.cw );
                }
                py_loop.append( py_lpt );
            }
            py_offsets.append( py_loop );
        }
        return py_offsets;
    }
private:
    Offset_py(); // don't use.
};

} // end namespace
#endif // OFFSET_PY_H
// end offset_py.hpp
