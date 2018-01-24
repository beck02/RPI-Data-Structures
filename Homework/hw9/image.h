#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cassert>
#include <string>
#include <iostream>

// ====================================================================
// 24 bit color pixel
class Color {
public:
  Color(unsigned char red=255, unsigned char green=255, unsigned char blue=255) : r(red),g(green),b(blue) {}
  bool isWhite() const { return r==255 && g==255 && b==255; }
  unsigned char r,g,b;
};

// ====================================================================
// 8 bit offset (dx = 4 bits, dy = 4 bits)
class Offset {
public:
  Offset(unsigned char dx_=0, unsigned char dy_=0) : dx(dx_),dy(dy_) {}
  unsigned char dx, dy;
};


// ====================================================================
// ====================================================================
// TEMPLATED IMAGE CLASS
//    can be saved and loaded from standard file formats:
//      .ppm    (when T == Color)
//      .pbm    (when T == bool)
//    and this custom file format:
//      .offset (when T == Offset)
//

template <class T>
class Image {
public:
  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Image() : width(0), height(0), data(NULL) {}
  Image(const Image &image) : data(NULL) { 
    copy_helper(image); }
  const Image& operator=(const Image &image) { 
    if (this != &image)
      copy_helper(image);
    return *this; }
  ~Image() {
    delete [] data; 
  }

  // initialize an image of a specific size
  void Allocate(int w, int h) {
    width = w;
    height = h;
    delete [] data;
    if (width == 0 && height == 0) {
      data = NULL;
    } else {
      assert (width > 0 && height > 0);
      data = new T[width*height]; 
    }
  }

  // =========
  // ACCESSORS
  int Width() const { return width; }
  int Height() const { return height; }
  const T& GetPixel(int x, int y) const {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    return data[y*width + x]; }

  // =========
  // MODIFIERS
  void SetAllPixels(const T &value) {
    for (int i = 0; i < width*height; i++) {
      data[i] = value; } }
  void SetPixel(int x, int y, const T &value) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    data[y*width + x] = value; }

  // ===========
  // LOAD & SAVE
  bool Load(const std::string &filename);
  bool Save(const std::string &filename) const; 
  
private:
  // private helper function
  void copy_helper(const Image &image) {
    Allocate (image.Width(), image.Height());
    for (int i = 0; i < image.Width(); i++) {
      for (int j = 0; j < image.Height(); j++) {
        this->SetPixel(i,j,image.GetPixel(i,j));
      }
    }
  }

  // ==============
  // REPRESENTATION
  int width;
  int height;
  T *data;
};

#endif
