#include <stdexcept>

namespace tiger
{

class LexicalError : public std::runtime_error
{
public:
    LexicalError(const std::string& str)
        : std::runtime_error(str)
    {
    }
};

}