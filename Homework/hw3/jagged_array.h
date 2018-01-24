#ifndef JagArr_h_
#define JagArr_h_

template <class T> class JaggedArray {

public:   
  // TYPEDEFS
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef unsigned int size_type;

  // CONSTRUCTORS, ASSIGNMNENT OPERATOR, & DESTRUCTOR
  JaggedArray() { this->create(); }
  JaggedArray(int n) { this->create(n); }
  JaggedArray(const JaggedArray& v) { this->copy(v); }
  JaggedArray& operator=(const JaggedArray& v); 
  ~JaggedArray() { this->wipe(); }

  // MEMBER FUNCTIONS AND OTHER OPERATORS
  //T& operator[] (int i) { return m_data[i]; }
  //const T& operator[] (int i) const { return m_data[i]; }
  void pack();
  void unpack();
  void print();
  //iterator erase(iterator p);

  // ITERATOR OPERATIONS
  /*iterator begin() { return m_data; }
  const_iterator begin() const { return m_data; }
  iterator end() { return m_data + m_size; }
  const_iterator end() const { return m_data + m_size; }*/

  //Accessors
  int numElements() const { return m_num_elements; }
  int numBins() const { return m_num_bins; }
  bool isPacked() const { return ( (m_counts == NULL) && (m_unpacked_values == NULL) && 
  									(m_offsets != NULL) && (m_packed_values != NULL) ); }
  int numElementsInBin(int binNum) const;
  const T& getElement(int binNum, int slotNum) const;

  //Modifiers
  void addElement(int binNum, const T& t);
  void removeElement(int binNum, int slotNum);
  void clear();
  void wipe();

private:  
  // PRIVATE MEMBER FUNCTIONS
  void create();
  void create(int n);
  void copy(const JaggedArray<T>& v); 

  // REPRESENTATION
  int m_num_elements;
  int m_num_bins;
  int * m_counts;
  T* * m_unpacked_values;
  int * m_offsets;
  T * m_packed_values;
};

template <class T> void JaggedArray<T>::addElement(int binNum, const T& t){
  if ( isPacked() ){
    std::cerr << "JaggedArray must be unpacked to add/remove elements." << std::endl;
    exit(1);
  } else if( binNum >= m_num_bins ){
  	std::cerr << "Bin #" << binNum << " doesn't exist to add elements to." 
  								<< std::endl;
  	exit(1);
  } else{
    if (m_counts == NULL){ //in case the counts array hasn't been created/initialized yet
    	m_counts = new int[1](); //since it was empty, just make it size one for the new element
    	m_num_bins = 1;
    }
    if ( m_counts[binNum] == 0 ){ //in case the unpacked values array hasn't been created/initialized yet
      m_unpacked_values[binNum] = new T[1];
      m_unpacked_values[binNum][0] = t;
    } else{
      T* temp = m_unpacked_values[binNum];
      //make a new array one element larger than the old one
      m_unpacked_values[binNum] = new T[m_counts[binNum] + 1]; 
      for (int i = 0; i < m_counts[binNum]; ++i){ 
        m_unpacked_values[binNum][i] = temp[i]; //copy everything from the old array to the new
      }
      //add the new element at the end
      m_unpacked_values[binNum][m_counts[binNum]] = t;
      delete [] temp;
      temp = NULL;
    }
    m_counts[binNum]++;
    m_num_elements++;
  }
}

template <class T> void JaggedArray<T>::removeElement(int binNum, int slotNum){
  if ( isPacked() ){
    std::cerr << "JaggedArray must be unpacked to add/remove elements." << std::endl;
    exit(1);
  } else if( binNum >= m_num_bins ){
  	std::cerr << "Bin #" << binNum << " doesn't exist to remove elements from." 
  								<< std::endl;
  	exit(1);
  } else if( slotNum > m_counts[binNum] ){
  	std::cerr << "Slot #" << slotNum << " doesn't exist in bin " << binNum 
  									<< " to remove an element from." << std::endl;
  	exit(1);
	} else{
    if (m_counts == NULL){
    	m_counts = new int[1]();
    	m_num_bins = 1;
    }
    if ( m_counts[binNum] == 1 ){
      delete [] m_unpacked_values[binNum];
      m_unpacked_values[binNum] = NULL;
    } else{
      T* temp = m_unpacked_values[binNum];
      m_unpacked_values[binNum] = new T[m_counts[binNum] - 1];
      for (int i = 0; i < slotNum; ++i){
        m_unpacked_values[binNum][i] = temp[i];
      }
      for (int i = slotNum + 1; i < m_counts[binNum]; ++i){
        m_unpacked_values[binNum][i-1] = temp[i];
      }
      delete [] temp;
      temp = NULL;
    }
    m_counts[binNum]--;
    m_num_elements--;
  }
}


