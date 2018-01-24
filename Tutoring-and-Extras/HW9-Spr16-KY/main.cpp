#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "mesh.h"

int main(int argc, char* argv[]) {

  // default values
  int rows = 10;
  int cols = 10;
  std::string image = "sunflowers.ppm";
  int target = 150;
  std::string which = "shortest";
  std::string method = "linear";
  bool preserve_area = false;
  bool debug = false;

  // process the command line arguments
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == std::string("-dimensions")) {
      assert (i+2 < argc);
      cols = atoi(argv[i+1]);
      rows = atoi(argv[i+2]);
      i+=2;
    } else if (std::string(argv[i]) == std::string("-image")) {
      assert (i+1 < argc);
      image = argv[i+1];
      i+=1;
    } else if (std::string(argv[i]) == std::string("-target")) {
      assert (i+1 < argc);
      target = atoi(argv[i+1]);
      i+=1;
    } else if (std::string(argv[i]) == std::string("-shortest")) {
      which = "shortest";
    } else if (std::string(argv[i]) == std::string("-random")) {
      which = "random";
    } else if (std::string(argv[i]) == std::string("-color")) {
      which = "color";
    } else if (std::string(argv[i]) == std::string("-priority_queue")) {
      method = "priority_queue";
    } else if (std::string(argv[i]) == std::string("-debug")) {
      debug = true;
    } else if (std::string(argv[i]) == std::string("-preserve_area")) {
      preserve_area = true;
    } else {
      std::cerr << "ERROR: unknown argument " << argv[i] << std::endl;
      exit(0);
    }
  }

  // create the mesh
  Mesh m(image, rows, cols, which, method, preserve_area, debug);
  m.CreateSVG("mesh_original.html");
  std::cout << "ORIGINAL:          " << m;
  
  if (debug) {
    // do a small number of collapses one at a time, to visualize the process
    for (int i = 1; i <= 10; i++) {
      m.Collapse();
      std::stringstream filename;
      filename << "mesh_collapse_" << std::setfill('0') << std::setw(2) << i << ".html";
      m.CreateSVG(filename.str());
      std::cout << "AFTER COLLAPSE " << std::setw(2)<< i << ": " << m;
      m.Check();
    }
  }
  
  // then finish the simplification
  m.Simplify(target);
  std::cout << "AFTER SIMPLIFY:    " <<  m;
  m.CreateSVG("mesh_final.html");
}
