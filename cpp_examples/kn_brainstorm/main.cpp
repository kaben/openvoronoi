#include <string>
#include <iostream>

//#include <openvoronoi/graph.hpp>
#include <openvoronoi/voronoidiagram.hpp>
#include <openvoronoi/polygon_interior.hpp>
#include <openvoronoi/medial_axis.hpp>
#include <openvoronoi/offset.hpp>
//#include <openvoronoi/common/halfedgediagram.hpp>

typedef std::pair<ovd::Point,ovd::Point> segment;

// very simple OpenVoronoi example program
int main() {
    ovd::VoronoiDiagram* vd = new ovd::VoronoiDiagram(1,100);
    std::cout << vd->version() << "\n";

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

    std::cout << "id0:" << id0 << std::endl;
    std::cout << "id1:" << id1 << std::endl;
    std::cout << "id2:" << id2 << std::endl;
    std::cout << "id3:" << id3 << std::endl;
    std::cout << "id4:" << id4 << std::endl;

    vd->insert_line_site(id0, id1);
    vd->insert_line_site(id1, id2);
    vd->insert_line_site(id2, id3);
    vd->insert_line_site(id3, id4);
    vd->insert_line_site(id4, id0);

    std::cout << std::endl;
    std::cout << "walk:" << std::endl;
    ovd::HEGraph& g = vd->get_graph_reference();
    ovd::PolygonInterior pi(g);
    ovd::Offset offset(g);
    for (int i=1; i<5; i++) {
      ovd::OffsetList offset_list = offset.offset(i*0.008);
      BOOST_FOREACH( ovd::Loop loop, offset_list ) { // loop through each loop
        bool first = true;
        BOOST_FOREACH( ovd::Lpt lpt, loop ) { // loop through each line/arc
          if (first) {
            first = false;
            std::cout << "first offset:p:" << lpt.p << std::endl;
          } else {
            std::cout << "offset:p:" << lpt.p << ",r:" << lpt.r << ",c:" << lpt.c << ",cw:" << lpt.cw << std::endl;
          }
        }
      }
    }
    ovd::MedialAxis ma(g);
    ovd::MedialAxisWalk maw(g);
    ovd::ChainList toolpath = maw.walk();
    BOOST_FOREACH( ovd::Chain chain, toolpath ) { // loop through each chain
      BOOST_FOREACH( ovd::MedialPointList pt_dist_list, chain ) { // loop through each point-list
        BOOST_FOREACH( ovd::MedialPoint pt_dist, pt_dist_list ) { // loop through each Point/distance
          std::cout << "position:" << pt_dist.p << "; clearance_radius:" << pt_dist.clearance_radius << std::endl;;
        }
      }
    }
    

    std::cout << std::endl;
    std::cout << "vertices:" << std::endl;
    int outer_vertex_ct = 0;
    int normal_vertex_ct = 0;
    int pointsite_vertex_ct = 0;
    int endpoint_vertex_ct = 0;
    int seppoint_vertex_ct = 0;
    int apex_vertex_ct = 0;
    int split_vertex_ct = 0;
    int unknown_vertex_ct = 0;
    BOOST_FOREACH(ovd::HEVertex vertex, g.vertices()) { // loop through each vertex
      switch(g[vertex].type){
      case ovd::OUTER: std::cout << "outer vertex:" << std::endl; outer_vertex_ct++; break;
      case ovd::NORMAL: std::cout << "normal vertex:" << std::endl; normal_vertex_ct++; break;
      case ovd::POINTSITE: std::cout << "pointsite vertex:" << std::endl; pointsite_vertex_ct++; break;
      case ovd::ENDPOINT: std::cout << "endpoint vertex:" << std::endl; endpoint_vertex_ct++; break;
      case ovd::SEPPOINT: std::cout << "seppoint vertex:" << std::endl; seppoint_vertex_ct++; break;
      case ovd::APEX: std::cout << "apex vertex:" << std::endl; apex_vertex_ct++; break;
      case ovd::SPLIT: std::cout << "split vertex:" << std::endl; split_vertex_ct++; break;
      default: std::cout << "unknown vertex:" << std::endl; unknown_vertex_ct++; break;
      }
      std::cout << "  vertex index:" << g[vertex].index << std::endl;
      std::cout << "  vertex position:" << g[vertex].position << std::endl;
      std::cout << "  vertex face:" << g[vertex].face << std::endl;
      std::cout << "  adjacent vertices:";
      BOOST_FOREACH(ovd::HEVertex v, g.adjacent_vertices(vertex)) {
        std::cout << g[v].index << g[v].position << "[";
        switch(g[v].type){
        case ovd::OUTER: std::cout << "outer"; break;
        case ovd::NORMAL: std::cout << "normal"; break;
        case ovd::POINTSITE: std::cout << "pointsite"; break;
        case ovd::ENDPOINT: std::cout << "endpoint"; break;
        case ovd::SEPPOINT: std::cout << "seppoint"; break;
        case ovd::APEX: std::cout << "apex"; break;
        case ovd::SPLIT: std::cout << "split"; break;
        default: std::cout << "unknown"; break;
        }
        std::cout << "]" << " ";
      }
      std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "vertex_ct: " << g.vertices().size() << std::endl;
    std::cout << std::endl;
    std::cout << "outer_vertex_ct: " << outer_vertex_ct << std::endl;
    std::cout << "normal_vertex_ct: " << normal_vertex_ct << std::endl;
    std::cout << "pointsite_vertex_ct: " << pointsite_vertex_ct << std::endl;
    std::cout << "endpoint_vertex_ct: " << endpoint_vertex_ct << std::endl;
    std::cout << "seppoint_vertex_ct: " << seppoint_vertex_ct << std::endl;
    std::cout << "apex_vertex_ct: " << apex_vertex_ct << std::endl;
    std::cout << "split_vertex_ct: " << split_vertex_ct << std::endl;
    std::cout << "unknown_vertex_ct: " << unknown_vertex_ct << std::endl;

    std::cout << std::endl;
    std::cout << "edges:" << std::endl;
    int line_edge_ct = 0;
    int lineline_edge_ct = 0;
    int para_lineline_edge_ct = 0;
    int outedge_edge_ct = 0;
    int parabola_edge_ct = 0;
    int ellipse_edge_ct = 0;
    int hyperbola_edge_ct = 0;
    int separator_edge_ct = 0;
    int nulledge_edge_ct = 0;
    int linesite_edge_ct = 0;
    int unknown_edge_ct = 0;
    BOOST_FOREACH(ovd::HEEdge edge, g.edges()) { // loop through each edge
      ovd::HEVertex source_vertex = g.source(edge);
      ovd::HEVertex target_vertex = g.target(edge);
      (void) source_vertex;
      (void) target_vertex;

      /* Count of types encountered. */
      switch(g[edge].type){
      case ovd::LINE: std::cout << "line edge:" << std::endl; line_edge_ct++; break;
      case ovd::LINELINE: std::cout << "lineline edge:" << std::endl; lineline_edge_ct++; break;
      case ovd::PARA_LINELINE: std::cout << "para_lineline edge:" << std::endl; para_lineline_edge_ct++; break;
      case ovd::OUTEDGE: std::cout << "outedge edge:" << std::endl; outedge_edge_ct++; break;
      case ovd::PARABOLA: std::cout << "parabola edge:" << std::endl; parabola_edge_ct++; break;
      case ovd::ELLIPSE: std::cout << "ellipse edge:" << std::endl; ellipse_edge_ct++; break;
      case ovd::HYPERBOLA: std::cout << "hyperbola edge:" << std::endl; hyperbola_edge_ct++; break;
      case ovd::SEPARATOR: std::cout << "separator edge:" << std::endl; separator_edge_ct++; break;
      case ovd::NULLEDGE: std::cout << "nulledge edge:" << std::endl; nulledge_edge_ct++; break;
      case ovd::LINESITE: std::cout << "linesite edge:" << std::endl; linesite_edge_ct++; break;
      default: std::cout << "unknown:" << std::endl; unknown_edge_ct++; break;
      }
      std::cout << "  edge valid:" << g[edge].valid << std::endl;
      std::cout << "  source_vertex index:" << g[source_vertex].index << std::endl;
      std::cout << "  source_vertex position:" << g[source_vertex].position << std::endl;
      std::cout << "  target_vertex index:" << g[target_vertex].index << std::endl;
      std::cout << "  target_vertex position:" << g[target_vertex].position << std::endl;

      /* These edge types are drawn as a single line from source to target. */
      //switch(g[edge].type){
      //case ovd::LINE: 
      //case ovd::LINELINE:
      //case ovd::PARA_LINELINE:
      //case ovd::OUTEDGE:
      //case ovd::SEPARATOR:
      //case ovd::LINESITE: {
      //    std::cout << "  source_vertex position:" << g[source_vertex].position << std::endl;
      //    std::cout << "  target_vertex position:" << g[target_vertex].position << std::endl;
      //  }
      //  break;
      //case ovd::PARABOLA:
      //case ovd::ELLIPSE:
      //case ovd::HYPERBOLA:
      //case ovd::NULLEDGE:
      //default:
      //  break;
      //}

    }

    std::cout << std::endl;
    std::cout << "edge_ct: " << g.edges().size() << std::endl;
    std::cout << std::endl;
    std::cout << "line_edge_ct: " << line_edge_ct << std::endl;
    std::cout << "lineline_edge_ct: " << lineline_edge_ct << std::endl;
    std::cout << "para_lineline_edge_ct: " << para_lineline_edge_ct << std::endl;
    std::cout << "outedge_edge_ct: " << outedge_edge_ct << std::endl;
    std::cout << "parabola_edge_ct: " << parabola_edge_ct << std::endl;
    std::cout << "ellipse_edge_ct: " << ellipse_edge_ct << std::endl;
    std::cout << "hyperbola_edge_ct: " << hyperbola_edge_ct << std::endl;
    std::cout << "separator_edge_ct: " << separator_edge_ct << std::endl;
    std::cout << "nulledge_edge_ct: " << nulledge_edge_ct << std::endl;
    std::cout << "linesite_edge_ct: " << linesite_edge_ct << std::endl;
    std::cout << "unknown_edge_ct: " << unknown_edge_ct << std::endl;

    std::cout << std::endl;
    std::cout << "faces:" << std::endl;
    g.print_faces();
    //for(ovd::HEGraph::Face f=0; f<g.num_faces(); f++){
    //  g.print_face(f);
    //}

    std::cout << std::endl;
    std::cout << vd->print();

    return 0;
}

