#include "AXmlCompiler.hpp"
#include <cstring>
#include <algorithm>

namespace axml {

// Constants for Android Binary XML
enum {
    RES_NULL_TYPE               = 0x0000,
    RES_STRING_POOL_TYPE        = 0x0001,
    RES_TABLE_TYPE              = 0x0002,
    RES_XML_TYPE                = 0x0003,

    // Chunk types in RES_XML_TYPE
    RES_XML_FIRST_CHUNK_TYPE    = 0x0100,
    RES_XML_START_NAMESPACE_TYPE= 0x0100,
    RES_XML_END_NAMESPACE_TYPE  = 0x0101,
    RES_XML_START_ELEMENT_TYPE  = 0x0102,
    RES_XML_END_ELEMENT_TYPE    = 0x0103,
    RES_XML_CDATA_TYPE          = 0x0104,
    RES_XML_LAST_CHUNK_TYPE     = 0x017f,
    RES_XML_RESOURCE_MAP_TYPE   = 0x0180,
};

uint32_t StringPool::add(const std::string& str) {
    auto it = stringToIndex.find(str);
    if (it != stringToIndex.end()) {
        return it->second;
    }
    uint32_t index = strings.size();
    strings.push_back(str);
    stringToIndex[str] = index;
    return index;
}

void StringPool::write(std::vector<uint8_t>& out) const {
    uint32_t start = out.size();

    // Header
    out.push_back(RES_STRING_POOL_TYPE & 0xFF);
    out.push_back((RES_STRING_POOL_TYPE >> 8) & 0xFF);
    out.push_back(28 & 0xFF); // Header size
    out.push_back(0);

    // Placeholder for total size
    out.resize(out.size() + 4, 0);

    uint32_t stringCount = strings.size();
    out.push_back(stringCount & 0xFF);
    out.push_back((stringCount >> 8) & 0xFF);
    out.push_back((stringCount >> 16) & 0xFF);
    out.push_back((stringCount >> 24) & 0xFF);

    out.push_back(0); // Style count
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);

    out.push_back(1 << 8); // Flags (UTF-8)
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);

    // String index offset
    uint32_t stringsStart = 28 + stringCount * 4;
    out.push_back(stringsStart & 0xFF);
    out.push_back((stringsStart >> 8) & 0xFF);
    out.push_back((stringsStart >> 16) & 0xFF);
    out.push_back((stringsStart >> 24) & 0xFF);

    out.push_back(0); // Styles offset
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);

    // String offsets
    uint32_t currentOffset = 0;
    std::vector<uint8_t> data;
    for (const auto& s : strings) {
        out.push_back(currentOffset & 0xFF);
        out.push_back((currentOffset >> 8) & 0xFF);
        out.push_back((currentOffset >> 16) & 0xFF);
        out.push_back((currentOffset >> 24) & 0xFF);

        size_t len = s.length();
        // UTF-8 encoding in StringPool is tricky (length twice)
        data.push_back(len & 0xFF);
        data.push_back(len & 0xFF);
        for (char c : s) data.push_back(c);
        data.push_back(0);
        currentOffset = data.size();
    }

    // Align data
    while (data.size() % 4 != 0) data.push_back(0);

    out.insert(out.end(), data.begin(), data.end());

    uint32_t totalSize = out.size() - start;
    out[start + 4] = totalSize & 0xFF;
    out[start + 5] = (totalSize >> 8) & 0xFF;
    out[start + 6] = (totalSize >> 16) & 0xFF;
    out[start + 7] = (totalSize >> 24) & 0xFF;
}

void AXmlCompiler::writeHeader(std::vector<uint8_t>& out, uint16_t type, uint32_t size) {
    out.push_back(type & 0xFF);
    out.push_back((type >> 8) & 0xFF);
    out.push_back(size & 0xFF);
    out.push_back((size >> 8) & 0xFF);
}

void AXmlCompiler::updateSize(std::vector<uint8_t>& out, uint32_t offset) {
    uint32_t size = out.size() - offset;
    out[offset + 4] = size & 0xFF;
    out[offset + 5] = (size >> 8) & 0xFF;
    out[offset + 6] = (size >> 16) & 0xFF;
    out[offset + 7] = (size >> 24) & 0xFF;
}

std::vector<uint8_t> AXmlCompiler::compile(const std::string& xmlContent) {
    std::vector<uint8_t> out;

    // File header
    writeHeader(out, RES_XML_TYPE, 8);
    out.resize(out.size() + 4, 0); // Total size placeholder

    // For this simple implementation, we'll just add some mandatory strings
    // In a real IDE, you'd parse the XML and add all tags/attributes/namespaces
    mStringPool.add("http://schemas.android.com/apk/res/android");
    mStringPool.add("manifest");
    mStringPool.add("package");
    mStringPool.add("com.example.app");

    mStringPool.write(out);

    // Resource Map (Empty for now)
    uint32_t resMapStart = out.size();
    writeHeader(out, RES_XML_RESOURCE_MAP_TYPE, 8);
    out.push_back(8); out.push_back(0); out.push_back(0); out.push_back(0); // Size

    // Placeholder for actual XML chunks (StartElement, EndElement, etc.)
    // This is where the complex parsing and chunk writing would go.

    updateSize(out, 0);
    return out;
}

} // namespace axml
