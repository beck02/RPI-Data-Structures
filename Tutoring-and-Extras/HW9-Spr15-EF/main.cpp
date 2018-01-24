#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "image.h"
#include "priority_queue.h"

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }
  /*
  std::cout << "max_distance = " << max_distance << std::endl;
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      std::cout << "Pixel at:  i=" << i << ",j=" << j << " (" << distance_image.GetPixel(i,j).getX() << "," << distance_image.GetPixel(i,j).getY() << ") distance = " << distance_image.GetPixel(i,j).getValue() << std::endl;
    }
  }
  */
  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
	// EXTRA CREDIT: create other visualizations 
	std::cerr << "ERROR: Unknown visualization style" << std::endl;
	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }

  return 0;
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
      for (int i2 = 0; i2 < w; i2++) {
      	for (int j2 = 0; j2 < h; j2++) {
      	  const Color& c = input.GetPixel(i2,j2);      
      	  // skip all pixels that are not black
      	  if (!c.isBlack()) continue;
      	  // calculate the distance between the two pixels
      	  double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
      	  // store the closest distance to a black pixel
      	  if (closest < 0 || distance < closest) {
      	    closest = distance;
      	  }
      	}
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  double answer = 0;
  std::vector<DistancePixel> black_pixels;
  // loop over the pixels in the input image to see which ones are black
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      distance_image.GetPixel(i,j).setX(i);
      distance_image.GetPixel(i,j).setY(j);
      const Color& c = input.GetPixel(i,j);
      if(c.isBlack()) black_pixels.push_back(distance_image.GetPixel(i,j));
    }
  }
  // now calculate how far any given pixel is from a black pixel
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      double closest = -1;
      for(int k = 0; k < black_pixels.size(); k++){
        //find the distance between the pixel we're on and a given black pixel
        double dist = sqrt((i-black_pixels[k].getX())*(i-black_pixels[k].getX()) + (j-black_pixels[k].getY())*(j-black_pixels[k].getY()));
        if(closest < 0 || dist < closest){
          closest = dist;
        }
      }
      assert(closest >= 0);
      answer = std::max(answer, closest);
      //save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}

double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  // loop over the pixels in the input image to see which ones are black
  int w = input.Width();
  int h = input.Height();
  double answer = 0;
  std::vector<DistancePixel> black_pixels;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      distance_image.GetPixel(i,j).setX(i);
      distance_image.GetPixel(i,j).setY(j);
      const Color& c = input.GetPixel(i,j);
      if(c.isBlack()){
        black_pixels.push_back(distance_image.GetPixel(i,j)); 
        distance_image.GetPixel(i,j).setValue(0);
      }
    }
  }
  DistancePixel_PriorityQueue DP_PQ;
  for(int k = 0; k < black_pixels.size(); k++){
    DistancePixel& currPix = distance_image.GetPixel(black_pixels[k].getX(), black_pixels[k].getY());
    for(int i = -1; i < 2; i++){
      for(int j = -1; j < 2; j++){
        if(!(i == 0 && j == 0)){
          int adj_x = currPix.getX() + i;
          int adj_y = currPix.getY() + j;
          if((adj_x >= 0 && adj_x < w) && (adj_y >= 0 && adj_y < h)){
            DistancePixel& adjacent = distance_image.GetPixel(adj_x, adj_y);
            double dist = 1;
            if(i != 0 && j != 0 ) dist = sqrt(2);
            if(adjacent.getValue() > dist + currPix.getValue()){
              adjacent.setValue(dist + currPix.getValue());
              if(DP_PQ.in_heap(&adjacent)) DP_PQ.update_position(&adjacent);
              else DP_PQ.push(&adjacent);
            }
          }
        }
      }
    }
  }
  while(!(DP_PQ.empty())){
    const DistancePixel* root = DP_PQ.top();
    DP_PQ.pop();
    for(int i = -1; i < 2; i++){
      for(int j = -1; j < 2; j++){
        if(!(i == 0 && j == 0)){
          int adj_x = (*root).getX() + i;
          int adj_y = (*root).getY() + j;
          if((adj_x >= 0 && adj_x < w) && (adj_y >= 0 && adj_y < h)){
            DistancePixel* adjacent = &distance_image.GetPixel(adj_x, adj_y);
            double dist = 1;
            if(i != 0 && j != 0 ) dist = sqrt(2);
            if((*adjacent).getValue() > dist + (*root).getValue()){
              (*adjacent).setValue(dist + (*root).getValue());
              if(DP_PQ.in_heap(adjacent)) DP_PQ.update_position(adjacent);
              else DP_PQ.push(adjacent);
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      answer = std::max(distance_image.GetPixel(i,j).getValue(), answer);
    }
  }
  return answer;
}

// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}

// ===================================================================================================
