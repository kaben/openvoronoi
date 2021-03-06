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
#ifndef VORONOI_DIAGRAM_HPP
#define VORONOI_DIAGRAM_HPP

#include <queue>
#include <set>
#include <boost/tuple/tuple.hpp>

#include "common/point.hpp"
#include "graph.hpp"
#include "vertex_positioner.hpp"

namespace ovd
{

class VoronoiDiagramChecker;
class FaceGrid;

// in augment_vertex_set() we grow the delete-tree by processing vertices
// one-by-one from a priority_queue. This is the priority_queue sort predicate.
// We handle vertices with a large fabs( in_circle() ) first, since we 
// believe their predicate to be more reliable.
typedef std::pair<HEVertex, double> VertexDetPair;
class abs_comparison {
public:
  bool operator() (const VertexDetPair& lhs, const VertexDetPair&rhs) const {
    return ( fabs(lhs.second) < fabs(rhs.second) );
  }
};

// vertices for processing held in this queue
// sorted by decreasing fabs() of in_circle-predicate, so that the vertices whose IN/OUT status we are 'most certain' about are processed first
typedef std::priority_queue< VertexDetPair , std::vector<VertexDetPair>, abs_comparison > VertexQueue;

// this struct used in add_edge() for storing information related to
// the new edge.
struct EdgeData {
    HEEdge v1_prv;
    HEVertex v1; // NEW edge source
    HEEdge v1_nxt;
    HEEdge v2_prv;
    HEVertex v2; // NEW edge target
    HEEdge v2_nxt;
    HEFace f;
};

/// \brief Voronoi diagram.
///
/// see http://en.wikipedia.org/wiki/Voronoi_diagram
/// 
/// the dual of a voronoi diagram is the delaunay diagram(triangulation).
///  voronoi-faces are dual to delaunay-vertices.
///  vornoi-vertices are dual to delaunay-faces 
///  voronoi-edges are dual to delaunay-edges
class VoronoiDiagram {
    public:
        /// ctor
        /// create diagram with given far-radius and number of bins
        /// \param far radius of circle centered at (0,0) within which all sites must lie
        /// \param n_bins number of bins used for nearest vd-vertex bucket-search
        VoronoiDiagram(double far, unsigned int n_bins);
        /// dtor
        virtual ~VoronoiDiagram();

        int insert_point_site(const Point& p, int step=0);

        bool insert_line_site(int idx1, int idx2, int step=13);
        /// return the far radius
        double get_far_radius() const {return far_radius;}
        /// return number of point sites in diagram
        int num_point_sites() const {return num_psites-3;} // the three initial vertices don't count
        /// return number of line-segments sites in diagram
        int num_line_sites() const {return num_lsites;}
        /// return number of voronoi-vertices
        int num_vertices() const { return g.num_vertices()-num_point_sites(); }
        int num_faces() const { return g.num_faces(); }
        int num_split_vertices();
        /// string repr
        std::string print() const;
        static void reset_vertex_count() { VoronoiVertex::reset_count(); }
        void debug_on() {debug=true;}
        bool check();
        HEGraph& get_graph_reference() {return g;}
        void filter_reset() {g.filter_reset();}
    protected:
        /// initialize the diagram with three generators
        void initialize();
        HEVertex   find_seed_vertex(HEFace f, Site* site);
        EdgeVector find_in_out_edges(); 
        EdgeData   find_edge_data(HEFace f, VertexVector startverts, std::pair<HEVertex,HEVertex> segment);
        EdgeVector find_split_edges(HEFace f, Point pt1, Point pt2);
        bool find_split_vertex(HEFace f, HEVertex& v);
        HEFace find_pointsite_face(HEEdge start_edge);
        std::pair<HEVertex,HEVertex> find_endpoints(int idx1, int idx2);
        
        void augment_vertex_set( Site* site);        
        bool predicate_c4(HEVertex v);
        bool predicate_c5(HEVertex v);
        void mark_adjacent_faces(HEVertex v, Site* site);
        void mark_adjacent_faces_p( HEVertex v, Site* site);
        void mark_vertex(HEVertex& v,  Site* site); 
        void   add_vertices( Site* site );
        HEFace add_face(Site* site);
        
