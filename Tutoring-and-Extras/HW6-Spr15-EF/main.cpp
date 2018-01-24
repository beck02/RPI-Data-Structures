#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>
#include "MersenneTwister.h"
#include "tile.h"
#include "location.h"
#include "board.h"


// this global variable is set in main.cpp and is adjustable from the command line
// (you are not allowed to make your own global variables)
int GLOBAL_TILE_SIZE = 11;


// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char *argv[]) {
  std::cerr << "USAGE: " << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -all_solutions" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -all_solutions  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -tile_size <odd # >= 11>" << std::endl;
  exit(1);
}



// ==========================================================================
// To get you started, this function places tiles on the board and
// randomly and outputs the results (in all likelihood *not* a
// solution!) in the required format
void RandomlyPlaceTiles(Board &board, const std::vector<Tile*> &tiles, std::vector<Location> &locations) {

  // MersenneTwister is an excellent library for psuedo-random numbers!
  MTRand mtrand;

  for (int t = 0; t < (int)tiles.size(); t++) {
    // loop generates random locations until we (eventually) find one
    // that is not occupied
    int i,j;
    do {
      // generate a random coordinate within the range 0,0 -> rows-1,cols-1
      i = mtrand.randInt(board.numRows()-1);
      j = mtrand.randInt(board.numColumns()-1);
    } while (board.getTile(i,j) != NULL);

    // rotation is always 0 (for now)
    locations.push_back(Location(i,j,0));
    board.setTile(i,j,tiles[t]);
  }
}


// ==========================================================================
void HandleCommandLineArguments(int argc, char *argv[], std::string &filename, 
                                int &rows, int &columns, bool &all_solutions, bool &allow_rotations) {

  // must at least put the filename on the command line
  if (argc < 2) {
    usage(argc,argv);
  }
  filename = argv[1];

  // parse the optional arguments
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("-tile_size")) {
      i++;
      assert (i < argc);
      GLOBAL_TILE_SIZE = atoi(argv[i]);
      if (GLOBAL_TILE_SIZE < 11 || GLOBAL_TILE_SIZE % 2 == 0) {
        std::cerr << "ERROR: bad tile_size" << std::endl;
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-all_solutions")) {
      all_solutions = true;
    } else if (argv[i] == std::string("-board_dimensions")) {
      i++;
      assert (i < argc);
      rows = atoi(argv[i]);
      i++;
      assert (i < argc);
      columns = atoi(argv[i]);
      if (rows < 1 || columns < 1) {
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-allow_rotations")) {
      allow_rotations = true;
    } else {
      std::cerr << "ERROR: unknown argument '" << argv[i] << "'" << std::endl;
      usage(argc,argv);
    }
  }
}


// ==========================================================================
void ParseInputFile(int argc, char *argv[], const std::string &filename, std::vector<Tile*> &tiles) {

  // open the file
  std::ifstream istr(filename.c_str());
  if (!istr) {
    std::cerr << "ERROR: cannot open file '" << filename << "'" << std::endl;
    usage(argc,argv);
  }
  assert (istr);

  // read each line of the file
  std::string token, north, east, south, west;
  while (istr >> token >> north >> east >> south >> west) {
    assert (token == "tile");
    Tile *t = new Tile(north,east,south,west);
    tiles.push_back(t);
  }
}

//checks to see if a tile can be placed in a given spot
bool can_place(Board board, Tile* tile, int x, int y){
  if((x < 0 || y < 0 || x >= board.numColumns() || y >= board.numRows())){
    //Out of bounds
    return false;
  }
  //In bounds
  if(board.getTile(x,y) != NULL){
    //Already a tile there
    return false;
  } else{
    //Checking above
    if(x > 0 && board.getTile(x-1,y) != NULL){
      //There's a tile above
      if((*(board.getTile(x-1,y))).getSouth() != (*tile).getNorth()){
        //Edges don't match
        return false;
      }
    }else if (x == 0){
      if((*tile).getNorth() != "pasture"){
        //Cant be an edge
        return false;
      }
    }
    //Above OK, checking down
    if(x < board.numColumns()-1 && board.getTile(x+1,y) != NULL){
      //There's a tile below
      if((*(board.getTile(x+1,y))).getNorth() != (*tile).getSouth()){
        //Edges don't match
        return false;
      }
    }else if(x == board.numColumns()-1){
      if((*tile).getSouth() != "pasture"){
        //Cant be an edge
        return false;
      }
    }
    //Below OK, checking left
    if(y > 0 && board.getTile(x,y-1) != NULL){
      //There's a tile to the left
      if((*(board.getTile(x,y-1))).getEast() != (*tile).getWest()){
        //Edges don't match
        return false;
      }
    }else if(y == 0){
      if((*tile).getWest() != "pasture"){
        //Cant be an edge
        return false;
      }
    }
    //Left OK, checking right
    if(y < board.numRows()-1 && board.getTile(x,y+1) != NULL){
      //There's a tile to the right
      if((*(board.getTile(x,y+1))).getWest() != (*tile).getEast()){
        //Edges don't match
        return false;
      }
    }else if(y == board.numRows()-1){
      if((*tile).getEast() != "pasture"){
        //Cant be an edge
        return false;
      }
    }
  }
  //All OK
  return true;
}

