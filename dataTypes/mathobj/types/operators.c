#include "typedefs.h"
#include "../mathobjheader.h"
#include "../mathobj.h"
#include "../../../errors.h"

math_obj buildMathObjectPlus(math_obj_array arr) {
    return __buildMathObjectOperatorLike(arr, PLUS);
}

math_obj buildMathObjectNegate(math_obj m) {
    math_obj_array arr = newMathObjectArray(1);
    arr[0] = m;
    return __buildMathObjectOperatorLike(arr, NEGATE);
}

math_obj buildMathObjectProduct(math_obj_array arr) {
    return __buildMathObjectOperatorLike(arr, PRODUCT);
}

math_obj buildMathObjectTerm(math_obj constant, math_obj var) {
    math_obj_array arr =  buildMathObjectArrayFrom2(constant, var);
    return buildMathObjectProduct(arr);
}

math_obj buildMathObjectFraction(math_obj n, math_obj d) {
    math_obj_array arr = newMathObjectArray(2);
    arr[0] = n;
    arr[1] = d;

    if (math_obj_isConstant(d) && math_obj_mvalue_isEqualToLong(d, 0)) {
        throw_error("Division By Zero", "0");
    }
    // check if d is zero
    return __buildMathObjectOperatorLike(arr, FRACTION);
}

math_obj buildMathObjectEquation(math_obj a, math_obj b) {
    math_obj_array mArr = newMathObjectArray(2);
    mArr[0] = a;
    mArr[1] = b;
    return __buildMathObjectOperatorLike(mArr, EQUATION);
}
