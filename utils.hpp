#include <string>

namespace utils
{
	template <class T> T to_int(const std::string& s)
	{
  		T base{10};
  		T result{0};
  		
		for (auto c : s)
		{
    			result = base * result + static_cast<T>(c - '0');
  		};
  		
		return result;
	};

};