bool isSolution(Board board, std::vector<Location> locations){
  //O(4n)
  //for loop w/ 4 if statements (directions)
  for(unsigned i = 0; i < locations.size(); i++){
    Location currLoc = locations[i];
    int currX = currLoc.row;
    int currY = currLoc.column;
    //north check
    if(currX <= 0 || board.getTile(currX-1,currY) == NULL){
      if((*board.getTile(currX,currY)).getNorth() != "pasture"){
        return false;
      }
    }
    //south check
    if(currX >= board.numRows()-1 || board.getTile(currX+1,currY) == NULL){
      if((*board.getTile(currX,currY)).getSouth() != "pasture"){
        return false;
      }
    }
    //east check
    if(currY >= board.numColumns()-1 || board.getTile(currX,currY+1) == NULL){
      if((*board.getTile(currX,currY)).getEast() != "pasture"){
        return false;
      }
    }
    //west check
    if(currY <= 0 || board.getTile(currX,currY-1) == NULL){
      if((*board.getTile(currX,currY)).getWest() != "pasture"){
        return false;
      }
    }
  }
  return true;
}


//PASS IN allow_rotations
void PlaceTiles(Board board, std::vector<Tile*>& tiles, std::vector<Location> locations, int tileNum, bool all_solutions, bool allow_rotations, std::vector<std::vector<Location> > &allLocs){
  tileNum+=1;
  if(tileNum < (int)tiles.size()){
    //board.Print();
    //std::cout << std::endl << std::endl << std::endl;
    //std::cout << "size: " << locations.size() << ", tileNum: " << tileNum << std::endl;
    for(int i = locations.size()-1; i >= 0; i--){
      Location currLoc = locations[i];
      int currX = currLoc.row;
      int currY = currLoc.column;
      int r = 1;
      /*
        if allow_rotations is true, we run through the contents
        of the for loop 3 more times, representing each
        90-degree rotation
      */
      if(allow_rotations) r = 4;
      for(int j = 0; j < r; j++){
        if(can_place(board, tiles[tileNum], currX, currY-1)){
          board.setTile(currX, currY-1, tiles[tileNum]);
          //std::cout << "Tile " << tileNum << " placed left of tile " << i << std::endl;
          locations.push_back(Location(currX, currY-1, 0));
          PlaceTiles(board, tiles, locations, tileNum, all_solutions, allow_rotations, allLocs);
          locations.pop_back();
          board.removeTile(currX, currY-1);
        }
        if(can_place(board, tiles[tileNum], currX, currY+1)){
          board.setTile(currX, currY+1, tiles[tileNum]);
          //std::cout << "Tile " << tileNum << " placed right of tile " << i << std::endl;
          locations.push_back(Location(currX, currY+1, 0));
          PlaceTiles(board, tiles, locations, tileNum, all_solutions, allow_rotations, allLocs);
          locations.pop_back();
          board.removeTile(currX, currY+1);
        }
        if(can_place(board, tiles[tileNum], currX-1, currY)){
          board.setTile(currX-1, currY, tiles[tileNum]);
          //std::cout << "Tile " << tileNum << " placed above tile " << i << std::endl;
          locations.push_back(Location(currX-1, currY, 0));
          PlaceTiles(board, tiles, locations, tileNum, all_solutions, allow_rotations, allLocs);
          locations.pop_back();
          board.removeTile(currX-1, currY);
        }
        if(can_place(board, tiles[tileNum], currX+1, currY)){
          board.setTile(currX+1, currY, tiles[tileNum]);
          //std::cout << "Tile " << tileNum << " placed below tile " << i << std::endl;
          locations.push_back(Location(currX+1, currY, 0));
          PlaceTiles(board, tiles, locations, tileNum, all_solutions, allow_rotations, allLocs);
          locations.pop_back();
          board.removeTile(currX+1, currY);
        }
        if(allow_rotations) (*tiles[tileNum]).rotate();
      }
    }
  }else{
    if(isSolution(board, locations)){
      if(!all_solutions){
        if(allLocs.size() < 1) allLocs.push_back(locations);
      }else{
        allLocs.push_back(locations);
      }
      //NEED THIS ANYMORE?
      /*std::cout << "Solution: ";
      for (int i = 0; i < (int)locations.size(); i++) {
        std::cout << locations[i];
      }
      std::cout << std::endl;

      // print the ASCII art board representation
      board.Print();
      std::cout << std::endl;*/
    }
  }
}

