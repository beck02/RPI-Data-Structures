#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <utility>
#include <vector>
#include "image.h"
using namespace std;

// ===================================================================================================
// ===================================================================================================

class OffsetCell {
public:
  OffsetCell(int ox = 0, int oy = 0) : cell_x(ox), cell_y(oy) {}
  int cell_x, cell_y;
  vector<pair<int,int> > in_coords_list;
};

//typedef pair< pair<int,int>, vector<pair<int,int> > > offset_cell;

bool commonFactors(int a, int b) {
  //cout << "checking for factors of " << a << " and " << b << endl;
  int divisor = min(a, b);
  while(divisor > 1){
    if( (a % divisor == 0) && (b % divisor == 0) ){
      //cout << "factor found: " << divisor << endl;
      return true;
    }
    divisor--;
  }
  //cout << "no factors found" << endl;
  return false;
}

void increaseTableSize2(int& s_offset, int& s_hash);
void increaseTableSize(int& s_offset, int& s_hash){
  s_offset++;
  //cout << "increased size 1" << endl;
  if(commonFactors(s_offset, s_hash)){
    increaseTableSize2(s_offset, s_hash);
  }
}
void increaseTableSize2(int& s_offset, int& s_hash){
  s_hash++;
  //cout << "increased size 2" << endl;
  if(commonFactors(s_offset, s_hash)){
    increaseTableSize(s_offset, s_hash);
  }
}

bool sortOffsetCellsByNumValues(const OffsetCell &lhs, const OffsetCell &rhs){ 
  return lhs.in_coords_list.size() > rhs.in_coords_list.size();
}

bool checkOffsetValuesForOffsetCell(const Offset& od, const OffsetCell& cell, int s_hash, const Image<Color>& hash_data);

// ===================================================================================================

void Compress(const Image<Color> &input, 
              Image<bool> &occupancy, Image<Color> &hash_data, Image<Offset> &offset) {
  //create the occupancy image by scanning over each pixel in the input image, and count the non-white pixels 
  occupancy.Allocate(input.Width(), input.Height());
  double p_count = 0;
  for(int x = 0; x < input.Width(); ++x){
    for(int y = 0; y < input.Height(); ++y){
      if( input.GetPixel(x, y).isWhite() ){
        occupancy.SetPixel(x, y, false);
      } else{
        occupancy.SetPixel(x, y, true);
        p_count++;
      }
    }
  }

  //calculate sizes for the hashdata and offset images, and make sure the sizes
  //don't have common factors that would cause overlap
  int s_hash = (int) ceil(sqrt(1.01 * p_count));
  int s_offset = (int) ceil(sqrt(p_count/4));
  
  if(commonFactors(s_hash, s_offset)){
    //cout << "initial s_hash: " << s_hash << ", initial s_offset: " << s_offset << endl;
    increaseTableSize(s_offset, s_hash);
    //cout << "end initial resizing" << endl;
  }

try_again:
  //cout << "s_hash: " << s_hash << ", s_offset: " << s_offset << endl;
  hash_data.Allocate(s_hash, s_hash);
  hash_data.SetAllPixels(Color());
  offset.Allocate(s_offset, s_offset);

  //determine which (and thus also how many) non-white pixels from the input image map
  //to each cell in the offset image/table
  vector<OffsetCell> off_cells(s_offset*s_offset);
  for(int x_in = 0; x_in < input.Width(); ++x_in){
    for(int y_in = 0; y_in < input.Height(); ++y_in){
      if( !input.GetPixel(x_in, y_in).isWhite() ){
        int x_off = x_in % s_offset;
        int y_off = y_in % s_offset;
        pair<int,int> curr_in_coords = make_pair(x_in, y_in);
        OffsetCell& curr_cell = off_cells[y_off*s_offset + x_off];
        curr_cell.cell_x = x_off;
        curr_cell.cell_y = y_off;
        curr_cell.in_coords_list.push_back(curr_in_coords);
      }
    }
  }
  //sort the offset table vector/cells by the size of their vector of coordinates
  //(the number of values mapped to the cell)
  sort(off_cells.begin(), off_cells.end(), sortOffsetCellsByNumValues);

  //start entering the offset and hashdata by going through each offset cell and
  //entering each bit of the input data that maps to it
  for(int cell = 0; cell < off_cells.size(); ++cell){
    OffsetCell& curr_cell = off_cells[cell];

    // cout << "Cell: (" << curr_cell.cell_x << ", " << curr_cell.cell_y << "), Num vals: "
    //      << curr_cell.in_coords_list.size() << endl;
    
    int x_data, y_data;
    bool entered = false;
    //check each possible offset to find one where values can be mapped without collisions
    for(x_data = 0; x_data < min(s_hash,16) && !entered; ++x_data){
      for(y_data = 0; y_data < min(s_hash,16) && !entered; ++y_data){
        
        Offset cell_off_data(x_data, y_data);
        
        //check make sure the current offset data values allow all input values to correctly map to
        //the hashdata image without collisions 
        if(checkOffsetValuesForOffsetCell(cell_off_data, curr_cell, s_hash, hash_data)){
          offset.SetPixel(curr_cell.cell_x, curr_cell.cell_y, cell_off_data);
          //go through each value that maps to the current offset cell and enter it in the hashdata image
          for(int pxl = 0; pxl < curr_cell.in_coords_list.size(); ++pxl){
            int x_in = curr_cell.in_coords_list[pxl].first;
            int y_in = curr_cell.in_coords_list[pxl].second;
            int x_hash = (x_in + cell_off_data.dx) % s_hash;
            int y_hash = (y_in + cell_off_data.dy) % s_hash;
            hash_data.SetPixel(x_hash, y_hash, input.GetPixel(x_in, y_in));
          }
          entered = true;
        }//end if current offset works with no collisions

      }//end for y_data values
    }//end for x_data values
    
    //if no possible collision-less offset exists, increase the offset size and start over
    if(!entered){
      //cout << "trying again..." << endl;
      increaseTableSize(s_offset, s_hash);
      goto try_again;
    }


  }//end for each offset cell

  cout << "s_hash: " << s_hash << ", s_offset: " << s_offset << endl;
  
}//end Compress

