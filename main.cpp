#include <iostream>
#include <vector>

class zip_iterator final {
private:
    std::vector<int>::const_iterator iter1_;
    std::vector<int>::const_iterator iter2_;
public:
    zip_iterator(std::vector<int>::const_iterator iter1, std::vector<int>::const_iterator iter2) :
    iter1_(iter1), iter2_(iter2) {}
    std::pair<int, int> operator*() const { return {*iter1_, *iter2_}; }
    bool operator==(const zip_iterator& zi) const {
        return zi.iter1_ == iter1_ && zi.iter2_ == iter2_;
    }
    zip_iterator& operator++() {
        ++iter1_; ++iter2_;
        return *this;
    }
};

class zip final {
private:
    const std::vector<int>& v1_;
    const std::vector<int>& v2_;
public:
    zip(const std::vector<int>& v1, const std::vector<int>& v2) : v1_(v1), v2_(v2) {}
    [[nodiscard]]
    auto begin() const { return zip_iterator(std::begin(v1_), std::begin(v2_)); }
    [[nodiscard]]
    auto end() const { return zip_iterator(std::end(v1_), std::end(v2_)); }
};

////

class iota final {
private:
    int start_{};
    std::size_t end_{};
public:
    using iterator = int*;
    using iterator_sz_t = std::size_t*;
    //using const_iterator = const std::size_t*;
    explicit iota(const int start = 0, const std::size_t end = 0) : start_(start), end_(end) {}
    [[nodiscard]]
    iterator begin() { return &start_; }
    [[nodiscard]]
    iterator_sz_t end() { return &end_; }
};

class zip_iota_iterator final {
private:
    iota::iterator iter1_{};
    iota::iterator_sz_t iter1_sz_{};
    std::vector<int>::const_iterator iter2_{};
public:
    zip_iota_iterator(iota::iterator iter1, std::vector<int>::const_iterator iter2) :
    iter1_(iter1), iter2_(iter2) {}
    // can make zip_iota_iterator template on the iota::iterator to get rid of this
    // dummy iter1_sz_
    zip_iota_iterator(iota::iterator_sz_t iter1, std::vector<int>::const_iterator iter2) :
    iter1_sz_(iter1), iter2_(iter2) {}
    std::pair<int, int> operator*() const { return {*iter1_, *iter2_}; }
    bool operator==(const zip_iota_iterator& zi) const {
        return zi.iter2_ == iter2_;
    }
    zip_iota_iterator& operator++() {
        ++(*iter1_); ++iter2_;
        return *this;
    }
};

class zip_iota final {
private:
    iota iota_;
    const std::vector<int>& v1_;
public:
    zip_iota(const iota i, const std::vector<int>& v1) : iota_(0, v1.size()), v1_(v1) {}
    [[nodiscard]]
    auto begin() { return zip_iota_iterator(std::begin(iota_), std::begin(v1_)); }
    [[nodiscard]]
    auto end() { return zip_iota_iterator(std::end(iota_), std::end(v1_) ); }
};

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::vector<int> v1 {1, 2, 3, 4, 5};
    std::vector<int> v2 {6, 7, 8, 9, 10};

    std::clog << "range for loop\n";
    for(const auto& i : v1) {
        std::clog << "i is " << i << '\n';
    }

    std::clog << "with idx\n";
    for(std::size_t i = 0; i < v1.size(); ++i) {
        std::clog << i << " is " << v1[i] << '\n';
    }

    // how do you use v2[i] ??
    std::clog << "with distance\n";
    for(/*const*/ auto& i : v1) {
        std::clog << "i is " << i << '\n';
        // one option, clunky
        // needed to remove const from the const auto&
        const auto idx = std::distance(&v1[0], &i);
        std::clog << "index is "  << idx << '\n';

        // I can not use v2[]
        std::clog << "v2: " << v2[idx] << '\n';
    }

    // entering zip_iterator land
    std::clog << "zip iterator\n";
    for (const auto [item1, item2] : zip(v1, v2)) {
        std::clog << "got " << item1 << ',' << item2 << '\n';
    }

    // also, can we do it with iota?
    std::clog << "iota iterator\n";
    for (const auto [idx, item] : zip_iota(iota{}, v1)) {
        std::clog << "got " << idx << ',' << item << '\n';
    }

    return 0;
}
