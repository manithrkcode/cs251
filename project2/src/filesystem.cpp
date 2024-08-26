#include "filesystem.hpp"

#include <iostream>

using namespace cs251;

filesystem::filesystem(const size_t sizeLimit)
{
    m_fileSystemSizeLimit = sizeLimit;
}

handle filesystem::create_file(const size_t fileSize, const std::string& fileName)
{
    //region checks
    if (!exist(0)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(0).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (fileName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    if (fileSize > (m_fileSystemSizeLimit - m_totalFileSize)) {
        throw exceeds_size();
    }

    if (!m_fileSystemNodes.ref_node(0).peek_children_handles().empty()) {
        for (auto childHandle: m_fileSystemNodes.ref_node(0).peek_children_handles()) {
            if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == fileName) {
                throw file_exists();
            }
        }
    }
    //endregion

    filesystem_node_data newFile;
    newFile.m_type = node_type::File;
    newFile.m_name = fileName;
    newFile.m_fileSize = fileSize;

    m_totalFileSize += fileSize;

    handle newHandle = m_fileSystemNodes.allocate(0);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newFile;
    return newHandle;
}

handle filesystem::create_directory(const std::string& directoryName)
{
    //region checks
    if (!exist(0)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(0).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (directoryName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    if (!m_fileSystemNodes.ref_node(0).peek_children_handles().empty()) {
        for (auto childHandle: m_fileSystemNodes.ref_node(0).peek_children_handles()) {
            if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == directoryName) {
                throw directory_exists();
            }
        }
    }
    //endregion

    filesystem_node_data newDirectory;
    newDirectory.m_type = node_type::Directory;
    newDirectory.m_name = directoryName;

    handle newHandle = m_fileSystemNodes.allocate(0);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newDirectory;
    return newHandle;
}

handle filesystem::create_link(const handle targetHandle, const std::string& linkName)
{
    //region checks
    if (!exist(targetHandle) || !exist(0)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(0).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (linkName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    if (!m_fileSystemNodes.ref_node(0).peek_children_handles().empty()) {
        for (auto childHandle: m_fileSystemNodes.ref_node(0).peek_children_handles()) {
            if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == linkName) {
                throw link_exists();
            }
        }
    }
    //endregion

    filesystem_node_data newLink;
    newLink.m_type = node_type::Link;
    newLink.m_name = linkName;
    newLink.m_linkedHandle = targetHandle;

    handle newHandle = m_fileSystemNodes.allocate(0);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newLink;
    return newHandle;
}

bool filesystem::remove(const handle targetHandle)
{
    //checks
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    tree_node<filesystem_node_data> &node = m_fileSystemNodes.ref_node(targetHandle);
    if (node.ref_data().m_type == node_type::Directory) {
        if (node.peek_children_handles().empty()) {
            m_fileSystemNodes.remove(targetHandle);
            return true;
        }
    }
    else if (node.ref_data().m_type == node_type::Link) {
        node.ref_data().m_linkedHandle = -1;
        m_fileSystemNodes.remove(targetHandle);
        return true;
    }
    else if (node.ref_data().m_type == node_type::File) {
        m_totalFileSize -= node.ref_data().m_fileSize;
        m_fileSystemNodes.remove(targetHandle);
        return true;
    }
    return false;
}

handle filesystem::create_file(const size_t fileSize, const std::string& fileName, const handle parentHandle)
{
    //region checks
    if (!exist(parentHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (fileName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    if ((m_totalFileSize + fileSize) > m_fileSystemSizeLimit) {
        throw exceeds_size();
    }

    for (auto childHandle: m_fileSystemNodes.ref_node(parentHandle).peek_children_handles()) {
        if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == fileName) {
            throw file_exists();
        }
    }
    //endregion

    filesystem_node_data newFile;
    newFile.m_type = node_type::File;
    newFile.m_name = fileName;
    newFile.m_fileSize = fileSize;

    m_totalFileSize += fileSize;

    handle newHandle = m_fileSystemNodes.allocate(parentHandle);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newFile;
    return newHandle;
}

handle filesystem::create_directory(const std::string& directoryName, const handle parentHandle)
{
    //region checks
    if (!exist(parentHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (directoryName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    for (auto childHandle: m_fileSystemNodes.ref_node(parentHandle).peek_children_handles()) {
        if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == directoryName) {
            throw directory_exists();
        }
    }
    //endregion

    filesystem_node_data newDirectory;
    newDirectory.m_type = node_type::Directory;
    newDirectory.m_name = directoryName;

    handle newHandle = m_fileSystemNodes.allocate(parentHandle);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newDirectory;
    return newHandle;
}

handle filesystem::create_link(const handle targetHandle, const std::string& linkName, const handle parentHandle)
{
    //region checks
    if (!exist(targetHandle) || !exist(parentHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(parentHandle).ref_data().m_type != node_type::Directory) {
        throw invalid_handle();
    }

    if (linkName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    for (auto childHandle: m_fileSystemNodes.ref_node(parentHandle).peek_children_handles()) {
        if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == linkName) {
            throw link_exists();
        }
    }
    //endregion

    filesystem_node_data newLink;
    newLink.m_type = node_type::Link;
    newLink.m_name = linkName;
    newLink.m_linkedHandle = targetHandle;

    handle newHandle = m_fileSystemNodes.allocate(parentHandle);
    m_fileSystemNodes.ref_node(newHandle).ref_data() = newLink;
    return newHandle;
}

std::string filesystem::get_absolute_path(const handle targetHandle)
{
    //checks
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    tree_node<filesystem_node_data> node = m_fileSystemNodes.ref_node(targetHandle);
    std::string path = node.ref_data().m_name;

    while (node.get_parent_handle() != 0) {
        node = m_fileSystemNodes.ref_node(node.get_parent_handle());
        if (node.ref_data().m_type == node_type::Directory) {
            path = node.ref_data().m_name + '/' + path;
        }
    }
    path = '/' + path;
    return path;
}

std::string filesystem::get_name(const handle targetHandle)
{
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }
    return m_fileSystemNodes.ref_node(targetHandle).ref_data().m_name;
}

void filesystem::rename(const handle targetHandle, const std::string& newName)
{
    //checks
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }
    if (newName.find("/") != std::string::npos) {
        throw invalid_name();
    }

    handle parentHandle = m_fileSystemNodes.ref_node(targetHandle).get_parent_handle();
    if (!m_fileSystemNodes.ref_node(parentHandle).peek_children_handles().empty()) {
        for (auto childHandle: m_fileSystemNodes.ref_node(parentHandle).peek_children_handles()) {
            if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == newName) {
                throw name_exists();
            }
        }
    }

    m_fileSystemNodes.ref_node(targetHandle).ref_data().m_name = newName;
}

bool filesystem::exist(const handle targetHandle)
{
    if (targetHandle >= 0 && targetHandle < m_fileSystemNodes.peek_nodes().size()) {
        return !m_fileSystemNodes.ref_node(targetHandle).is_recycled();
    }
    return false;
}

handle filesystem::get_handle(const std::string& absolutePath)
{
    std::queue<std::string> paths;
    std::stringstream ss(absolutePath);
    std::string p;

    ss.get();
    while (std::getline(ss, p, '/')) {
        paths.push(p);
    }

    handle handle = 0;
    while (!paths.empty()) {
        bool found = false;
        if (m_fileSystemNodes.ref_node(handle).ref_data().m_type == node_type::Link) {
            handle = m_fileSystemNodes.ref_node(handle).ref_data().m_linkedHandle;
            found = true;
        }
        for (auto childHandle: m_fileSystemNodes.ref_node(handle).peek_children_handles()) {
            if (m_fileSystemNodes.ref_node(childHandle).ref_data().m_name == paths.front()) {
                found = true;
                handle = childHandle;
                paths.pop();
                break;
            }
        }
        if (!found) {
            throw invalid_path();
        }
    }
    return handle;
}

handle filesystem::follow(const handle targetHandle)
{
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    handle linkedHandle = m_fileSystemNodes.ref_node(targetHandle).ref_data().m_linkedHandle;
    if (m_fileSystemNodes.ref_node(linkedHandle).ref_data().m_type == node_type::Link) {
        return follow(linkedHandle);
    }
    return linkedHandle;
}

handle filesystem::get_largest_file_handle() const
{
    file_size_max_heap fH;
    for (auto n: m_fileSystemNodes.peek_nodes()) {
        if (n.ref_data().m_type == node_type::File) {
            fH.push((n.ref_data().m_fileSize), n.get_handle());
        }
    }
    return fH.top();
}

size_t filesystem::get_available_size() const
{
    return m_fileSystemSizeLimit - m_totalFileSize;
}

size_t filesystem::get_file_size(const handle targetHandle)
{
    //checks
    if (!exist(targetHandle)) {
        throw invalid_handle();
    }

    if (m_fileSystemNodes.ref_node(targetHandle).ref_data().m_type != node_type::File) {
        if (m_fileSystemNodes.ref_node(follow(targetHandle)).ref_data().m_type != node_type::File) {
            throw invalid_handle();
        }
        else {
            return m_fileSystemNodes.ref_node(follow(targetHandle)).ref_data().m_fileSize;
        }
    }

    return m_fileSystemNodes.ref_node(targetHandle).ref_data().m_fileSize;
}

size_t filesystem::get_file_size(const std::string& absolutePath)
{
    return get_file_size(get_handle(absolutePath));
}

std::string filesystem::print_layout()
{
    std::stringstream ss{};
    const auto& node = m_fileSystemNodes.ref_node(0);
    for (const auto& childHandle : node.peek_children_handles()) {
        print_traverse(0, ss, childHandle);
    }
    return ss.str();
}

void filesystem::print_traverse(const size_t level, std::stringstream& ss, const handle targetHandle)
{
    auto& node = m_fileSystemNodes.ref_node(targetHandle);
    std::stringstream indentation{};
    for (auto i = level; i > 0; i--)
    {
        indentation << "\t";
    }
    std::string type{};
    switch (node.ref_data().m_type)
    {
        case node_type::Directory: type = "[D]"; break;
        case node_type::Link: type = "[L]"; break;
        case node_type::File: type = "[F]"; break;
    }
    ss << indentation.str() << type << node.ref_data().m_name;
    if (node.ref_data().m_type == node_type::Link)
    {
        try {
            const auto path = get_absolute_path(follow(node.get_handle()));
            ss << " [->" << path << "]";
        }
        catch (const std::exception& e)
        {
            ss << " [invalid]";
        }
    }
    else if (node.ref_data().m_type == node_type::File)
    {
        ss << " (size = " << std::to_string(node.ref_data().m_fileSize) << ")";
    }
    ss << std::endl;
    for (const auto& childHandle : node.peek_children_handles())
    {
        print_traverse(level + 1, ss, childHandle);
    }
}
