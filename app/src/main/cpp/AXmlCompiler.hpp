#ifndef AXMLCOMPILER_HPP
#define AXMLCOMPILER_HPP

#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace axml {

struct StringPool {
    std::vector<std::string> strings;
    std::map<std::string, uint32_t> stringToIndex;

    uint32_t add(const std::string& str);
    void write(std::vector<uint8_t>& out) const;
};

class AXmlCompiler {
public:
    AXmlCompiler() = default;

    // Compiles a simple AndroidManifest.xml string to binary
    std::vector<uint8_t> compile(const std::string& xmlContent);

private:
    StringPool mStringPool;

    void writeHeader(std::vector<uint8_t>& out, uint16_t type, uint32_t size);
    void updateSize(std::vector<uint8_t>& out, uint32_t offset);
};

} // namespace axml

#endif // AXMLCOMPILER_HPP
