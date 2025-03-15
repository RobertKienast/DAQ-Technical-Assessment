#include "dbcppp/Network.h"
#include <dbcppp/CApi.h>
//void manualDecode(uint64_t data, const dbcppp::ISignal* signal, uint64_t messageSize);

uint64_t convertToLittleEndian(uint64_t data, uint64_t size);

void extractDumpLog(struct canframe *head, std::string dumplo);

short convertHextoBinary (char hex);

void decode(std::string file1, std::string file2, std::string file3, std::string dumplog);