template <class T> void JaggedArray<T>::pack(){ //method to pack an array
	if( !isPacked() ){
	  m_offsets = new int[m_num_bins]();
	  m_packed_values = new T[m_num_elements];
	  int pkindex = 0;
	  for (int binNum = 0; binNum < m_num_bins; ++binNum){
	    m_offsets[binNum] = pkindex;
	    for (int i = 0; i < m_counts[binNum]; ++i){
	      m_packed_values[pkindex] = m_unpacked_values[binNum][i];
	      pkindex++;
	    }
	  }
	  delete [] m_counts;
	  m_counts = NULL;
	  for(int i = 0; i < m_num_bins; i++){
	     delete [] *(m_unpacked_values+i);
	    *(m_unpacked_values+i) = NULL;
	  }
	  delete [] m_unpacked_values;
	  m_unpacked_values = NULL;
	}
}

template <class T> void JaggedArray<T>::unpack(){ //method to unpack an array
  if( isPacked() ){
	  delete [] m_counts;
	  m_counts = new int[m_num_bins]();
	  m_unpacked_values = new T*[m_num_bins];
	  int pkindex = 0;
	  for (int binNum = 0; binNum < m_num_bins; ++binNum){
	    if ( binNum == m_num_bins-1 ){
	      m_counts[binNum] = m_num_elements - m_offsets[binNum];
	    } else if( binNum < m_num_bins-1 ){
	      m_counts[binNum] = m_offsets[binNum+1] - m_offsets[binNum];
	    }
	    if ( m_counts[binNum] == 0 ){
	      m_unpacked_values[binNum] = NULL;
	    } else{
	      m_unpacked_values[binNum] = new T[m_counts[binNum]];
	      for (int i = 0; i < m_counts[binNum]; ++i){
	        m_unpacked_values[binNum][i] = m_packed_values[pkindex];
	        pkindex++;
	      }//end for elements in bin
	    }//end if/else
  	}//end for bins
  }//end if packed
  delete [] m_offsets;
  m_offsets = NULL;
  delete [] m_packed_values;
  m_packed_values = NULL;
}

template <class T> void JaggedArray<T>::print(){
  std::string spacing(2, ' ');
  if ( isPacked() ){ //print if packed
    std::cout << "packed JaggedArray" << std::endl;
    std::cout << spacing + "num_bins: " << m_num_bins << std::endl;
    std::cout << spacing + "num_elements: " << m_num_elements << std::endl;
    std::cout << spacing + "offsets:" + spacing;
    for (int i = 0; i < m_num_bins; ++i){
      std::cout << m_offsets[i] << " ";
    }
    std::cout << std::endl << spacing + "values: " + spacing;
    if( m_packed_values != NULL ){
	    for (int i = 0; i < m_num_elements; ++i){
	      std::cout << m_packed_values[i] << " ";
	    }
  	}
  } else{ //print if unpacked
    std::cout << "unpacked JaggedArray" << std::endl;
    std::cout << spacing + "num_bins: " << m_num_bins << std::endl;
    std::cout << spacing + "num_elements: " << m_num_elements << std::endl;
    std::cout << spacing + "counts:" + spacing;
    int mostEperB = 0; //find how many elements are in the largest bin to know how many lines to print
    for (int binNum = 0; binNum < m_num_bins; ++binNum){
      mostEperB = std::max(m_counts[binNum], mostEperB);
      std::cout << m_counts[binNum] << " ";
    }
    std::cout << std::endl << spacing + "values:" + spacing;
    for (int i = 0; i < mostEperB; ++i){
      if(i > 0){
        std::cout << std::endl << spacing + std::string(7, ' ') + spacing;
      }
      for(int binNum = 0; binNum < m_num_bins; ++binNum){
        if (m_counts[binNum] <= i){
          std::cout << "  ";
        } else{
          std::cout << m_unpacked_values[binNum][i] << " ";
        } //end if/else
      } //end for bins
    } //end for largest bin
  } // end print if unpacked
  std::cout << std::endl << std::endl;
}


template <class T> void JaggedArray<T>::clear() {
	for (int binNum = 0; binNum < m_num_bins; ++binNum){
		if (m_offsets != NULL){
			m_offsets[binNum] = 0;
		} //end offsets if
		if (m_counts != NULL){
			m_counts[binNum] = 0;
		} //end counts if
		if ( (m_unpacked_values != NULL) && (m_unpacked_values[binNum] != NULL) ){
			delete [] m_unpacked_values[binNum];
      m_unpacked_values[binNum] = NULL;
		} //end unpacked if
	} //end for loop
	if( m_packed_values != NULL ){
		delete [] m_packed_values;
		m_packed_values = NULL;
	} //end packed if
	m_num_elements = 0;
}