// ===================================================================================================

bool checkOffsetValuesForOffsetCell(const Offset& od, const OffsetCell& cell, int s_hash, const Image<Color>& hash_data){
  for(int pxl = 0; pxl < cell.in_coords_list.size(); ++pxl){
    int x_in = cell.in_coords_list[pxl].first;
    int y_in = cell.in_coords_list[pxl].second;
    int x_hash = (x_in + od.dx) % s_hash;
    int y_hash = (y_in + od.dy) % s_hash;
    if( !hash_data.GetPixel(x_hash, y_hash).isWhite() ){ return false; }
  }
  return true;
}

// ===================================================================================================

void UnCompress(const Image<bool> &occupancy, const Image<Color> &hash_data, const Image<Offset> &offset, 
                Image<Color> &output) {
  // cout << "Input image sizes:\n" << "  Occupancy: " << occupancy.Width() << " x " << occupancy.Height()
  //           << "\n  Hash Data: " << hash_data.Width() << " x " << hash_data.Height() << "\n  Offset: "
  //           << offset.Height() << " x " << offset.Width() << endl;
  output.Allocate(occupancy.Width(), occupancy.Height());
  //int s_offset = offset.Width() * offset.Height();
  //int s_hash = hash_data.Width() * hash_data.Height();
  for(int x = 0; x < occupancy.Width(); ++x){
    for(int y = 0; y < occupancy.Height(); ++y){
      //cout << "Pixel location: (" << x << ", " << y << ")" << endl;
      if(occupancy.GetPixel(x, y)){
        
        int x_off = x % offset.Width(); int y_off = y % offset.Width();
        //cout << "  Offset Loc: (" << x_off << ", " << y_off << ")";
        Offset off_val = offset.GetPixel(x_off, y_off);
        //cout << ", Offset Data: dx = " << (int)off_val.dx << ", dy = " << (int)off_val.dy << endl;

        int x_hash = (x + off_val.dx) % hash_data.Width(); int y_hash = (y + off_val.dy) % hash_data.Width();
        //cout << "  Hash Loc: (" << x_hash << ", " << y_hash << ")" << endl;
        Color pixel = hash_data.GetPixel(x_hash, y_hash);
        output.SetPixel(x, y, pixel);
      } else{
        output.SetPixel(x, y, Color());
      }
    }
  }

}


// ===================================================================================================
// ===================================================================================================

