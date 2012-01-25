
#include <string>
#include <iostream>

#include <openvoronoi/offset2.hpp>
#include <openvoronoi/voronoidiagram.hpp>
#include <openvoronoi/utility/vd2svg.hpp>
#include <openvoronoi/version.hpp>

// Very simple OpenVoronoi example program demonstrating offsets.
int main() {
    ovd::VoronoiDiagram* vd = new ovd::VoronoiDiagram(1,100); // (r, bins)
    // double r: radius of circle within which all input geometry must fall. use 1 (unit-circle). Scale geometry if necessary.
    // int bins:  bins for face-grid search. roughly sqrt(n), where n is the number of sites is good according to Held.
     
    std::cout << ovd::version() << "\n"; // the git revision-string
    ovd::Point p0(-0.1,-0.2);
    ovd::Point p1(0.2,0.1);
    ovd::Point p2(0.4,0.2);
    ovd::Point p3(0.6,0.6);
    ovd::Point p4(-0.6,0.3);

    int id0 = vd->insert_point_site(p0);
    int id1 = vd->insert_point_site(p1);
    int id2 = vd->insert_point_site(p2);
    int id3 = vd->insert_point_site(p3);
    int id4 = vd->insert_point_site(p4);

    vd->insert_line_site(id0, id1);
    vd->insert_line_site(id1, id2);
    vd->insert_line_site(id2, id3);
    vd->insert_line_site(id3, id4);
    vd->insert_line_site(id4, id0);
    vd->check();

    ovd::HEGraph& g = vd->get_graph_reference();

    // save drawing to svg file.
    svg::Dimensions dimensions(1024, 1024);
    svg::Document doc("basic_offset.svg", svg::Layout(dimensions, svg::Layout::BottomLeft));
    BOOST_FOREACH( ovd::HEEdge e, g.edges() ) {
        write_edge_to_svd(g,doc,e);
    }
    
    // draw four offsets.
    svg::Color line_color( svg::Color::Lime );
    svg::Color arc_color( svg::Color::Green );
    ovd::FaceOffset face_offset(g);
    for (int i=1; i<5; i++) {
        face_offset.offset(i*0.008);
        ovd::FaceOffsetLoops offset_list = face_offset.get_offset_list();

        BOOST_FOREACH( ovd::FaceOffsetLoop loop, offset_list ) { // loop through each loop
            ovd::HEEdge current_edge = loop.start_edge;
            ovd::Point prev_pt( g[current_edge].point( loop.t ) );
            std::cout << "first offset:p:" << prev_pt << std::endl;

            BOOST_FOREACH( ovd::FaceEdge face_edge, loop.face_edges ) { //loop through each face
                ovd::Site* s = g[face_edge.f].site;
                ovd::Point next_pt( g[face_edge.next_edge].point(loop.t) );
                ovd::Ofs* o = s->offset( prev_pt, next_pt ); // ask Site for offset-geometry here.

                if (!s->isLine() ) {
                    // point and arc-sites produce arc-offsets, for which cw must be set.
                    bool cw = face_offset.find_cw( o->start(), o->center(), o->end() ); // figure out cw or ccw arcs?
                    write_arc_to_svd( g, doc, prev_pt, next_pt, o->radius(), o->center(), cw, arc_color );
                    std::cout << "offset:p:" << prev_pt << ",r:" << o->radius() << ",c:" << o->center() << ",cw:" << cw << std::endl;
                } else {
                    write_line_to_svd( g, doc, prev_pt, next_pt, line_color );
                    std::cout << "offset:p:" << prev_pt << std::endl;
                }
                prev_pt = next_pt;
            }
        }
    }

    doc.save();
    std::cout << vd->print();
    delete vd;

    return 0;
}
