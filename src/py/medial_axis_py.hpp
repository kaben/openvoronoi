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
#ifndef MEDIAL_AXIS_PY_H
#define MEDIAL_AXIS_PY_H

#ifndef PYTHON_DWA2002810_HPP
#include <boost/python.hpp>
#endif

#ifndef MEDIAL_AXIS_H
#include "medial_axis.hpp"
#endif
#ifndef VODI_PY_H
#include "voronoidiagram.hpp"
#endif

namespace ovd
{

class MedialAxisWalk_py : public MedialAxisWalk {
public:
    MedialAxisWalk_py(HEGraph& gi): MedialAxisWalk(gi) { }

    boost::python::list walk_py() {
        walk();
        boost::python::list walk_out;
        BOOST_FOREACH( Chain chain, out ) { // loop through each chain
            boost::python::list list;
            BOOST_FOREACH( MedialPointList pt_dist_list, chain ) { // loop through each point-list
                boost::python::list point_list;
                BOOST_FOREACH( MedialPoint pt_dist, pt_dist_list ) { // loop through each Point/distance
                    boost::python::list pt;
                    pt.append( pt_dist.p );
                    pt.append( pt_dist.clearance_radius );
                    point_list.append( pt );
                }
                list.append( point_list );
            }
            walk_out.append( list );
        }
        return walk_out;
    }
private:
    MedialAxisWalk_py(); // don't use.
};

} // end namespace
#endif // MEDIAL_AXIS_PY_H
// end medial_axis_py.hpp
