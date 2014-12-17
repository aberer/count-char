#include "Counter.hpp"

#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <fstream>
#include <thread>
#include <vector>

using std::cout;
using std::endl; 

using std::ifstream;
using std::ios_base;
using std::vector; 


Counter::Counter(string fileName, size_t readerRank, size_t readerSize)
  : _fn{fileName}
  , _offset{0}
  , _length{0}
  , _fileSize{0}
{
  auto&& tmp = ifstream(_fn);
  tmp.seekg(0, ios_base::end);
  _fileSize = tmp.tellg();

  auto minReading = _fileSize / readerSize;
  
  auto rest = _fileSize - minReading * readerSize;

  auto offsets = vector<size_t>{};
  auto lengths = vector<size_t>{};
  auto curOffset = 0u;

  for(auto i = 0u; i < readerSize; ++i)
    {
      offsets.push_back(curOffset);
      lengths.push_back( minReading + (i  <  rest ? 1 : 0  ) );
      curOffset += lengths.back();
    }

  _offset = offsets.at(readerRank);
  _length = lengths.at(readerRank);
  
  std::fill(_counts, _counts + 256, 0u );
  
  // cout << "thread " << readerRank << " / " << readerSize << " reading at offset " << _offset << " for length " << _length << endl; 
}


Counter operator+( Counter const& lhs, Counter const& rhs )
{
  auto cpy = lhs;
  for(int i =0 ; i < 256; ++i)
    cpy._counts[i] = lhs._counts[i] +  rhs._counts[i];
  return cpy; 
}



bool operator==(Counter const& lhs, Counter const& rhs)
{
  for(int i = 0; i < 256 ; ++i)
    if(lhs._counts[i] != rhs._counts[i])
      return false;
  return true; 
}


void Counter::count()
{
  auto&& in = ifstream(_fn);
  in.seekg(_offset, ios_base::beg);
  
  auto numRead = 0u;
  while(numRead  < _length )
    {
      char c ;  
      in.get(c);
      ++_counts[static_cast<size_t>(c)];
      ++numRead; 
    }
}


void Counter::printResult() const
{
  for(int i = 0; i < 256 ; ++i)
    {
      if(  _counts[i] > 0 )
        {
          cout << static_cast<char>(i) << "\t" << _counts[i] << endl;
        }
    }
}
