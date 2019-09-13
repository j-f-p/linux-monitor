#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization.
float Processor::Utilization() {
  enum class tickTypes {user, nice, system, idle, iowait, irq, softirq, steal};
  std::vector<long> tickValues = LinuxParser::aggregateCPUtickData();

  long activetickTypes = tickValues[static_cast<int>(tickTypes::user)]
    + tickValues[static_cast<int>(tickTypes::nice)]
    + tickValues[static_cast<int>(tickTypes::system)]
    + tickValues[static_cast<int>(tickTypes::irq)]
    + tickValues[static_cast<int>(tickTypes::softirq)]
    + tickValues[static_cast<int>(tickTypes::steal)];

  long idletickTypes = tickValues[static_cast<int>(tickTypes::idle)]
    + tickValues[static_cast<int>(tickTypes::iowait)];

  return static_cast<float>(activetickTypes)
    / static_cast<float>(activetickTypes + idletickTypes);
}