template <class T> void JaggedArray<T>::wipe(){
	//delete any memory for unpacked bin contents
	if(m_unpacked_values != NULL){
		for (int binNum = 0; binNum < m_num_bins; ++binNum){
			if(m_unpacked_values[binNum] != NULL){
				delete [] m_unpacked_values[binNum];
				m_unpacked_values[binNum] = NULL;
			}
		}
		//delete unpacked bins
		delete [] m_unpacked_values;
		m_unpacked_values = NULL;
	}
	//delete offsets
	if(m_offsets != NULL){
		delete [] m_offsets;
		m_offsets = NULL;
	}
	//delete packed values
	if(m_packed_values != NULL){
		delete [] m_packed_values;
		m_packed_values = NULL;
	}
	//delete bin counts
	if(m_counts != NULL){
		delete [] m_counts;
		m_counts = NULL;
	}
	//reset bin and element numbers
	m_num_bins = 0;
	m_num_elements = 0;
}

// Create an empty vector (null pointers everywhere).
template <class T> void JaggedArray<T>::create() {
  m_counts = NULL;
  m_unpacked_values = NULL;
  m_offsets = NULL;
  m_packed_values = NULL;
  m_num_bins = m_num_elements = 0;  // No memory allocated yet
}

// Create a vector with size n, each location having the given value
template <class T> void JaggedArray<T>::create(int n) {
  m_num_elements = 0;
  m_num_bins = n;
  m_counts = new int[n]();
  m_unpacked_values = new T*[n];
  for (int i = 0; i < n; ++i){
    m_unpacked_values[i] = NULL;
  }
  m_offsets = NULL;
  m_packed_values = NULL;
}

// Assign one vector to another, avoiding duplicate copying.
template <class T> JaggedArray<T>& JaggedArray<T>::operator=(const JaggedArray<T>& v) {
  if (this != &v) {
    this -> wipe();
    this -> copy(v);
  }
  return *this;
}

// Create the vector as a copy of the given vector. 
template <class T> void JaggedArray<T>::copy(const JaggedArray<T>& v) {
  create();
  this->m_num_elements = v.m_num_elements;
  this->m_num_bins = v.m_num_bins;
  if ( v.isPacked() ){ //if the array being copied is packed
    this->m_offsets = new int[this->m_num_bins];
    for (int i = 0; i < m_num_bins; ++i){
      this->m_offsets[i] = v.m_offsets[i];
    }
    this->m_packed_values = new T[this->m_num_elements];
    for (int i = 0; i < m_num_elements; ++i){
      this->m_packed_values[i] = v.m_packed_values[i];
    }
  } else{ //if the array being copied is unpacked
    this->m_counts = new int[this->m_num_bins];
    this->m_unpacked_values = new T*[this->m_num_bins];
    for (int binNum = 0; binNum < m_num_bins; ++binNum){
      this->m_counts[binNum] = v.m_counts[binNum];
      this->m_unpacked_values[binNum] = new T[m_counts[binNum]];
      for (int i = 0; i < m_counts[binNum]; ++i){
      	this -> m_unpacked_values[binNum][i] = v.m_unpacked_values[binNum][i];
      }//end for elements in bin
    }//end for each bin
  }//end if copied array is unpacked
}

template <class T> int JaggedArray<T>::numElementsInBin(int binNum) const {
	if( isPacked() ){
		if ( binNum == m_num_bins-1 ){
		 	return m_num_elements - m_offsets[binNum];
		} else{
			return m_offsets[binNum + 1] - m_offsets[binNum];
		}
	} else{
		return m_counts[binNum];
	}
}

template <class T> const T& JaggedArray<T>::getElement(int binNum, int slotNum) const {
	if( isPacked() ){
		return m_packed_values[ m_offsets[binNum] + slotNum ];
	} else{
		return m_unpacked_values[binNum][slotNum];
	}
}

// Shift each entry of the array after the iterator. Return the iterator, 
// which will have the same value, but point to a different location.
/*template <class T> typename JaggedArray<T>::iterator JaggedArray<T>::erase(iterator p) {
  // remember iterator and T* are equivalent
  for (iterator q = p; q < m_data+m_size; ++q)
    *q = *(q+1);
  m_size --;
  return p;
}*/


#endif
