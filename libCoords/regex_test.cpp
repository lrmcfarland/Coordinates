// ================================================================
// Filename:    regex_test.cpp
// Description: tests c++11 vs. boost regex for centos 6.x compiles
//
//              with c++11 on gcc 4.8.2 this raises regex_error
//
//
// Author:      L.R. McFarland
// Created:     2015 Sep 04
// ================================================================

#include <iostream>
#include <sstream>


#if BOOST_REGEX // from compiler -D option
#include <boost/regex.hpp>
#else
#include <regex>
#endif


int main(int argc, char **argv) {

  const std::string cc_fmt("(\\d{4}[- ]){3}\\d{4}");

  std::cout << "test regex: " << cc_fmt << std::endl;


  // concatinate the args to make the test string
  std::stringstream test_buffer;
  for (int i(1); i < argc; ++i) {

    test_buffer << argv[i];
    if (i < argc - 1) 
      test_buffer << " ";

  }

  std::cout << "test buffer: " << test_buffer.str() << std::endl;


#if BOOST_REGEX

  std::cout << "boost regex" << std::endl;

  boost::regex cc_rex(cc_fmt);
  boost::smatch m;
  bool found = boost::regex_match(test_buffer.str(), m, cc_rex);
  boost::smatch::iterator it = m.begin();
#else

  std::cout << "c++11 regex" << std::endl;

  std::regex cc_rex(cc_fmt);
  std::smatch m;
  bool found = std::regex_match(test_buffer.str(), m, cc_rex);
  std::smatch::iterator it = m.begin();
#endif



  if (found) {

    std::cout << "Matches: " << std::endl;

    for (it = m.begin(); it != m.end(); ++it) {
      std::cout << "smatch iter: " << *it << std::endl;
    }


  } else {

    std::cout << "No Match " << std::endl;
  }



}
