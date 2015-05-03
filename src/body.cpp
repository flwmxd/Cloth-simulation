#include "body.h"

Body::Body(Shape * s)
    : shape(s) {

    shape->body = this;
}