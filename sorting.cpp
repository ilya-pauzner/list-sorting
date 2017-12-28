#include<iostream>

using std::cin;
using std::cout;

template<typename T>
class List {
private:
    class Node {
    public:
        T value;
        Node *right;

        explicit Node(const T &v = T()) {
            value = v;
            right = nullptr;
        }

        bool operator==(const Node &other) const {
            return value == other.value && right == other.right;
        }
    };

    // [first; last)
    Node *first;
    Node *last;
    size_t size;

public:
    class Iterator {
    private:
        Node *cur;

    public:
        typedef List<T> list_type;

        explicit Iterator(Node *curr)
                : cur(curr) {}

        T &operator*() {
            return cur->value;
        }

        Iterator &operator++() {
            cur = cur->right;
            return *this;
        }

        bool operator==(const Iterator &other) const {
            return (*cur) == *(other.cur);
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }
    };

    explicit List(size_t list_size = 0) {
        first = new Node;
        last = first;
        size = list_size;
        while (list_size) {
            push_front(0);
            --list_size;
        }
    }

    ~List() {
        Node *cur = first;
        while (cur != last) {
            auto tmp = cur;
            cur = cur->right;
            delete tmp;
        }
        delete last;
    }

    void push_front(const T &value) {
        auto *new_first = new Node(value);
        new_first->right = first;
        first = new_first;
        ++size;
    }

    void pop_front() {
        Node *old_first = first;
        first = first->right;
        delete old_first;
        --size;
    }

    Iterator begin() {
        return Iterator(first);
    }

    Iterator end() {
        return Iterator(last);
    }

    size_t get_size() const {
        return size;
    }
};

template<typename Iter>
size_t distance(Iter first, Iter last) {
    size_t ans = 0;
    while (first != last) {
        ++first;
        ++ans;
    }
    return ans;
}

template<typename Iter>
void advance_forward(Iter &it, size_t delta) {
    while (delta != 0) {
        ++it;
        --delta;
    }
}

template<typename Iter>
Iter copy(Iter first, Iter last, Iter out) {
    while (first != last) {
        *out = *first;
        ++out;
        ++first;
    }
    return out;
}

template<typename Iter>
Iter merge(Iter a_first, Iter a_last,
           Iter b_first, Iter b_last,
           Iter ans_first) {
    for (; a_first != a_last; ++ans_first) {
        if (b_first == b_last) {
            return copy(a_first, a_last, ans_first);
        }
        if (*b_first < *a_first) {
            *ans_first = *b_first;
            ++b_first;
        } else {
            *ans_first = *a_first;
            ++a_first;
        }
    }
    return copy(b_first, b_last, ans_first);
}

template<typename Iter, typename LList = typename Iter::list_type>
void _mergesort(Iter first, Iter last, LList& buf) {
    size_t dist = distance(first, last);
    if (dist > 1) {
        auto middle = first;
        advance_forward(middle, dist / 2);

        _mergesort(first, middle, buf);
        _mergesort(middle, last, buf);
        Iter It = merge(first, middle, middle, last, buf.begin());
        copy(buf.begin(), It, first);
    }
}

template<typename Iter, typename LList = typename Iter::list_type>
void mergesort(Iter first, Iter last) {
    LList buf(distance(first, last));
    _mergesort(first, last, buf);
}


int main() {
    size_t n = 0;
    cin >> n;
    List<int> l;
    for (size_t i = 0; i < n; ++i) {
        int elem = 0;
        cin >> elem;
        l.push_front(elem);
    }
    mergesort(l.begin(), l.end());
    for (auto elem : l) {
        cout << elem << " ";
    }
}
