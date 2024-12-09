#include "visitor.h"
#include "bear.h"
#include "vup.h"
#include "vykhuhol.h"

bool AttackerVisitor::visit(Bear &bear) {
    return false;
}

bool AttackerVisitor::visit(Vup &vup) {
    return true;
}

bool AttackerVisitor::visit(Vykhuhol &vykhuhol) {
    return true;
}
