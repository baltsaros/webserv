#include <iostream>
// #include <fstream>
// #include "./inc/Request.hpp"

	void	trimTrailingChar(std::string &str, char trail) {
	size_t	pos = str.find_last_not_of(trail);

	if (pos != std::string::npos)
		str.erase(pos + 1);
	}

// map::count
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  char c;

  mymap ['a'];
  mymap ['c']=202;
  mymap ['f']=303;

  for (c='a'; c<'h'; c++)
  {
    std::cout << c;
    if (mymap.count(c)>0)
      std::cout << " is an element of mymap.\n";
    else 
      std::cout << " is not an element of mymap.\n";
  }

  return 0;
}