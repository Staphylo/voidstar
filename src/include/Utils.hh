#pragma once

#include <sstream>

#define fmt(Body) ({ \
    std::stringstream os; os << Body; os.str(); \
})
