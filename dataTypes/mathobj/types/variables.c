#include "typedefs.h"
#include "../mathobjheader.h"
#include "../mathobj.h"
#include "../../../errors.h"

math_obj buildMathObjectVariable(String * label) {
    return __buildMathObjectVarLike(label, VARIABLE);
}