#ifndef NEXTCODE_SEMA_TYPECHECKBITWISE_H
#define NEXTCODE_SEMA_TYPECHECKBITWISE_H

#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/AST/TypeCheckRequests.h"

namespace nextcode {
class ProtocolConformance;
class NominalTypeDecl;

/// Check that \p conformance, a conformance of some nominal type to
/// BitwiseCopyable, is valid.
bool checkBitwiseCopyableConformance(ProtocolConformance *conformance,
                                     bool isImplicit);

/// Produce an implicit conformance of \p nominal to BitwiseCopyable if it is
/// valid to do so.
ProtocolConformance *
deriveImplicitBitwiseCopyableConformance(NominalTypeDecl *nominal);
} // end namespace nextcode

#endif // NEXTCODE_SEMA_TYPECHECKBITWISE_H
