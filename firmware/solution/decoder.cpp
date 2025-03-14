#include "decoder.hpp"

struct canframe {
    struct timeval timestamp;
    std::string interface; // with 5 useless
    uint32_t can_id;
    uint8_t data[8]; // none useless
    struct canframe *next; // linked list
    bool hasData;
    uint8_t numBits;
};




using canid_t = uint32_t;
struct can_frame
{
	canid_t    can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	uint8_t    __pad;   /* padding */
	uint8_t    __res0;  /* reserved / padding */
	uint8_t    __res1;  /* reserved / padding */
	uint8_t    data[8];
};




short convertHextoBinary (char hex){
    return (hex >= 'A') ? (hex - 'A' + 10) : (hex - '0');
}




void run(){
    std::cout << "Hello World!" << std::endl;
    struct canframe *headcanframe = new canframe;
    extractDumpLog(headcanframe);
    std::shared_ptr<dbcppp::INetwork> net[3];
    std::unordered_map<uint64_t, const dbcppp::IMessage *> messageMap[3];
    std::string files[3] = {"dbc-files/ControlBus.dbc", "dbc-files/SensorBus.dbc", "dbc-files/TractiveBus.dbc"};
    for (int i = 0; i < 3; i++) {
        {
            std::ifstream idbc(files[i]);
            if (!idbc.is_open()) {
                std::cout << "Error could not open dbc file\n";
            }
            net[i] = dbcppp::INetwork::LoadDBCFromIs(idbc);
        }
        std::unordered_map<uint64_t, const dbcppp::IMessage *> messagesMap;
        for (const dbcppp::IMessage& message : net[i]->Messages()) {
            messagesMap.insert(std::make_pair(message.Id(), &message));
        }
        messageMap[i] = messagesMap;
    }
    struct canframe *current = headcanframe;
    struct can_frame currentFrame;
    while (current != NULL) {
        currentFrame.can_id = current->can_id;
        currentFrame.can_dlc = current->numBits;
        for (int i = 0; i < 8; i ++) {
            currentFrame.data[i] = current->data[i];
        }
        if (current->interface == "can0") {
            auto message = messageMap[0].find(current->can_id);
            if (message != messageMap[0].end()) {
                const dbcppp::IMessage* msg = message->second;
                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    const dbcppp::ISignal* mux_sig = msg->MuxSignal();
                    if (signal.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue ||
                        (mux_sig && mux_sig->Decode(currentFrame.data) == signal.MultiplexerSwitchValue()))
                    {
                        std::cout << signal.RawToPhys(signal.Decode(currentFrame.data)) << "\n";
                    }
                }
            }
        } else if (current->interface == "can1") {
            auto message = messageMap[1].find(current->can_id);
            if (message != messageMap[1].end()) {
                const dbcppp::IMessage* msg = message->second;
                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    const dbcppp::ISignal* mux_sig = msg->MuxSignal();
                    if (signal.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue ||
                        (mux_sig && mux_sig->Decode(currentFrame.data) == signal.MultiplexerSwitchValue()))
                    {
                        std::cout << signal.RawToPhys(signal.Decode(currentFrame.data)) << "\n";
                    }
                }
            }
        } else if (current->interface == "can2") {
            auto message = messageMap[2].find(current->can_id);
            if (message != messageMap[2].end()) {
                const dbcppp::IMessage* msg = message->second;
                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    const dbcppp::ISignal* mux_sig = msg->MuxSignal();
                    if (signal.MultiplexerIndicator() != dbcppp::ISignal::EMultiplexer::MuxValue ||
                        (mux_sig && mux_sig->Decode(currentFrame.data) == signal.MultiplexerSwitchValue())) {
                        std::cout << signal.RawToPhys(signal.Decode(currentFrame.data)) << "\n";
                    }
                }
            }
        } else {
            std::cout << "Somthing went wrong\n";
        }
        current = current->next;
        
    }
}

