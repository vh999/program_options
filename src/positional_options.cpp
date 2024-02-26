// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROGRAM_OPTIONS_SOURCE
# define BOOST_PROGRAM_OPTIONS_SOURCE
#endif
#include <boost/program_options/config.hpp>

#include <boost/program_options/options_description.hpp>

#include <boost/program_options/positional_options.hpp>

#include <boost/limits.hpp>

#include <cassert>

/*
Add a new method for declaring positional options.
This method requires that positional option be declared with the normal
option declarations first, and then setting the positionals count for
that option. Non positionals have the count set to 0 upon initialization.
Then, to change it to a positional, set the count to a number other than
0. Positive integer indicates the number of tokens the positional requires.
A negative 1 (-1) indicate unbounded tokens. This option can only be set
as the last positional.
*/
using namespace boost::program_options;

namespace boost { namespace program_options {

    positional_options_description::positional_options_description()
    {}
    
    positional_options_description::positional_options_description(const options_description &options_desc)
    {
        std::string name;

        for(auto option : options_desc.options())
        {
            if(option->is_positional())
            {
                add(option->canonical_name().c_str(),option->get_positionals());   
            } 
        }
    }

    positional_options_description&
    positional_options_description::add(const char* name, int max_count)
    {
        assert(max_count != -1 || m_trailing.empty());

        if (max_count == -1)
            m_trailing = name;
        else {
            m_names.resize(m_names.size() + max_count, name);
        }
        return *this;
    }

    unsigned
    positional_options_description::max_total_count() const
    {
        return m_trailing.empty() ? 
          static_cast<unsigned>(m_names.size()) : (std::numeric_limits<unsigned>::max)();
    }
    
    const std::string& 
    positional_options_description::name_for_position(unsigned position) const
    {
        assert(position < max_total_count());

        if (position < m_names.size())
            return m_names[position];
        else
            return m_trailing;
    }


}}

