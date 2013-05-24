#include "config.h"
#include <iomanip>
#include <iostream>
#include <map>

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Type.h"

#include "WorkItem.h"

using namespace std;

WorkItem::WorkItem(size_t gid_x, size_t gid_y, size_t gid_z)
{
  m_globalID[0] = gid_x;
  m_globalID[1] = gid_y;
  m_globalID[2] = gid_z;
}

void WorkItem::dumpPrivateMemory() const
{
  cout << "Work-item ("
       << m_globalID[0] << ","
       << m_globalID[1] << ","
       << m_globalID[2] << "):" << endl;

  PrivateMemory::const_iterator pmitr;
  for (pmitr = m_privateMemory.begin(); pmitr != m_privateMemory.end(); pmitr++)
  {
    // Output symbolic name if available
    cout << setw(16) << setfill(' ');
    if (pmitr->first->hasName())
    {
      cout << pmitr->first->getName().str() << ":";
    }
    else
    {
      cout << pmitr->first << ":";
    }

    // TODO: Interpret type?
    // TODO: Deal with larger private variables (e.g. arrays)
    for (int i = 0; i < pmitr->second.size; i++)
    {
      cout << " " << hex << uppercase << setw(2) << setfill('0')
           << (int)pmitr->second.data[i];
    }
    cout << setw(0) << endl;
  }
}

void WorkItem::execute(const llvm::Instruction& instruction)
{
  // Prepare private variable for instruction result
  size_t resultSize = instruction.getType()->getPrimitiveSizeInBits() >> 3;
  // TODO: Is this necessary for GEP?
  if (instruction.getOpcode() == llvm::Instruction::GetElementPtr)
  {
    // TODO: Configurable pointer size
    resultSize = 4;
  }

  // TODO: Only allocate if not in map already?
  PrivateVariable result = {resultSize, new unsigned char[resultSize]};

  // Temporary: Dump instruction sequence (TODO: remove)
  if (resultSize > 0)
  {
    cout << "%" << (&instruction) << "(" << resultSize << ") = ";
  }
  cout << left << setw(14) << instruction.getOpcodeName();
  llvm::User::const_op_iterator opitr;
  for (opitr = instruction.op_begin(); opitr != instruction.op_end(); opitr++)
  {
    cout << " %" << opitr->get();
  }
  cout << right << endl;

  // Execute instruction
  switch (instruction.getOpcode())
  {
  case llvm::Instruction::Call:
    break;
  case llvm::Instruction::GetElementPtr:
    break;
  case llvm::Instruction::Load:
    break;
  case llvm::Instruction::Store:
    break;
  case llvm::Instruction::FAdd:
    break;
  case llvm::Instruction::Ret:
    break;
  default:
    cout << "Unhandled instruction: " << instruction.getOpcodeName() << endl;
    break;
  }

  // Store result
  if (resultSize > 0)
  {
    m_privateMemory[&instruction] = result;
  }
}