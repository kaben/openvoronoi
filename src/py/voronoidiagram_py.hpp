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
#ifndef VODI_PY_H
#define VODI_PY_H

// from where is boost::python included?
#include "voronoidiagram.hpp"
#include "vertex.hpp"
#include "facegrid.hpp"
#include "common/numeric.hpp"

using namespace ovd::numeric;

namespace ovd
{

// to allow reading/writing Point objects using pickle
struct point_pickle_suite : boost::python::pickle_suite {
    static boost::python::tuple getinitargs(Point const& p) {
        return boost::python::make_tuple( p.x, p.y );
    }
};

/// \brief python wrapper for VoronoiDiagram
class VoronoiDiagram_py : public VoronoiDiagram {
public:
    /// create diagram with given far-radius and number of bins
    VoronoiDiagram_py(double far, unsigned int n_bins) 
        : VoronoiDiagram( far, n_bins) {
        _edge_points=40;
        null_edge_offset=0.01;
    }
    
    int insert_point_site1(const Point& p) {
        return insert_point_site(p);
    }
    int insert_point_site2(const Point& p, int step) {
        return insert_point_site(p,step);
    }
    
    bool insert_line_site2(int idx1, int idx2) {
        return insert_line_site( idx1, idx2);
    }
    bool insert_line_site3(int idx1, int idx2, int step) {
        return insert_line_site( idx1, idx2, step);
    }
    void set_null_edge_offset(double ofs) {
        null_edge_offset=ofs;
    }
    /// return list of generators to python
    boost::python::list getGenerators()  {
        boost::python::list plist;
        BOOST_FOREACH( HEVertex v, g.vertices() ) {
            if ( g[v].type == POINTSITE ) {
                boost::python::list pd;
                Point offset(0,0);
                //double ofs= 0.01;
                if ( (g[v].alfa!=-1) ) { 
                    boost::tie(offset.x, offset.y )  = numeric::diangle_xy( g[v].alfa );
                    //offset.x = null_edge_offset*numeric::diangle_x( g[v].alfa );
                    //offset.y = null_edge_offset*numeric::diangle_y( g[v].alfa );
                }
                    
                pd.append( g[v].position+offset*null_edge_offset );
                pd.append( g[v].dist() );
                pd.append( g[v].status );
                pd.append( g[v].index );
                plist.append(pd);
            }
        }
        return plist;
    }
    boost::python::list getStat() {
        boost::python::list elist;
        BOOST_FOREACH( double  e, vpos->get_stat() ) {
            elist.append(e);
        }
        return elist;
    }
    
