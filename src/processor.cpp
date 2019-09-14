#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::unordered_map;

// Return the aggregate CPU utilization.
float Processor::Utilization() {
  unordered_map<string, long> ticks = LinuxParser::aggregateCPUtickData();

  long activetickTypes = ticks["user"] + ticks["nice"] + ticks["system"]
    + ticks["irq"] + ticks["softirq"] + ticks["steal"];

  long idletickTypes = ticks["idle"] + ticks["iowait"];

  return static_cast<float>(activetickTypes)
    / static_cast<float>(activetickTypes + idletickTypes);
}
