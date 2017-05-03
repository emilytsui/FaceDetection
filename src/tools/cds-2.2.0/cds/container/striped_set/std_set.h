/*
    This file is a part of libcds - Concurrent Data Structures library

    (C) Copyright Maxim Khizhinsky (libcds.dev@gmail.com) 2006-2017

    Source code repo: http://github.com/khizmax/libcds/
    Download: http://sourceforge.net/projects/libcds/files/

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CDSLIB_CONTAINER_STRIPED_SET_STD_SET_ADAPTER_H
#define CDSLIB_CONTAINER_STRIPED_SET_STD_SET_ADAPTER_H

#include <cds/container/striped_set/adapter.h>
#include <set>

//@cond
namespace cds { namespace container {
    namespace striped_set {

        // Copy policy for std::set
        template <typename T, typename Traits, typename Alloc>
        struct copy_item_policy< std::set< T, Traits, Alloc > >
        {
            typedef std::set< T, Traits, Alloc > set_type;
            typedef typename set_type::iterator iterator;

            void operator()( set_type& set, iterator itWhat )
            {
                set.insert( *itWhat );
            }
        };

        template <typename T, typename Traits, typename Alloc>
        struct swap_item_policy< std::set< T, Traits, Alloc > >: public copy_item_policy< std::set< T, Traits, Alloc > >
        {};

        // Move policy for std::set
        template <typename T, typename Traits, typename Alloc>
        struct move_item_policy< std::set< T, Traits, Alloc > >
        {
            typedef std::set< T, Traits, Alloc > set_type;
            typedef typename set_type::iterator iterator;

            void operator()( set_type& set, iterator itWhat )
            {
                set.insert( std::move( *itWhat ));
            }
        };
    }   // namespace striped_set
}} // namespace cds::container

namespace cds { namespace intrusive { namespace striped_set {

    /// std::set adapter for hash set bucket
    template <typename T, class Traits, class Alloc, typename... Options>
    class adapt< std::set<T, Traits, Alloc>, Options... >
    {
    public:
        typedef std::set<T, Traits, Alloc>     container_type          ;   ///< underlying container type

    private:
        /// Adapted container type
        class adapted_container: public cds::container::striped_set::adapted_container
        {
        public:
            typedef typename container_type::value_type value_type  ;   ///< value type stored in the container
            typedef typename container_type::iterator      iterator ;   ///< container iterator
            typedef typename container_type::const_iterator const_iterator ;    ///< container const iterator

            static bool const has_find_with = false;
            static bool const has_erase_with = false;

        private:
            //@cond
            typedef typename cds::opt::select<
                typename cds::opt::value<
                    typename cds::opt::find_option<
                        cds::opt::copy_policy< cds::container::striped_set::move_item >
                        , Options...
                    >::type
                >::copy_policy
                , cds::container::striped_set::copy_item, cds::container::striped_set::copy_item_policy<container_type>
                , cds::container::striped_set::swap_item, cds::container::striped_set::swap_item_policy<container_type>
                , cds::container::striped_set::move_item, cds::container::striped_set::move_item_policy<container_type>
            >::type copy_item;
            //@endcond

        private:
            //@cond
            container_type  m_Set;
            //@endcond

        public:

            template <typename Q, typename Func>
            bool insert( const Q& val, Func f )
            {
                std::pair<iterator, bool> res = m_Set.insert( value_type(val));
                if ( res.second )
                    f( const_cast<value_type&>(*res.first));
                return res.second;
            }

            template <typename... Args>
            bool emplace( Args&&... args )
            {
                std::pair<iterator, bool> res = m_Set.emplace( std::forward<Args>(args)... );
                return res.second;
            }

            template <typename Q, typename Func>
            std::pair<bool, bool> update( const Q& val, Func func, bool bAllowInsert )
            {
                if ( bAllowInsert ) {
                    std::pair<iterator, bool> res = m_Set.insert( value_type(val));
                    func( res.second, const_cast<value_type&>(*res.first), val );
                    return std::make_pair( true, res.second );
                }
                else {
                    auto it = m_Set.find(value_type(val));
                    if ( it == m_Set.end())
                        return std::make_pair( false, false );
                    func( false, const_cast<value_type&>(*it), val );
                    return std::make_pair( true, false );
                }
            }

            template <typename Q, typename Func>
            bool erase( const Q& key, Func f )
            {
                iterator it = m_Set.find( value_type(key));
                if ( it == m_Set.end())
                    return false;
                f( const_cast<value_type&>(*it));
                m_Set.erase( it );
                return true;
            }

            template <typename Q, typename Func>
            bool find( Q& val, Func f )
            {
                iterator it = m_Set.find( value_type(val));
                if ( it == m_Set.end())
                    return false;
                f( const_cast<value_type&>(*it), val );
                return true;
            }

            void clear()
            {
                m_Set.clear();
            }

            iterator begin()                { return m_Set.begin(); }
            const_iterator begin() const    { return m_Set.begin(); }
            iterator end()                  { return m_Set.end(); }
            const_iterator end() const      { return m_Set.end(); }

            void move_item( adapted_container& /*from*/, iterator itWhat )
            {
                assert( m_Set.find( *itWhat ) == m_Set.end());
                copy_item()( m_Set, itWhat );
            }

            size_t size() const
            {
                return m_Set.size();
            }
        };

    public:
        typedef adapted_container type ; ///< Result of \p adapt metafunction

    };
}}} // namespace cds::intrusive::striped_set


//@endcond

#endif // #ifndef CDSLIB_CONTAINER_STRIPED_SET_STD_SET_ADAPTER_H
