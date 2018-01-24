// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cassert>
#include <cstdlib>
#include "MersenneTwister.h"

// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  std::map<std::string, std::map<std::string, int> >  MY_MAP ;
typedef  std::map<std::string, MY_MAP>  MY_MAP2 ;


// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
	// break words at whitespace
	return true;
      }
    } else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
	istr.get(c);
	word.push_back(c);
      }
      return true;
    } else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    } else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(MY_MAP &data, MY_MAP2 &data2, const std::string &filename, int window, const std::string &parse_method) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }
  std::string word;
  std::string prev_word = " ";
  std::string two_prev_word = " ";
  
  while (ReadNextWord(istr,word)) {
    // skip the quotation marks (not used for this part)
    if (word == "\"") continue;
    
    if(window == 2){
      MY_MAP::iterator a = data.find(word);
      if (a == data.end()){
        std::map<std::string, int> temp;
        data.insert(std::make_pair(word, temp));
      }
      if(prev_word != " ") data[prev_word][word]++;    
      prev_word = word;

    } else if(window == 3){
      MY_MAP2::iterator a = data2.find(word);
      if (a == data2.end()){
        MY_MAP temp;
        data2.insert(std::make_pair(word, temp));
      }
      if(prev_word != " "){ 
        MY_MAP::iterator b = data2[prev_word].find(word);
        if(b == data2.end()){
          std::map<std::string, int> temp;
          data2[prev_word].insert(std::make_pair(word, temp));
        }
      }
      if(two_prev_word != " "){ data2[two_prev_word][prev_word][word]++; }
      two_prev_word = prev_word;
      prev_word = word;
    }

  }

}



int main () {

  // ASSIGNMENT: THE MAIN DATA STRUCTURE
  MY_MAP data;
  MY_MAP2 data2;

  // Parse each command
  std::string command;    
  while (std::cin >> command) {

    // load the sample text file
    if (command == "load") {
      std::string filename;
      int window;
      std::string parse_method;
      std::cin >> filename >> window >> parse_method;      
      LoadSampleText(data, data2, filename, window, parse_method);
      std::cout << "Loaded " << filename << " with window = " << window << " and parse method = " << parse_method << std::endl;
    } 

    // print the portion of the map structure with the choices for the
    // next word given a particular sequence.
    else if (command == "print") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);
      
      if(data.size() > 0){
        std::map<std::string,int> inside_map = data[sentence[0]];
        std::map<std::string,int>::iterator p = inside_map.begin();
        int temp = 0;
        for(; p != inside_map.end(); p++){
          temp += p->second;
        }
        std::cout << sentence[0] << " (" << temp << ')' << std::endl;
        for(p = inside_map.begin(); p != inside_map.end(); p++){
          std::cout << sentence[0] << ' '<< p->first << " (" << p->second << ')' << std::endl; 
        }

      } else if(data2.size() > 0){
        MY_MAP middle_map = data2[sentence[0]];
        int word_count = 0;
        if(sentence.size() == 2){
          std::map<std::string,int> inside_map = middle_map[sentence[1]];
          std::map<std::string,int>::iterator p = inside_map.begin();
          for(; p != inside_map.end(); p++){
            word_count += p->second;
          }
          std::cout << sentence[0] << " " << sentence[1] << " (" << word_count << ")" << std::endl;
          for(p = inside_map.begin(); p != inside_map.end(); p++){
            std::cout << sentence[0] << " " << sentence[1] << " " << p->first << " (" << p->second << ")" << std::endl; 
          }

        } else{
          MY_MAP::iterator mid_itr = middle_map.begin();
          std::map<std::string, int> phrase_count;
          for(; mid_itr != middle_map.end(); mid_itr++){
            std::map<std::string,int> inside_map = mid_itr->second;
            std::map<std::string,int>::iterator in_itr = inside_map.begin();
            for(; in_itr != inside_map.end(); in_itr++){
              word_count += in_itr->second;
              phrase_count[mid_itr->first] += in_itr->second;
            }
          }
          std::cout << sentence[0] << " (" << word_count << ")" << std::endl;
          for(mid_itr = middle_map.begin(); mid_itr != middle_map.end(); mid_itr++){
            if(sentence[0] != mid_itr->first){
              std::cout << sentence[0] << " " << mid_itr->first << " (" << phrase_count[mid_itr->first] << ")" << std::endl;
            }
          }
        }

      }

    }

    // generate the specified number of words 
    else if (command == "generate") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);
      // how many additional words to generate
      int length;
      std::cin >> length;
      std::string selection_method;
      std::cin >> selection_method;
      //bool random_flag;
      if (selection_method == "random") {
        //random_flag = true;
        std::cout << sentence[0];
        if(data.size() > 0){
          std::string random_next = sentence[0];
          std::map<std::string,int> inside_map = data[random_next];
          for(int i = 0; i < length; i++){
            MTRand mtrand;
            int j = mtrand.randInt(inside_map.size());
            std::map<std::string,int>::iterator p = inside_map.begin();
            for(int k = 0; k < j; k++, p++){}
            std::cout << " " << p->first;
            random_next = p->first;
          }

        } else if(data2.size() > 0){
          std::string random_next = sentence[0];
          if(sentence.size() == 2){
            random_next = sentence[1];
            std::cout << " " << sentence[1];
          }
          MY_MAP inside_map = data2[random_next];
          for(int i = 0; i < length; i++){
            MTRand mtrand;
            int j = mtrand.randInt(inside_map.size());
            MY_MAP::iterator p = inside_map.begin();
            for(int k = 0; k < j; k++, p++){}
            std::cout << " " << p->first;
            random_next = p->first;
          }

        }
      } else {
        assert (selection_method == "most_common");
        //random_flag = false;
        std::cout << sentence[0];
        if(data.size() > 0){
          std::string most_common_next = sentence[0];
          for(int i = 0; i < length; i++){
            std::map<std::string,int> inside_map = data[most_common_next];
            std::map<std::string,int>::iterator p = inside_map.begin();
            int most_common_times = 0;
            for(; p != inside_map.end(); p++){
              if (p->second > most_common_times){
                most_common_times = p->second;
                most_common_next = p->first;
              }else if(p->second == most_common_times){
                if(p->first < most_common_next) most_common_next = p->first;
              }
            }
            std::cout << " " << most_common_next;
          }

        } else if(data2.size() > 0){
          std::string most_common_next = sentence[0];
          if(sentence.size() == 2){
            most_common_next = sentence[1];
            std::cout << " " << sentence[1];
          }
          for(int i = 0; i < length; i++){
            MY_MAP middle_map = data2[most_common_next];
            MY_MAP::iterator mid_itr = middle_map.begin();
            int most_common_times = 0;
            for(; mid_itr != middle_map.end(); mid_itr++){
              std::map<std::string,int> inside_map = mid_itr->second;
              std::map<std::string,int>::iterator p = inside_map.begin();
              int phrase_count = 0;
              for(; p != inside_map.end(); p++){
                phrase_count += p->second;
              }
              if (phrase_count > most_common_times){
                most_common_times = phrase_count;
                most_common_next = mid_itr->first;
              }else if(phrase_count == most_common_times){
                if(mid_itr->first < most_common_next) most_common_next = mid_itr->first;
              }
              std::cout << " " << most_common_next;
            }
          }
        }
      }
      std::cout << std::endl;

    } else if (command == "quit") {
      break;
    } else {
      std::cout << "WARNING: Unknown command: " << command << std::endl;
    }
    std::cout << std::endl;
  }
}