/*  
void manualParse(){
    while (current != NULL) {
        if (current->interface == "can0") {
            auto message = messageMap[0].find(current->can_id);
            if (message != messageMap[0].end()) {
                const dbcppp::IMessage* msg = message->second;
                uint64_t signalSize = 0;
                for (const auto& signal: msg->Signals()) {
                    signalSize += signal.BitSize();
                }

                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    decode(current->data, &signal, current->numBits);
                }
                
            }
        } else if (current->interface == "can1") {
            auto message = messageMap[1].find(current->can_id);
            if (message != messageMap[1].end()) {
                const dbcppp::IMessage* msg = message->second;
                uint64_t signalSize = 0;
                for (const auto& signal: msg->Signals()) {
                    signalSize += signal.BitSize();
                }

                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    decode(current->data, &signal, current->numBits);
                }
                
            }   
        } else if (current->interface == "can2") {
            auto message = messageMap[2].find(current->can_id);
            if (message != messageMap[2].end()) {
                const dbcppp::IMessage* msg = message->second;
                uint64_t signalSize = 0;
                for (const auto& signal: msg->Signals()) {
                    signalSize += signal.BitSize();
                }

                for (const auto& signal: msg->Signals()) {
                    std::cout << '(' << current->timestamp.tv_sec << '.' << current->timestamp.tv_usec << "): " << signal.Name() << ": ";
                    decode(current->data, &signal, current->numBits);
                }
                
            }  
        } else {
            std::cout << "Somthing went wrong\n";
        }
        current = current->next;
        
    }
}
*/

inline void decode(uint64_t data, const dbcppp::ISignal* signal, uint64_t messageSize) {
    uint64_t start = signal->StartBit();
    uint64_t size = signal->BitSize();
    dbcppp::ISignal::EByteOrder endianess = signal->ByteOrder();
    uint16_t littleEndian = 0;
    dbcppp::ISignal::EValueType type = signal->ValueType();
    double offset = signal->Offset();
    bool signedVal = 0;
    double factor = signal->Factor();
    if (endianess  == dbcppp::ISignal::EByteOrder::LittleEndian) {
        littleEndian = 1;
    }
    if (type == dbcppp::ISignal::EValueType::Signed) {
        signedVal = 1;
    }

    uint64_t valueU = 0;
    uint64_t dataBytes = 0;
    int64_t valueS = 0;
    uint64_t mask = (1 << size) - 1;
    if (littleEndian ) {
        dataBytes = (data >> (messageSize - size - start)) & mask; 
    } else {
        dataBytes = (data << (start));
    }
   
    std::cout << "data " << data << " masked " << dataBytes << '\n';

    //if (littleEndian) {
    valueS = convertToLittleEndian(dataBytes, size);
    valueU = valueS;
    //} else {
    //    valueS = dataBytes;
    //    valueU = dataBytes;
    //}
    if (valueS >= (1 << (size - 1))) {
        valueS -= (1 << size);
    }
    
    if (signedVal) {
        std::cout << valueS * factor + offset << '\n';
    } else {
        std::cout << valueU * factor + offset << '\n';
    }
}

inline uint64_t convertToLittleEndian(uint64_t data, uint64_t size) {
    uint64_t newData = 0;
    for (int i = 0; i < size; i += 8) {
        uint64_t maskedData = (data & (0b11111111 << i));
        newData = (newData << 8) + (maskedData >> i);
    }
    return newData;
}


inline void extractDumpLog(struct canframe *head) {
    std::ifstream dumpLog("dump.log");
    int currentByte;
    struct canframe *current = head;
    std::string line;
    std::string timeStampString;
    int idLength = 0;
    int startOfData = 0;
    std::string dataBytes;
    while(std::getline(dumpLog, line)) {
        int secondbrackedPos = 0;
        while(line[secondbrackedPos] != ')') {
            secondbrackedPos++;
        }
        timeStampString = line.substr(1, secondbrackedPos - 1);
        double rawTime = std::stod(timeStampString);
        current->timestamp.tv_sec = int(rawTime);
        current->timestamp.tv_usec = (rawTime - int(rawTime)) * 1000000;

        startOfData = line.find("#");
        int endOfInterface = 0;
        while(line[secondbrackedPos + 2 + endOfInterface] != ' ') {
            endOfInterface++;
        }
        current->interface = line.substr(secondbrackedPos + 2, endOfInterface);

        idLength = startOfData - (secondbrackedPos + 2 + endOfInterface + 1);
        current->can_id = stoul(line.substr(secondbrackedPos + 2 + endOfInterface + 1, idLength), nullptr, 16);
        dataBytes = line.substr(startOfData + 1);
        if (dataBytes[0] != '\0') {
            for (int i = 0; i < dataBytes.length(); i += 2) {
                current->data[i/2] = (convertHextoBinary(dataBytes[i]) << 4) + (convertHextoBinary(dataBytes[i+1]));
            }
            current->hasData = 1;
            current->numBits = line.substr(startOfData + 1).size() * 4;
        } else {
            current->hasData = 0;
            current->numBits = 0;
        }
        current->next = new canframe;
        current = current->next;
    }
    
    dumpLog.close();
}