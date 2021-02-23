#pragma once

#include <array>
#include <iostream>

// The length is the length of a SHA256 digest.
// That 32 is the correct value will be verified in hash.cc
class Hash : public std::array<unsigned char, 32> {
  public:
  /*
   * Output a hash
   */
  friend std::ostream &operator<<(std::ostream &stream, const Hash &hash);
};

