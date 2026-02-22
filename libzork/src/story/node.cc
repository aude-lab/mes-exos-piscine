#include <libzork/story/node.hh>

#include "exceptions.hh"
#include "story/node_impl.hh"

namespace libzork::story
{

    std::unique_ptr<Node> make_node(const std::string& name,
                                    const fs::path& script_path)
    {
        return std::make_unique<NodeImpl>(name, script_path);
    }

} // namespace libzork::story
