#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "mesh.h"


// a helper function to output a color for use in html
std::string OutputColor(Vertex pt) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::hex << (int)floor(pt.r())
     << std::setfill('0') << std::setw(2) << std::hex << (int)floor(pt.g())
     << std::setfill('0') << std::setw(2) << std::hex << (int)floor(pt.b());
  return ss.str();
}


void Mesh::CreateSVG(const std::string& filename) const {
  std::ofstream ostr(filename.c_str());
  ostr << "<body bgcolor=dddddd  onLoad=\"render()\" >\n";

  // a few checkboxes at the top of the page to control the visualization
  ostr << "<form name=\"orderForm\">\n";
  ostr << "   <input type=\"checkbox\" name=\"illegal\"   checked  "
       << " onClick=\"render()\">draw illegal (red) & next legal collapse (blue) edges<br>\n";
  ostr << "   <input type=\"checkbox\" name=\"wireframe\"          "
       << " onClick=\"render()\">draw all edges<br>\n";
  ostr << "   <input type=\"checkbox\" name=\"black\"              "
       << " onClick=\"render()\">toggle white vs. black<br>\n";
  ostr << "</form>\n";

  // javascript to actually change the visualization based on the checkboxes
  ostr << "<script language=\"JavaScript\">\n";
  ostr << "  function render() {\n";
  ostr << "    var mysvg = document.getElementById(\"mesh\");\n";
  ostr << "    if (document.orderForm.wireframe.checked == false) {\n";
  ostr << "      var polys = mysvg.children;\n";
  ostr << "      for (var i = 0; i < polys.length; i++) {\n";
  ostr << "        if (polys[i].tagName.toUpperCase() == \"POLYGON\") {\n";
  ostr << "          polys[i].style[\"strokeWidth\"] = \"0\"\n";
  ostr << "        }\n";
  ostr << "      }\n";
  ostr << "    } else if (document.orderForm.black.checked == false) {\n";
  ostr << "      var polys = mysvg.children;\n";
  ostr << "      for (var i = 0; i < polys.length; i++) {\n";
  ostr << "        if (polys[i].tagName.toUpperCase() == \"POLYGON\") {\n";
  ostr << "          polys[i].style.stroke = \"#FFFFFF\"\n";
  ostr << "          polys[i].style[\"strokeWidth\"] = \"1\"\n";
  ostr << "        }\n";
  ostr << "      }\n";
  ostr << "      mysvg.style.background = \"white\"\n";
  ostr << "    } else  {\n";
  ostr << "      var polys = mysvg.children;\n";
  ostr << "      for (var i = 0; i < polys.length; i++) {\n";
  ostr << "        if (polys[i].tagName.toUpperCase() == \"POLYGON\") {\n";
  ostr << "          polys[i].style.stroke = \"#000000\"\n";
  ostr << "          polys[i].style[\"strokeWidth\"] = \"2\"\n";
  ostr << "        }\n";
  ostr << "      }\n";
  ostr << "      mysvg.style.background = \"white\"\n";
  ostr << "    }\n";
  ostr << "    if (document.orderForm.illegal.checked == false) {\n";
  ostr << "      var polys = mysvg.children;\n";
  ostr << "      for (var i = 0; i < polys.length; i++) {\n";
  ostr << "        if (polys[i].tagName.toUpperCase() == \"LINE\") {\n";
  ostr << "          polys[i].style[\"strokeWidth\"] = \"0\"\n";
  ostr << "        }\n";
  ostr << "      }\n";
  ostr << "    } else  {\n";
  ostr << "      var polys = mysvg.children;\n";
  ostr << "      for (var i = 0; i < polys.length; i++) {\n";
  ostr << "        if (polys[i].tagName.toUpperCase() == \"LINE\") {\n";
  ostr << "          polys[i].style[\"strokeWidth\"] = \"5\"\n";
  ostr << "        }\n";
  ostr << "      }\n";
  ostr << "      mysvg.style.background = \"white\"\n";
  ostr << "    }\n";
  ostr << "  }\n";
  ostr << "</script>\n";

  ostr << "<svg  id=\"mesh\" height=\"" << height + 2*BORDER
       << "\" width=\""    << width + 2*BORDER
       << "\" style=\"background:white\" shape-rendering=\"crispEdges\">\n";

  // draw the triangles with the average color of the vertices
  for (triangles_set::const_iterator itr = triangles.begin(); itr != triangles.end(); itr++) {
    Triangle *t = *itr;
    float r = (t->getVertex(0)->r() +t->getVertex(1)->r() +t->getVertex(2)->r()) / 3.0;
    float g = (t->getVertex(0)->g() +t->getVertex(1)->g() +t->getVertex(2)->g()) / 3.0;
    float b = (t->getVertex(0)->b() +t->getVertex(1)->b() +t->getVertex(2)->b()) / 3.0;
    ostr << "<polygon points=\""
         << std::setw(8) << t->getVertex(0)->x() << "," << std::setw(8) << t->getVertex(0)->y() << "  "
         << std::setw(8) << t->getVertex(1)->x() << "," << std::setw(8) << t->getVertex(1)->y() << "  "
         << std::setw(8) << t->getVertex(2)->x() << "," << std::setw(8) << t->getVertex(2)->y()
         << "\" style=\"fill:#" << OutputColor(Vertex(0,0,r,g,b)) 
         << ";stroke:#" << OutputColor(Vertex(0,0,r,g,b)) 
         << ";stroke-width:1"
         << ";stroke-linecap:round\" "
         << ";stroke-linejoin:round\" "
         << "/>" << std::endl;
  }

  // draw the illegal edges in red
  for (edges_map::const_iterator itr = edges.begin(); itr != edges.end(); itr++) {
    const Edge *e = itr->second;
    assert (itr->first.first->getID() < itr->first.second->getID());
    if (!e->isLegal()) {
      ostr << "<line "
           << "x1=\"" << std::setw(8) << e->getV1()->x() << "\" "
           << "y1=\"" << std::setw(8) << e->getV1()->y() << "\" "
           << "x2=\"" << std::setw(8) << e->getV2()->x() << "\" " 
           << "y2=\"" << std::setw(8) << e->getV2()->y() << "\" "
           << " stroke=\"red\" "
           << " stroke-width=\"0\" "
           << " stroke-linecap=\"round\" "
           << "/>" << std::endl;
    }
  }

  // draw the next (legal) edge to collapse in blue
  Edge *e = FindEdge();
  if (e != NULL) {
    ostr << "<line "
         << "x1=\"" << std::setw(8) << e->getV1()->x() << "\" " 
         << "y1=\"" << std::setw(8) << e->getV1()->y() << "\" "
         << "x2=\"" << std::setw(8) << e->getV2()->x() << "\" " 
         << "y2=\"" << std::setw(8) << e->getV2()->y() << "\" "
         << " stroke=\"blue\" "
         << " stroke-width=\"0\" "
         << " stroke-linecap=\"round\" "
         << "/>" << std::endl;
  }
  
  ostr << "</svg>\n";

  // print some simple stats at the bottom of the page
  ostr << "<p>" << *this << "</p>" << std::endl;
  ostr << "</body>\n";
}