    /// return list of vd vertices to python
    boost::python::list getVoronoiVertices()  {
        boost::python::list plist;
        BOOST_FOREACH( HEVertex v, g.vertices() ) {
            if ( g[v].type == NORMAL || g[v].type == ENDPOINT || 
               g[v].type == SEPPOINT || g[v].type == APEX || 
               g[v].type == OUTER || g[v].type == SPLIT) {
                boost::python::list pd;
                Point offset(0,0);
                //double ofs= 0.01;
                if ( (g[v].alfa!=-1) ) {
                    boost::tie(offset.x, offset.y )  = numeric::diangle_xy( g[v].alfa ); 
                    //offset.x = null_edge_offset*numeric::diangle_x( g[v].alfa );
                    //offset.y = null_edge_offset*numeric::diangle_y( g[v].alfa );
                }
                
                pd.append( g[v].position+offset*null_edge_offset );
                pd.append( g[v].dist() );
                pd.append( g[v].status );
                pd.append( g[v].index );
                plist.append(pd);
            }
        }
        return plist;
    }
    /// return list of the three special far-vertices to python
    boost::python::list getFarVoronoiVertices()  {
        boost::python::list plist;
        BOOST_FOREACH( HEVertex v, g.vertices() ) {
            if ( g.degree( v ) == 4 ) {
                boost::python::list pd;
                pd.append( g[v].position );
                pd.append( g[v].dist() );
                plist.append(pd);
            }
        }
        return plist;
    }
    /// return list of vd-edges to python
    boost::python::list getVoronoiEdges()  {
        boost::python::list edge_list;
        BOOST_FOREACH( HEEdge edge, g.edges() ) { // loop through each edge
                if (!g[edge].valid) continue;
                boost::python::list edge_data;
                boost::python::list point_list; // the endpoints of each edge
                HEVertex v1 = g.source( edge );
                HEVertex v2 = g.target( edge );
                // these edge-types are drawn as a single line from source to target.
                if ( (g[edge].type == SEPARATOR) || (g[edge].type == LINE) || 
                     (g[edge].type == LINESITE) || (g[edge].type == OUTEDGE) || 
                     (g[edge].type == LINELINE)  || (g[edge].type == PARA_LINELINE)) {
                    point_list.append( g[v1].position );
                    point_list.append( g[v2].position );
                } else if ( g[edge].type == PARABOLA ) { // these edge-types are drawn as polylines with edge_points number of points
                    double t_src = g[v1].dist();
                    double t_trg = g[v2].dist();
                    double t_min = std::min(t_src,t_trg);
                    double t_max = std::max(t_src,t_trg);
                    for (int n=0;n< _edge_points;n++) {
                        //double t = t_min + n*(t_max-t_min)/(_edge_points-1); // linear
                        double t = t_min + ((t_max-t_min)/sq(_edge_points-1))*sq(n);
                        Point pt = g[edge].point(t);
                        point_list.append(pt);
                    }
                    
                } else {
                    //assert(0);
                } 
                edge_data.append( point_list );
                edge_data.append( g[edge].type );
                edge_data.append( g[v1].status ); // source status
                edge_data.append( g[v2].status ); // target status
                edge_list.append( edge_data );
        }
        return edge_list;
    }
        

    
    // NOTE: no g[edge].valid check here!?
    boost::python::list getVoronoiEdgesOffset()  {
        boost::python::list edge_list;
        BOOST_FOREACH( HEEdge edge, g.edges() ) { // loop through each edge
                boost::python::list edge_data;
                boost::python::list point_list; // the endpoints of each edge
                HEVertex v1 = g.source( edge );
                HEVertex v2 = g.target( edge );
                // these edge-types are drawn as a single line from source to target.
                if ( (g[edge].type == SEPARATOR) || (g[edge].type == LINE) || 
                     (g[edge].type == LINESITE) || (g[edge].type == OUTEDGE) || 
                     (g[edge].type == LINELINE)  || (g[edge].type == PARA_LINELINE)  ) {
                    Point v1_offset(0,0);
                    Point v2_offset(0,0);
                    //double ofs= 0.01;
                    if ( (g[v1].alfa!=-1) ) { // || (g[v2].alfa!=-1) ) {
                        boost::tie(v1_offset.x, v1_offset.y )  = numeric::diangle_xy( g[v1].alfa );
                        //v1_offset.y = null_edge_offset*numeric::diangle_y( g[v1].alfa );
                    }
                    if ( (g[v2].alfa!=-1) ) { // || (g[v2].alfa!=-1) ) {
                        boost::tie(v2_offset.x, v2_offset.y ) = numeric::diangle_xy( g[v2].alfa );
                        //= null_edge_offset*numeric::diangle_y( g[v2].alfa );
                    }
                    point_list.append( g[v1].position + v1_offset*null_edge_offset );
                    point_list.append( g[v2].position + v2_offset*null_edge_offset );
                } else if ((g[edge].type == NULLEDGE)) {
                    //int nmax=20;
                    double dalfa = 0.1;
                    if ( g[v1].alfa < g[v2].alfa ) {// the normal case
                        int nmax = (int)((g[v2].alfa-g[v1].alfa)/dalfa);
                        for(int n=0;n<nmax;n++) {
                            double alfa = g[v1].alfa + n*(g[v2].alfa-g[v1].alfa)/(nmax-1);
                            Point offset(0,0);
                            boost::tie(offset.x,offset.y) = numeric::diangle_xy( alfa );
                            point_list.append( g[v1].position + offset*null_edge_offset );
                        }
                    } else {
                        // go via zero
                        int nmax = std::max( (int)((4-g[v1].alfa)/dalfa), 5 ); // from v1 to zero
                        for(int n=0;n<nmax;n++) {
                            double alfa = g[v1].alfa + n*(4-g[v1].alfa)/(nmax-1);
                            Point offset(0,0);
                            boost::tie(offset.x,offset.y) = numeric::diangle_xy( alfa );
                            point_list.append( g[v1].position + offset*null_edge_offset );
                        }
                        nmax = std::max( (int)((g[v2].alfa-0)/dalfa), 5 );
                        for(int n=0;n<nmax;n++) { // from zero to v2
                            double alfa = 0 + n*(g[v2].alfa-0)/(nmax-1);
                            Point offset(0,0);
                            boost::tie(offset.x,offset.y) = numeric::diangle_xy( alfa );
                            point_list.append( g[v1].position + offset*null_edge_offset );
                        }
                    }
                    
                } else if ( g[edge].type == PARABOLA ) { // these edge-types are drawn as polylines with edge_points number of points
                    double t_src = g[v1].dist();
                    double t_trg = g[v2].dist();
                    double t_min = std::min(t_src,t_trg);
                    double t_max = std::max(t_src,t_trg);
                    for (int n=0;n< _edge_points;n++) {
                        //double t = t_min + n*(t_max-t_min)/(_edge_points-1); // linear
                        double t = t_min + ((t_max-t_min)/sq(_edge_points-1))*sq(n);
                        Point pt = g[edge].point(t);
                        point_list.append(pt);
                    }
                    
                } else {
                    //assert(0);
                } 
                edge_data.append( point_list );
                edge_data.append( g[edge].type );
                edge_data.append( g[v1].status ); // source status
                edge_data.append( g[v2].status ); // target status
                edge_list.append( edge_data );
        }
        return edge_list;
    }
    
