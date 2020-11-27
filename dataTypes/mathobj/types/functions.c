#include "typedefs.h"
#include "../mathobj.h"

math_obj buildMathObjectFunction(math_obj label, math_obj m) {
    //math_obj funcLabel = buildMathObjectVariable(s);

    math_obj_array arr = newMathObjectArray(2);
    arr[0] = label;
    arr[1] = m;

    return __buildMathObjectOperatorLike(arr, FUNCTION);
}
