#pragma once
#include <vector>

namespace sklib::container
{

template <class T, class _Alloc = std::allocator<T>>
class tree
{
public:
    using value_type = T;
    using allocator = _Alloc;
    using allocator_traits = std::allocator_traits<_Alloc>;
    
    struct node
    {
        node *parent = nullptr;
        T *value = nullptr;
        std::vector<node> children;
    };
    
    // tree();
    // tree(const tree &other);
    // tree(tree &&other) noexcept;
    // tree &operator=(const tree &other);
    // tree &operator=(tree &&other) noexcept;

    template<class... Args>
    const value_type &emplace_back(const node &node, Args&&... args)
    {
        node n;
        n.parent = &node;
        n.value = m_alloc.allocate(1);
        allocator_traits::construct(m_alloc, n.value, std::forward<Args>(args)...);
        return *n.value;
    }

    auto push_back(const node &node, const T &val) -> std::enable_if_t<std::is_copy_constructible_v<T>>
    {
        node n;
        n.parent = &node;
        n.value = m_alloc.allocate(1);
        allocator_traits::construct(m_alloc, n.value, val);
    }

    auto push_back(const node &node, T &&val) -> std::enable_if_t<std::is_move_constructible_v<T>>
    {
        node n;
        n.parent = &node;
        n.value = m_alloc.allocate(1);
        allocator_traits::contruct(m_alloc, n.value, std::move(val));
    }

    // TODO return type
    template<class Fn>
    void walk() const
    {
        for(const auto &root : m_root_nodes)
        {
            walk(root);
        }
    }

    template<class Fn>
    void walk(const node &node, Fn func) const
    {
        if(node.children.empty()) func(*node.value);
        else
        {
            for(const auto &child : node.children)
            {
                walk(child, func);
            }
        }
    }

    size_t size(const node &node) const
    {
        auto get_size = [](const node &node, size_t &size)
        {
            for(const auto &child : node.children)
            {
                get_size(child, size);
            }

            size++;
        };

        size_t total_size = 0;
        get_size(node, total_size);
        return total_size;
    }

    size_t size() const
    {
        size_t total_size = 0;
        for(const auto &root : m_root_nodes)
        {
            total_size += size(root);
        }
        return total_size;
    }
private:
    std::vector<node> m_root_nodes;
    allocator m_alloc;
};

}