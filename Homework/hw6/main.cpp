#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>
#include <utility>

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

  for (int t = 0; t < tiles.size(); t++) {
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

void PrintRandom(int times, int rows, int columns, const std::vector<Tile*> &tiles){
  for (int i = 0; i < times; i++) {
    // generate a random tile layouts
    Board board(rows,columns);
    std::vector<Location> locations;
    RandomlyPlaceTiles(board, tiles, locations);
    
    // print the solution to cout
    std::cout << "probably-not-a-Solution: ";
    for (int j = 0; j < locations.size(); j++) {
      std::cout << j+1 << ":" << locations[j] << ", ";
    }
    std::cout << std::endl;

    // print the ASCII art board representation
    board.Print();
    std::cout << std::endl;
  }
  // delete the tiles
  for (int t = 0; t < tiles.size(); t++) {
    delete tiles[t];
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

  if(rows < 1 || columns < 1){//sets default board dimensions if not specified
    rows = 3; columns = 3;
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

// ==========================================================================
void CheckEdges(int &r, int &c, const Tile* currTile, const int rows, const int cols){
  bool moved = false;
  if( (c == 0) && (currTile->getWest() != "pasture") ){
    ++c;
    moved = true;
  } else if( (c == cols-1) && (currTile->getEast() != "pasture") ){
    --c;
    moved = true;
  } else if( (r == 0) && (currTile->getNorth() != "pasture") ){
    ++r;
    moved = true;
  } else if( (r == rows-1) && (currTile->getSouth() != "pasture") ){
    --r;
    moved = true;
  }
  if(moved){
    CheckEdges(r, c, currTile, rows, cols);
  }
}

// ==========================================================================
bool legalPlacement(const int r, const int c, const Board &board, const Tile* currTile, bool final){
  bool legal = false;
  Location loc(r, c, 0);
  // std::cout << "Checking legality of location: " << loc << std::endl;
  if( board.inBounds(loc) ){
    if( (board.getTile(loc) == NULL) || final ){

      Location above(r-1, c, 0);
      Location below(r+1, c, 0);
      Location left(r, c-1, 0);
      Location right(r, c+1, 0);
      bool okabove = false;
      bool okbelow = false;
      bool okleft = false;
      bool okright = false;

      //check space above to see if it's in grid bounds
      if( board.inBounds(above) ){
        //if it is in bounds, check to see if there's a tile there
        if(board.getTile(above) != NULL){
          //if there is a tile there, check to see if its relevent edge matches the current tile
          if(board.getTile(above)->getSouth() == currTile->getNorth()){
            okabove = true;
          }
        }
        //if there's no tile there, check if the board is done/tiles are all placed
        else{
          if(final){//if they are all placed, we can't have a hanging road or city
            if( currTile->getNorth() == "pasture" ){
              okabove = true;
            }
          }
          //if there are still tiles to be placed, then we're fine for now even if the edge has a hanging road/city
          else{
            okabove = true;
          }
        }
      }
      //if the space above isn't in grid bounds, check the current tile's relevent edge
      else{
        //if the top edge is pasture, then we're good on top
        if(currTile->getNorth() == "pasture"){
          okabove = true;
        }
      }
      // std::cout << "okabove: " << okabove << std::endl;

      //check space below to see if it's in grid bounds
      if( board.inBounds(below) ){
        //if it is in bounds, check to see if there's a tile there
        if(board.getTile(below) != NULL){
          //if there is a tile there, check to see if its relevent edge matches the current tile
          if(board.getTile(below)->getNorth() == currTile->getSouth()){
            okbelow = true;
          }
        }
        //if there's no tile there, check if the board is done/tiles are all placed
        else{
          if(final){//if they are all placed, we can't have a hanging road or city
            if( currTile->getSouth() == "pasture" ){
              okbelow = true;
            }
          }
          //if there are still tiles to be placed, then we're fine for now even if the edge has a hanging road/city
          else{
            okbelow = true;
          }
        }
      }
      //if the space below isn't in grid bounds, check the current tile's relevent edge
      else{
        //if the bottom edge is pasture, then we're good on bottom
        if(currTile->getSouth() == "pasture"){
          okbelow = true;
        }
      }
      // std::cout << "okbelow: " << okbelow << std::endl;

      //check space left to see if it's in grid bounds
      if( board.inBounds(left) ){
        //if it is in bounds, check to see if there's a tile there
        if(board.getTile(left) != NULL){
          //if there is a tile there, check to see if its relevent edge matches the current tile
          if(board.getTile(left)->getEast() == currTile->getWest()){
            okleft = true;
          }
        }
        //if there's no tile there, check if the board is done/tiles are all placed
        else{
          if(final){//if they are all placed, we can't have a hanging road or city
            if( currTile->getWest() == "pasture" ){
              okleft = true;
            }
          }
          //if there are still tiles to be placed, then we're fine for now even if the edge has a hanging road/city
          else{
            okleft = true;
          }
        }
      }
      //if the space left isn't in grid bounds, check the current tile's relevent edge
      else{
        //if the left edge is pasture, then we're good on left
        if(currTile->getWest() == "pasture"){
          okleft = true;
        }
      }
      // std::cout << "okleft: " << okleft << std::endl;

      //check space right to see if it's in grid bounds
      if( board.inBounds(right) ){
        //if it is in bounds, check to see if there's a tile there
        if(board.getTile(right) != NULL){
          //if there is a tile there, check to see if its relevent edge matches the current tile
          if(board.getTile(right)->getWest() == currTile->getEast()){
            okright = true;
          }
        }
        //if there's no tile there, check if the board is done/tiles are all placed
        else{
          if(final){//if they are all placed, we can't have a hanging road or city
            if( currTile->getEast() == "pasture" ){
              okright = true;
            }
          }
          //if there are still tiles to be placed, then we're fine for now even if the edge has a hanging road/city
          else{
            okright = true;
          }
        }
      }
      //if the space right isn't in grid bounds, check the current tile's relevent edge
      else{
        //if the right edge is pasture, then we're good on right
        if(currTile->getEast() == "pasture"){
          okright = true;
        }
      }
      // std::cout << "okright: " << okright << std::endl;

      legal = ( okabove && okbelow && okleft && okright );
      // std::cout << "legal: " << legal << std::endl;
      
    }//end if empty or final
    else{
      // std::cout << "location not empty (or checking solution)" << std::endl;
    }
  } //end if loc in bounds
  else{
    // std::cout << "location not in bounds" << std::endl;
  }
  return legal;
}

// ==========================================================================
bool validSolution(const Board &board, const std::vector<Tile*> &tiles, const std::vector<Location> &locations){
  if( tiles.size() != locations.size() ){
    // std::cout << "Not all tiles placed." << std::endl;
    return false;
  }
  for(int i = 0; i < locations.size(); ++i){
    Tile* currTile = board.getTile( locations[i] );
    int r = locations[i].row;
    int c = locations[i].column;
    if( !legalPlacement(r, c, board, currTile, true) ){
      return false;
    }
  }
  return true;
}

// ==========================================================================
bool unique(const Board &board, const std::vector<Location> &locations, const std::vector<std::pair<Board, std::vector<Location> > > &solutions){
  if( solutions.size() == 0 ){ return true; }
  Location newStartLoc = locations[0];
  //check each of the previously found solutions
  for(int i = 0; i < solutions.size(); ++i){
    Location oldStartLoc = solutions[i].second[0];
    // std::cout << "Comparing tiles against previous solution #" << i+1 << "--------------------------------------------" << std::endl;
    //check each tile location in the solution
    for (int j = 0; j < solutions[i].second.size(); ++j){
      Location oldLoc = solutions[i].second[j];
      std::pair<int, int> oldOffset = oldLoc.offset(oldStartLoc);
      Tile oldT = *solutions[i].first.getTile(oldLoc);

      Location newLoc = locations[j];
      std::pair<int, int> newOffset = newLoc.offset(newStartLoc);
      Tile newT = *board.getTile(newLoc);


      /*Location newLoc2 = newStartLoc.getRelativeLocation(oldOffset);
      Tile* newT2 = NULL;
      if( board.inBounds(newLoc2) ){ newT2 = board.getTile(newLoc2); }*/
      Location oldLoc2 = oldStartLoc.getRelativeLocation(newOffset);
      Tile* oldT2 = NULL;
      if( board.inBounds(oldLoc2) ){ oldT2 = solutions[i].first.getTile(oldLoc2); }

      /*if( j == 6 || j == 7 ){
        std::cout << "\nOld start: " << oldStartLoc << ", new start: " << newStartLoc << std::endl;
        std::cout << "Tile " << j+1 << "\nOld offset: " << oldOffset.first << ", " << oldOffset.second << std::endl;
        std::cout << "New offset: " << newOffset.first << ", " << newOffset.second << std::endl;
        std::cout << "Old location: " << oldLoc << ", new location2: " << newLoc2 << std::endl;
      }*/

      /*std::cout << "Old tile:\tLocation: " << oldLoc << std::endl;
      oldT.printTile(std::cout);
      std::cout << "New tile:\tLocation: " << newLoc << std::endl;
      newT.printTile(std::cout);
      std::cout << "New tile2:\tLocation: " << newLoc2 << std::endl;
      newT2->printTile(std::cout);*/

      //if the tile locations are different or the tiles are different, solutions not the same so go on to the next solution
      if( !(oldLoc == newLoc) && (oldOffset != newOffset) ){//&& !(oldT2 != NULL && newT == *oldT2) ){ 
        // std::cout << "Tiles and locations are different" << std::endl;
        break; }
      //if the locations are the same and we've gone through the whole solution, then it's the same as an old solution
      if( j == locations.size()-1 ){ 
        // std::cout << "duplicate solution" << std::endl;
        return false; }
    }
  }
  //if we get to this point, we've gone through and compared this new solution to every solution we already had 
  //and found some difference in each comparison, so this solution is new
  return true;
}

// ==========================================================================
void PlaceTiles(int tNum, Board board, std::vector<Tile*> &tiles, std::vector<Location> locations, std::vector<std::pair<Board, std::vector<Location> > > &solutions, const bool &all_solutions, const bool &allow_rotations) {
  // std::cout << "Placing tile " << tNum+1 << std::endl;
  if( tNum == tiles.size() ){ 
    /*std::cout << "End possible solution: ";
    for(int i = 0; i < locations.size(); ++i){ std::cout << locations[i]; }
    std::cout << std::endl;
    board.Print(); */
    if( validSolution(board, tiles, locations) && unique(board, locations, solutions) ){
      // std::cout << "Solution is valid and unique" << std::endl;
      solutions.push_back( std::make_pair(board, locations) );
      if( all_solutions || solutions.size() == 1 ){
        std::cout << "Solution: ";
        for(int i = 0; i < locations.size(); ++i){ std::cout << locations[i]; }
        std::cout << std::endl;
        board.Print(); 
        std::cout << std::endl;
      }
      // std::cout << "Solutions so far: " << solutions.size() << std::endl;
    }
    return;
  }
  Tile* currTile = tiles[tNum];
  int r, c;
  int rows = board.numRows();
  int cols = board.numColumns();

  if( (rows <= 2) && (currTile->getNorth() != "pasture") && (currTile->getSouth() != "pasture") ){ return; }
  if( (cols <= 2) && (currTile->getEast() != "pasture") && (currTile->getWest() != "pasture") ){ return; }
  if( (rows == 1) && ( (currTile->getNorth() != "pasture") || (currTile->getSouth() != "pasture") ) ){ return; }
  if( (cols == 1) && ( (currTile->getEast() != "pasture") || (currTile->getWest() != "pasture") ) ){ return; }
  
  bool placed = false;

  int k = 1;
  if(allow_rotations){ k = 4; }
  if( tNum == 0 ){
    for(int i = 0; i < rows; ++i){
      for (int j = 0; j < cols; ++j){
        //for(int timesRotated = 0; timesRotated < k; ++timesRotated){
          if( legalPlacement(i, j, board, currTile, false) ){
            r = i; c = j;
            //CheckEdges(r, c, currTile, rows, cols);
            locations.push_back( Location(r, c, 0) );
            board.setTile(r, c, currTile);
            placed = true;
            // std::cout << "Tile " << tNum+1 << " added" << std::endl;
            // board.Print();
            PlaceTiles(tNum+1, board, tiles, locations, solutions, all_solutions, allow_rotations);
          }
          if(placed){
            locations.pop_back();
            board.removeTile(r, c);
            placed = false;
          }
          //if(allow_rotations){ currTile->rotate(); }
        //}
      }
    }
  } else{
    for(int timesRotated = 0; timesRotated < k; ++timesRotated){
      int rot = currTile->getRotation();
      // std::cout << "Tile rotation: " << rot << std::endl;
      //checking next to all previously placed tiles
      for(int i = 0; i < tNum; i++){
        // std::cout << "Checking for a spot next to tile " << i+1 << std::endl;
      
        //check spot above tile
        r = locations[i].row-1;
        c = locations[i].column;
        // std::cout << "Checking above tile " << i+1 << std::endl;
        if( legalPlacement(r, c, board, currTile, false) ){
          locations.push_back( Location(r, c, rot) );
          board.setTile(r, c, currTile);
          placed = true;
          // std::cout << "Tile " << tNum+1 << " added" << std::endl;
          // board.Print();
          PlaceTiles(tNum+1, board, tiles, locations, solutions, all_solutions, allow_rotations);
        }
        if(placed){
          locations.pop_back();
          board.removeTile(r, c);
          placed = false;
        }

        //check spot to right of tile
        r = locations[i].row;
        c = locations[i].column+1;
        // std::cout << "Checking right of tile " << i+1 << std::endl;
        if( legalPlacement(r, c, board, currTile, false) ){
          locations.push_back( Location(r, c, rot) );
          board.setTile(r, c, currTile);
          placed = true;
          // std::cout << "Tile " << tNum+1 << " added" << std::endl;
          // board.Print();
          PlaceTiles(tNum+1, board, tiles, locations, solutions, all_solutions, allow_rotations);
        }
        if(placed){
          locations.pop_back();
          board.removeTile(r, c);
          placed = false;
        }

        //check spot below tile
        r = locations[i].row+1;
        c = locations[i].column;
        // std::cout << "Checking below tile " << i+1 << std::endl;
        if( legalPlacement(r, c, board, currTile, false) ){
          locations.push_back( Location(r, c, rot) );
          board.setTile(r, c, currTile);
          placed = true;
          // std::cout << "Tile " << tNum+1 << " added" << std::endl;
          // board.Print();
          PlaceTiles(tNum+1, board, tiles, locations, solutions, all_solutions, allow_rotations);
        }
        if(placed){
          locations.pop_back();
          board.removeTile(r, c);
          placed = false;
        }

        //check spot to left of tile
        r = locations[i].row;
        c = locations[i].column-1;
        // std::cout << "Checking left of tile " << i+1 << std::endl;
        if( legalPlacement(r, c, board, currTile, false) ){
          locations.push_back( Location(r, c, rot) );
          board.setTile(r, c, currTile);
          placed = true;
          // std::cout << "Tile " << tNum+1 << " added" << std::endl;
          // board.Print();
          PlaceTiles(tNum+1, board, tiles, locations, solutions, all_solutions, allow_rotations);
        }
        if(placed){
          locations.pop_back();
          board.removeTile(r, c);
          placed = false;
        }

      }

      if(allow_rotations){ currTile->rotate(); }

    }

  }

}

// ==========================================================================
void PrintSolutions(int rows, int columns, std::vector<Tile*> &tiles, const bool all_solutions, const bool allow_rotations){
  Board board(rows,columns);
  std::vector<Location> locations;
  std::vector<std::pair<Board, std::vector<Location> > > solutions;
  PlaceTiles(0, board, tiles, locations, solutions, all_solutions, allow_rotations);

  if( solutions.size() > 0 ){
    /*int k = 1;
    if(all_solutions){ k = solutions.size(); }
    for(int i = 0; i < k; ++i){
      // print the solution to cout
      std::cout << "Solution: ";
      for (int j = 0; j < solutions[i].second.size(); j++) {
        std::cout << solutions[i].second[j];
      }
      std::cout << std::endl;
      // print the ASCII art board representation
      solutions[i].first.Print();
      std::cout << std::endl;
    }*/
    if(all_solutions){ 
      std::cout << "Found " << solutions.size() << " Solution(s)." << std::endl;
    }
  } else{
    std::cout << "No Solutions." << std::endl;
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
  if (rows < 1  ||  columns < 1  ||  rows * columns < tiles.size()) {
    std::cerr << "ERROR: specified board is not large enough" << rows << "X" << columns << "=" << rows*columns << " " << tiles.size() << std::endl;
    usage(argc,argv);
  }

  //PrintRandom(2, rows, columns, tiles);

  PrintSolutions(rows, columns, tiles, all_solutions, allow_rotations);

  // delete the tiles
  for (int t = 0; t < tiles.size(); t++) {
    delete tiles[t];
  }

}
// ==========================================================================







