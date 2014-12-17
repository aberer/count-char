#ifndef COUNTER_H
#define COUNTER_H

#include <string>

using std::string;

/** 
    @brief counts the characters in a part of a file 
 */ 
class Counter
{
public:
  Counter(string fileName, size_t readerRank, size_t readerSize);
  void count();
  void printResult() const;

public:                         // FRIENDS 
  friend Counter operator+( Counter const& lhs, Counter const& rhs );
  friend bool operator==(Counter const& lhs, Counter const& rhs); 
  
private:
  string _fn;
  size_t _offset;
  size_t _length;
  size_t _fileSize; 
  size_t _counts[256]; 
};

#endif /* COUNTER_H */
