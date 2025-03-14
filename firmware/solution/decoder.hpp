#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <bits/stdc++.h>
#include <chrono>

#include <unordered_map>
#include <dbcppp/Network.h>
#include <dbcppp/CApi.h>
void decode(uint64_t data, const dbcppp::ISignal* signal, uint64_t messageSize);

uint64_t convertToLittleEndian(uint64_t data, uint64_t size);

void extractDumpLog(struct canframe *head);

short convertHextoBinary (char hex);

void run();