void PlaceFirst(Board board, std::vector<Tile*> tiles, std::vector<Location> locations, bool all_solutions, bool allow_rotations, std::vector<std::vector<Location> > &allLocs){
  if(tiles.size() != 0){
    int midX = (board.numRows()/2)-1;
    int midY = (board.numColumns()/2)-1;
    /*
      if allow_rotations is true, we run through the contents
      of the for loop 3 more times, representing each
      90-degree rotation
    */
    int r = 1;
    if(allow_rotations) r = 4;
    for(int j = 0; j < r; j++){
      //place in middle.  If it cant place it for whatever reason, it tries four positions around it
      if(can_place(board, tiles[0], midX, midY )){
        board.setTile(midX, midY, tiles[0]);
        locations.push_back(Location(midX, midY, 0));
        PlaceTiles(board, tiles, locations, 0, all_solutions, allow_rotations, allLocs);
        locations.pop_back();
        board.removeTile(midX, midY);
      }
      if(can_place(board, tiles[0], (midX)-1, midY )){
        board.setTile(midX-1, midY, tiles[0]);
        locations.push_back(Location(midX-1, midY, 0));
        PlaceTiles(board, tiles, locations, 0, all_solutions, allow_rotations, allLocs);
        locations.pop_back();
        board.removeTile(midX-1, midY);
      }
      if(can_place(board, tiles[0], midX+1, midY )){
        board.setTile(midX+1, midY, tiles[0]);
        locations.push_back(Location(midX+1, midY, 0));
        PlaceTiles(board, tiles, locations, 0, all_solutions, allow_rotations, allLocs);
        locations.pop_back();
        board.removeTile(midX+1, midY);
      }
      if(can_place(board, tiles[0], midX, midY-1 )){
        board.setTile(midX, midY-1, tiles[0]);
        locations.push_back(Location(midX, midY-1, 0));
        PlaceTiles(board, tiles, locations, 0, all_solutions, allow_rotations, allLocs);
        locations.pop_back();
        board.removeTile(midX, midY-1);
      }
      if(can_place(board, tiles[0], midX, midY+1 )){
        board.setTile(midX, midY+1, tiles[0]);
        locations.push_back(Location(midX, midY+1, 0));
        PlaceTiles(board, tiles, locations, 0, all_solutions, allow_rotations, allLocs);
        locations.pop_back();
        board.removeTile(midX, midY+1);
      }
      if(allow_rotations) (*tiles[0]).rotate();
    }
  }
}

// ==========================================================================
int main(int argc, char *argv[]) {

  std::string filename;
  int rows = -1;
  int columns = -1;
  bool all_solutions = false;
  bool allow_rotations = false;
  HandleCommandLineArguments(argc, argv, filename, rows, columns, all_solutions, allow_rotations);

  // load in the tiles
  std::vector<Tile*> tiles;
  ParseInputFile(argc,argv,filename,tiles);


  // confirm the specified board is large enough
  if (rows < 1  ||  columns < 1  ||  rows * columns < (int)tiles.size()) {
    std::cerr << "ERROR: specified board is not large enough" << rows << "X" << columns << "=" << rows*columns << " " << tiles.size() << std::endl;
    usage(argc,argv);
  }
  Board board(rows,columns);
  std::vector<Location> locations;
  std::vector<std::vector<Location> > allLocs;
  std::cout << "locations.size() before: " << locations.size() << std::endl;
  std::cout << "allLocs.size() before: " << allLocs.size() << std::endl;
  PlaceFirst(board, tiles, locations, all_solutions, allow_rotations, allLocs);
  std::cout << "locations.size() after: " << locations.size() << std::endl;
  std::cout << "allLocs.size() after: " << allLocs.size() << std::endl;



  // print the solution

  for (int i = 0; i < (int)allLocs.size(); i++) {
    for (int j = 0; j < (int)allLocs[i].size(); j++){
      std::cout << "Solution: " << allLocs[i][j];
    }
    std::endl;
  }
  std::cout << std::endl;

  
  // delete the tiles
  for (int t = 0; t < (int)tiles.size(); t++) {
    delete tiles[t];
  }
}
// ==========================================================================
