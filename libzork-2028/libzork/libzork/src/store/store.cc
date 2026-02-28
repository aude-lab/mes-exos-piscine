#include <libzork/store/store.hh>

#include "exceptions.hh"
#include "store/store_impl.hh"

namespace libzork::store
{

    std::unique_ptr<Store> make_store()
    {
        throw NotImplemented();
    }

} // namespace libzork::store
