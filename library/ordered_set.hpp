//
// Created by zjsdu on 6/9/2021.
//

#ifndef JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
#define JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
#include <ext/pb_ds/assoc_container.hpp>
template<typename T>
using ordered_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;
#endif// JHELPER_EXAMPLE_PROJECT_LIBRARY_ORDERED_SET_HPP_
