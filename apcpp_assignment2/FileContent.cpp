#include "FileContent.h"


FileContent::FileContent(const Header & header, const Body & body)
: m_header{ header }
, m_body{ body } {
}

const FileContent::Header & FileContent::header() const {
    return m_header;
}

void FileContent::setHeader(const Header & header) {
    m_header = header;
}

void FileContent::setHeader(Header && header) {
    m_header = move(header);
}

void FileContent::addHeaderItem(const std::string & key, const Value & value) {
    m_header.emplace_back(key, value);
}

const FileContent::Body & FileContent::body() const {
    return m_body;
}

void FileContent::setBody(const Body & body) {
    m_body = body;
}

void FileContent::setBody(Body && body) {
    m_body = move(body);
}

bool FileContent::operator==(const FileContent & rhs) const {
    if (this == &rhs) {
        return true;
    }

    if (m_header.size() != rhs.m_header.size()) {
        return false; // add breakpoint here to debug differences
    }

    for (size_t i = 0; i < m_header.size(); ++i) {
        if (m_header[i].first != rhs.m_header[i].first) {
            return false; // add breakpoint here to debug differences
        }
        if (!(m_header[i].second == rhs.m_header[i].second)) {
            return false; // add breakpoint here to debug differences
        }
    }

    return m_body == rhs.m_body;
}
