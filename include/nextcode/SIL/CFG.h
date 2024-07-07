//===--- CFG.h - SIL control-flow graph utilities ---------------*- C++ -*-===//
//
/*
 * Copyright (c) 2024, NeXTech Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with NeXTech, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// This file provides basic declarations and utilities for working with
// SIL basic blocks as a control-flow graph.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_CFG_H
#define NEXTCODE_SIL_CFG_H

#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILValue.h"
#include "llvm/ADT/GraphTraits.h"

#if defined(__has_include)
#if __has_include("llvm/Support/CfgTraits.h")
#include "llvm/Support/CfgTraits.h"
#define NEXTCODE_LLVM_HAS_CFGTRAITS_H
#endif
#endif

namespace llvm {

//===----------------------------------------------------------------------===//
// GraphTraits for SILBasicBlock
//===----------------------------------------------------------------------===//

template <> struct GraphTraits<nextcode::SILBasicBlock *> {
  using ChildIteratorType = nextcode::SILBasicBlock::succblock_iterator;
  using Node = nextcode::SILBasicBlock;
  using NodeRef = Node *;

  static NodeRef getEntryNode(NodeRef BB) { return BB; }

  static ChildIteratorType child_begin(NodeRef N) {
    return N->succblock_begin();
  }
  static ChildIteratorType child_end(NodeRef N) { return N->succblock_end(); }
};

template <> struct GraphTraits<const nextcode::SILBasicBlock*> {
  using ChildIteratorType = nextcode::SILBasicBlock::const_succblock_iterator;
  using Node = const nextcode::SILBasicBlock;
  using NodeRef = Node *;

  static NodeRef getEntryNode(NodeRef BB) { return BB; }

  static ChildIteratorType child_begin(NodeRef N) {
    return N->succblock_begin();
  }
  static ChildIteratorType child_end(NodeRef N) { return N->succblock_end(); }
};

template <> struct GraphTraits<Inverse<nextcode::SILBasicBlock*> > {
  using ChildIteratorType = nextcode::SILBasicBlock::pred_iterator;
  using Node = nextcode::SILBasicBlock;
  using NodeRef = Node *;
  static NodeRef getEntryNode(Inverse<nextcode::SILBasicBlock *> G) {
    return G.Graph;
  }
  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->pred_begin();
  }
  static inline ChildIteratorType child_end(NodeRef N) {
    return N->pred_end();
  }
};

template <> struct GraphTraits<Inverse<const nextcode::SILBasicBlock*> > {
  using ChildIteratorType = nextcode::SILBasicBlock::pred_iterator;
  using Node = const nextcode::SILBasicBlock;
  using NodeRef = Node *;

  static NodeRef getEntryNode(Inverse<const nextcode::SILBasicBlock *> G) {
    return G.Graph;
  }
  static inline ChildIteratorType child_begin(NodeRef N) {
    return N->pred_begin();
  }
  static inline ChildIteratorType child_end(NodeRef N) {
    return N->pred_end();
  }
};

template <>
struct GraphTraits<nextcode::SILFunction *>
    : public GraphTraits<nextcode::SILBasicBlock *> {
  using GraphType = nextcode::SILFunction *;
  using NodeRef = nextcode::SILBasicBlock *;

  static NodeRef getEntryNode(GraphType F) { return &F->front(); }

  using nodes_iterator = pointer_iterator<nextcode::SILFunction::iterator>;
  static nodes_iterator nodes_begin(GraphType F) {
    return nodes_iterator(F->begin());
  }
  static nodes_iterator nodes_end(GraphType F) {
    return nodes_iterator(F->end());
  }
  static unsigned size(GraphType F) { return F->size(); }
};

template <> struct GraphTraits<Inverse<nextcode::SILFunction*> >
    : public GraphTraits<Inverse<nextcode::SILBasicBlock*> > {
  using GraphType = Inverse<nextcode::SILFunction *>;
  using NodeRef = NodeRef;

  static NodeRef getEntryNode(GraphType F) { return &F.Graph->front(); }

  using nodes_iterator = pointer_iterator<nextcode::SILFunction::iterator>;
  static nodes_iterator nodes_begin(GraphType F) {
    return nodes_iterator(F.Graph->begin());
  }
  static nodes_iterator nodes_end(GraphType F) {
    return nodes_iterator(F.Graph->end());
  }
  static unsigned size(GraphType F) { return F.Graph->size(); }
};

#ifdef NEXTCODE_LLVM_HAS_CFGTRAITS_H

class SILCfgTraitsBase : public CfgTraitsBase {
public:
  using ParentType = nextcode::SILFunction;
  using BlockRef = nextcode::SILBasicBlock *;
  using ValueRef = nextcode::SILInstruction *;

  static CfgBlockRef wrapRef(BlockRef block) {
    return makeOpaque<CfgBlockRefTag>(block);
  }
  static CfgValueRef wrapRef(ValueRef block) {
    return makeOpaque<CfgValueRefTag>(block);
  }
  static BlockRef unwrapRef(CfgBlockRef block) {
    return static_cast<BlockRef>(getOpaque(block));
  }
  static ValueRef unwrapRef(CfgValueRef block) {
    return static_cast<ValueRef>(getOpaque(block));
  }
};
/// \brief CFG traits for SIL IR.
class SILCfgTraits : public CfgTraits<SILCfgTraitsBase, SILCfgTraits> {
public:
  explicit SILCfgTraits(nextcode::SILFunction * /*parent*/) {}

  static nextcode::SILFunction *getBlockParent(nextcode::SILBasicBlock *block) {
    return block->getParent();
  }

  static auto predecessors(nextcode::SILBasicBlock *block) {
    return block->getPredecessorBlocks();
  }
  static auto successors(nextcode::SILBasicBlock *block) {
    return block->getSuccessors();
  }

  /// Get the defining block of a value if it is an instruction, or null
  /// otherwise.
  static BlockRef getValueDefBlock(ValueRef value) {
    if (auto *instruction = dyn_cast<nextcode::SILInstruction>(value))
      return instruction->getParent();
    return nullptr;
  }

  struct block_iterator
      : iterator_adaptor_base<block_iterator, nextcode::SILFunction::iterator> {
    using Base = iterator_adaptor_base<block_iterator, nextcode::SILFunction::iterator>;

    block_iterator() = default;

    explicit block_iterator(nextcode::SILFunction::iterator i) : Base(i) {}

    nextcode::SILBasicBlock *operator*() const { return &Base::operator*(); }
  };

  static iterator_range<block_iterator> blocks(nextcode::SILFunction *function) {
    return {block_iterator(function->begin()), block_iterator(function->end())};
  }

  struct value_iterator
      : iterator_adaptor_base<value_iterator, nextcode::SILBasicBlock::iterator> {
    using Base = iterator_adaptor_base<value_iterator, nextcode::SILBasicBlock::iterator>;

    value_iterator() = default;

    explicit value_iterator(nextcode::SILBasicBlock::iterator i) : Base(i) {}

    ValueRef operator*() const { return &Base::operator*(); }
  };

  static iterator_range<value_iterator> blockdefs(BlockRef block) {
    return {value_iterator(block->begin()), value_iterator(block->end())};
  }
  struct Printer {
    explicit Printer(const SILCfgTraits &) {}
    ~Printer(){}

    void printBlockName(raw_ostream &out, BlockRef block) const {
      block->printAsOperand(out);
    }
    void printValue(raw_ostream &out, ValueRef value) const {
      value->print(out);
    }
  };
};

template <> struct CfgTraitsFor<nextcode::SILBasicBlock> {
  using CfgTraits = SILCfgTraits;
};

#endif

} // end llvm namespace

#endif

