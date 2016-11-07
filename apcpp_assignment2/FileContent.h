#pragma once

#include <vector>

#include "Value.h"


class FileContent {
public:
    using Header = std::vector<std::pair<std::string, Value>>;
    using Body = std::vector<char>;


public:
    FileContent() = default;
    FileContent(const Header & header, const Body & body);
    FileContent(const FileContent & content) = default;
    FileContent(FileContent && content) = default;

    FileContent & operator=(const FileContent & rhs) = default;
    FileContent & operator=(FileContent && rhs) = default;


public:
    const Header & header() const;
    void setHeader(const Header & header);
    void setHeader(Header && header);
    void addHeaderItem(const std::string & key, const Value & value);

    const Body & body() const;
    void setBody(const Body & body);
    void setBody(Body && body);

    bool operator==(const FileContent & rhs) const;


private:
    Header m_header;
    Body m_body;
};