// Takes in two 24-bit color images as input and creates a b&w output
// image (black where images are the same, white where different)
void Compare(const Image<Color> &input1, const Image<Color> &input2, Image<bool> &output) {

  // confirm that the files are the same size
  if (input1.Width() != input2.Width() ||
      input1.Height() != input2.Height()) {
    std::cerr << "Error: can't compare images of different dimensions: " 
         << input1.Width() << "x" << input1.Height() << " vs " 
         << input2.Width() << "x" << input2.Height() << std::endl;
  } else {
    // make sure that the output images is the right size to store the
    // pixel by pixel differences
    output.Allocate(input1.Width(),input1.Height());
    int count = 0;
    for (int i = 0; i < input1.Width(); i++) {
      for (int j = 0; j < input1.Height(); j++) {
        Color c1 = input1.GetPixel(i,j);
        Color c2 = input2.GetPixel(i,j);
        if (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)
          output.SetPixel(i,j,true);
        else {
          count++;
          output.SetPixel(i,j,false);
        }
      }      
    }     

    // confirm that the files are the same size
    if (count == 0) {
      std::cout << "The images are identical." << std::endl;
    } else {
      std::cout << "The images differ at " << count << " pixel(s)." << std::endl;
    }
  }
}

// ===================================================================================================
// ===================================================================================================

// to allow visualization of the custom offset image format
void ConvertOffsetToColor(const Image<Offset> &input, Image<Color> &output) {
  // prepare the output image to be the same size as the input image
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < output.Width(); i++) {
    for (int j = 0; j < output.Height(); j++) {
      // grab the offset value for this pixel in the image
      Offset off = input.GetPixel(i,j);
      // set the pixel in the output image
      int dx = off.dx;
      int dy = off.dy;
      assert (dx >= 0 && dx <= 15);
      assert (dy >= 0 && dy <= 15);
      // to make a pretty image with purple, cyan, blue, & white pixels:
      int red = dx * 16;
      int green = dy *= 16;
      int blue = 255;
      assert (red >= 0 && red <= 255);
      assert (green >= 0 && green <= 255);
      output.SetPixel(i,j,Color(red,green,blue));
    }
  }
}

// ===================================================================================================
// ===================================================================================================

void usage(char* executable) {
  std::cerr << "Usage:  4 options" << std::endl;
  std::cerr << "  1)  " << executable << " compress input.ppm occupancy.pbm data.ppm offset.offset" << std::endl;
  std::cerr << "  2)  " << executable << " uncompress occupancy.pbm data.ppm offset.offset output.ppm" << std::endl;
  std::cerr << "  3)  " << executable << " compare input1.ppm input2.ppm output.pbm" << std::endl;
  std::cerr << "  4)  " << executable << " visualize_offset input.offset output.ppm" << std::endl;
}

// ===================================================================================================
// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc < 2) { usage(argv[0]); exit(1); }

  if (argv[1] == std::string("compress")) {
    if (argc != 6) { usage(argv[0]); exit(1); }
    // the original image:
    Image<Color> input;
    // 3 files form the compressed representation:
    Image<bool> occupancy;
    Image<Color> hash_data;
    Image<Offset> offset;
    input.Load(argv[2]);
    Compress(input,occupancy,hash_data,offset);
    // save the compressed representation
    occupancy.Save(argv[3]);
    hash_data.Save(argv[4]);
    offset.Save(argv[5]);

  } else if (argv[1] == std::string("uncompress")) {
    if (argc != 6) { usage(argv[0]); exit(1); }
    // the compressed representation:
    Image<bool> occupancy;
    Image<Color> hash_data;
    Image<Offset> offset;
    occupancy.Load(argv[2]);
    hash_data.Load(argv[3]);
    offset.Load(argv[4]);
    // the reconstructed image
    Image<Color> output;
    UnCompress(occupancy,hash_data,offset,output);
    // save the reconstruction
    output.Save(argv[5]);
  
  } else if (argv[1] == std::string("compare")) {
    if (argc != 5) { usage(argv[0]); exit(1); }
    // the original images
    Image<Color> input1;
    Image<Color> input2;    
    input1.Load(argv[2]);
    input2.Load(argv[3]);
    // the difference image
    Image<bool> output;
    Compare(input1,input2,output);
    // save the difference
    output.Save(argv[4]);

  } else if (argv[1] == std::string("visualize_offset")) {
    if (argc != 4) { usage(argv[0]); exit(1); }
    // the 8-bit offset image (custom format)
    Image<Offset> input;
    input.Load(argv[2]);
    // a 24-bit color version of the image
    Image<Color> output;
    ConvertOffsetToColor(input,output);
    output.Save(argv[3]);

  } else {
    usage(argv[0]);
    exit(1);
  }
}

// ===================================================================================================
// ===================================================================================================
