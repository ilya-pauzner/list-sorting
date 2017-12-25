#include<iostream>

using std::cin;
using std::cout;

template<typename T>
class List {
private:
    class Node {
    public:
        T value_;
        Node *right_;

        explicit Node(const T &v = T()) {
            value_ = v;
            right_ = nullptr;
        }

        bool operator==(const Node &other) const {
            return value_ == other.value_ && right_ == other.right_;
        }
    };

public:
    class Iterator {
    private:
        Node *cur_;

    public:
        typedef T value_type;

        explicit Iterator(Node *curr) : cur_(curr) {}

        T &operator*() {
            return cur_->value_;
        }

        Iterator &operator++() {
            cur_ = cur_->right_;
            return *this;
        }

        bool operator==(const Iterator &other) const {
            return (*cur_) == *(other.cur_);
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }
    };

    // [first_; last_)
    Node *first_;
    Node *last_;
    int size_;

    explicit List(size_t size = 0) {
        first_ = new Node;
        last_ = first_;
        size_ = 0;
        while (size) {
            push_front(0);
            --size;
        }
    }

    ~List() {
        Node *cur = first_;
        while (cur != last_) {
            auto tmp = cur;
            cur = cur->right_;
            delete tmp;
        }
        delete last_;
    }

    void push_front(const T &value) {
        auto *new_first = new Node(value);
        new_first->right_ = first_;
        first_ = new_first;
        ++size_;
    }

    Iterator begin() {
        return Iterator(first_);
    }

    Iterator end() {
        return Iterator(last_);
    }

    int size() const {
        return size_;
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
void advance(Iter &it, int delta) {
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

template<typename Iter, typename T = typename Iter::value_type>
void mergesort(Iter first, Iter last) {
    if (distance(first, last) > 1) {
        int m = distance(first, last) / 2;
        List<T> buf(distance(first, last));
        auto middle = first;
        advance(middle, m);
        mergesort(first, middle);
        mergesort(middle, last);
        merge(first, middle, middle, last, buf.begin());
        copy(buf.begin(), buf.end(), first);
    }
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