    /// return edges and generators to python
    boost::python::list getEdgesGenerators()  {
        boost::python::list edge_list;
        BOOST_FOREACH( HEEdge edge, g.edges() ) {
                boost::python::list point_list; // the endpoints of each edge
                HEVertex v1 = g.source( edge );
                HEVertex v2 = g.target( edge );
                Point src = g[v1].position;
                Point tar = g[v2].position;
                int src_idx = g[v1].index;
                int trg_idx = g[v2].index;
                point_list.append( src );
                point_list.append( tar );
                point_list.append( src_idx );
                point_list.append( trg_idx );
                edge_list.append(point_list);
        }
        return edge_list;
    }

    // for animation/visualization only, not needed in main algorithm
    EdgeVector find_in_in_edges() { 
        assert( !v0.empty() );
        EdgeVector output; // new vertices generated on these edges
        BOOST_FOREACH( HEVertex v, v0 ) {                                   
            assert( g[v].status == IN ); // all verts in v0 are IN
            BOOST_FOREACH( HEEdge edge, g.out_edges( v ) ) {
                HEVertex adj_vertex = g.target( edge );
                if ( g[adj_vertex].status == IN ) 
                    output.push_back(edge); // this is an IN-IN edge
            }
        }
        return output;
    }
    void set_edge_points(int n) { _edge_points=n; }

    // count edges, counting apex-split edges as one
    unsigned int num_face_edges( HEFace f) {
        HEEdge start_edge = g[f].edge;
        HEEdge current_edge = start_edge;
        EdgeVector out;
        //std::cout << " edges on face " << f << " :\n ";
        do {
            //Vertex src = source(current_edge);
            HEVertex trg = g.target(current_edge);
            //   std::cout << out.size() << " " << g[src].index << "[" << g[src].type <<"]";
            //   std::cout << " - " << g[trg].index << "[" << g[trg].type <<"]" <<"\n ";
            if (g[trg].type != APEX )
                out.push_back(current_edge);
            current_edge = g[current_edge].next;
        } while( current_edge != start_edge );
        return out.size();
    }    

    boost::python::list getFaceStats()  {
        boost::python::list stats;
        const double radius_limit = 0.7;
        for(HEFace f=0 ; f < g.num_faces() ; f++ ) {
            // only accept if site is within radius
            Point pt = g[f].site->position();
            if ( pt.norm() < radius_limit ) {
                boost::python::list data;
                data.append( f );
                data.append( g[f].site->position() );  
                data.append( num_face_edges(f) );
                stats.append(data);
            }
        }
        return stats;
    }
private:
    int _edge_points; // number of points to plot on quadratic edges
    double null_edge_offset;
};


} // end namespace
#endif
// end voronoidiagram_py.h
