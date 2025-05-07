module;

#include <algorithm>
#include <memory>
#include <string_view>
#include <string>


export module jnikot.util.std;

namespace std {
    export using std::copy;
    export using std::copy_n;
    export using std::equal;
    export using std::replace;
    export using std::shared_ptr;
    export using std::make_shared;
    export using std::unique_ptr;
    export using std::make_unique;
    export using std::string_view;
    export using std::string;
}