        void   add_edges(HEFace new_f1, HEFace f);        
        void   add_edges(HEFace new_f1, HEFace f, HEFace new_f2, std::pair<HEVertex,HEVertex> seg);
        
        void   add_edge(EdgeData ed, HEFace new1, HEFace new2=0);
        void   add_separator(HEFace f, HEFace nf, boost::tuple<HEEdge, HEVertex, HEEdge,bool> target, HEVertex endp, Site* s1, Site* s2);
        void   add_split_vertex(HEFace f, Site* s);
        std::pair<HEFace,HEFace> add_linesite_edges(HEVertex seg_start, HEVertex seg_end, bool linesite_k_sign);
        
        boost::tuple<HEVertex,HEFace,HEVertex,HEVertex,HEFace>
            find_null_face(HEVertex start, HEVertex other, Point l);
        boost::tuple<HEEdge,HEVertex,HEEdge,bool> find_separator_target(HEFace f, HEVertex endp);

        std::pair<HEVertex,HEFace> process_null_edge(Point dir, HEEdge next_edge , bool k3, bool next_prev);
        HEVertex add_separator_point(HEVertex endp, HEEdge edge, Point sep_dir);
        
        void repair_face( HEFace f );
        void repair_face( HEFace f , std::pair<HEVertex,HEVertex> segs,
                                     std::pair<HEFace,HEFace> nulled_faces,
                                     std::pair<HEFace,HEFace> null_faces );
        void remove_vertex_set();
        void remove_split_vertex(HEFace f);
        void reset_status();
        int num_new_vertices(HEFace f);
    // HELPER-CLASSES
        /// sanity-checks on the diagram are done by this helper class
        VoronoiDiagramChecker* vd_checker;
        /// a grid-search algorithm which allows fast nearest-neighbor search
        FaceGrid* fgrid;
        /// an algorithm for positioning vertices
        VertexPositioner* vpos;
    // DATA
        /// the half-edge diagram of the vd
        HEGraph g;
        /// the voronoi diagram is constructed for sites within a circle with radius far_radius
        double far_radius;
        /// the number of point sites
        int num_psites;
        /// the number of line-segment sites
        int num_lsites;
        /// temporary variable for incident faces, will be reset to NONINCIDENT after a site has been inserted
        FaceVector incident_faces;
        /// temporary variable for in-vertices, out-vertices that need to be reset
        /// after a site has been inserted
        std::set<HEVertex> modified_vertices;
        /// IN-vertices, i.e. to-be-deleted
        VertexVector v0;
        /// queue of vertices to be processed
        VertexQueue vertexQueue; 
        std::map<int,HEVertex> vertex_map;
        bool debug;
private:
        VoronoiDiagram(); // don't use.

};

// class for passing to numerical boost::toms748 root-finding algorithm
// to locate split-points
class SplitPointError {
public:
    SplitPointError(HEGraph& gi, HEEdge split_edge, Point pt1, Point pt2) :
    g(gi),  edge(split_edge), p1(pt1), p2(pt2)
    {}
    
    // find point on edge at t-value
    // compute a signed distance to the pt1-pt2 line
    double operator()(const double t) {
        Point p = g[edge].point(t);
        // line: pt1 + u*(pt2-pt1) = p
        //   (p-pt1) dot (pt2-pt1) = u* (pt2-pt1) dot (pt2-pt1)
        
        double u = (p-p1).dot(p2-p1) / ( (p2-p1).dot(p2-p1) );
        Point proj = p1 + u*(p2-p1);
        double dist = (proj-p).norm();
        double sign;
        if ( p.is_right(p1,p2) )
            sign = +1;
        else
            sign = -1;
            
        return sign*dist;
    }
private:
    HEGraph& g;
    HEEdge edge;
    Point p1;
    Point p2;
};

} // end namespace
#endif
// end voronoidiagram.h
