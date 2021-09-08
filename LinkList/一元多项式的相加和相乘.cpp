#include "LinkList.h"
using namespace My;
using std::istream;
class Monomial {
public:
	int coe, idx;
	Monomial() :coe(0), idx(0) {}
	Monomial(int coe, int idx=0) :coe(coe), idx(idx) {}
	Monomial operator + (const Monomial &mono) const{
		if (idx != mono.idx)
			throw "Indexes do not match";
		return Monomial(coe + mono.coe, idx);
	}
	Monomial operator * (const Monomial &mono) const{
		return Monomial(coe * mono.coe, idx + mono.idx);
	}
	Monomial &operator += (const Monomial &mono) {
		if (idx != mono.idx)
			throw "Indexes do not match";
		coe += mono.coe;
		return *this;
	}
	Monomial &operator *= (const Monomial &mono){
		coe *= mono.coe;
		idx += mono.idx;
		return *this;
	}
	Monomial operator - () const {
		return Monomial(-coe, idx);
	}
	bool operator ! () const {
		return coe == 0;
	}
	bool operator < (const Monomial &mono) const{
		return idx < mono.idx || idx == mono.idx && coe < mono.coe;
	}
	bool operator == (const Monomial &mono) const{
		return idx == mono.idx;
	}
};
istream &operator >> (istream &in, Monomial &mono) {
	in >> mono.coe >> mono.idx;
	return in;
}
ostream &operator << (ostream &out, const Monomial &mono) {
	out << mono.coe << ' ' << mono.idx;
	return out;
}
class Polynomial {
public:
	SingleLinkList<Monomial> data;
	Polynomial() :data() {}
	Polynomial(const SingleLinkList<Monomial> &lnk) :data(lnk) {}
	Polynomial(SingleLinkList<Monomial> &&lnk) :data(std::move(lnk)) {}
	Polynomial(const Polynomial &poly):data(poly.data) {}
	Polynomial(Polynomial &&poly) noexcept :data(std::move(poly.data)){}
	Polynomial &operator = (const Polynomial &poly) {
		data = poly.data;
		return *this;
	}
	Polynomial &operator = (Polynomial &&poly) noexcept{
		data = std::move(poly.data);
		return *this;
	}
	Polynomial &append(const Monomial &mono) {
		data.append(mono);
		return *this;
	}
	Polynomial &insert(const Monomial &mono) {
		auto i = data.head;
		for (int index = 0; index < data.length() && mono < i->value; i = i->next, ++index);
		if (mono == i->value)
			i->value += mono;
		else
			data.insert(i, mono);
		return *this;
	}
	Polynomial &sort() {
		data.sort();
		return *this;
	}
	Polynomial &merge() {
		int index = 0;
		Pointer<Node<Monomial>> pre = nullptr;
		int length = data.length();
		for (auto i = data.head; index < length;++index) {
			for (; i->next && i->value == i->next->value; ++index) {
				i->value += i->next->value;
				data.removeAt(i);
			}
			if (!i->value) {
				i = i->next;
				data.removeAt(pre);
			}
			else {
				pre = i;
				i = i->next;
			}
		}
		return *this;
	}
	Polynomial &standardize() {
		sort();
		return merge();
	}
	Polynomial operator + (const Polynomial &poly) const {
		Polynomial result(*this);
		result.data.concat(SingleLinkList<Monomial>(poly.data));
		return std::move(result.standardize());
	}
	Polynomial operator * (const Polynomial &poly) const {
		Polynomial result;
		int index = 0;
		for (auto i = data.head; index < data.length(); i = i->next, ++index) {
			int jndex = 0;
			for (auto j = poly.data.head; jndex < poly.data.length(); j = j->next, ++jndex)
				result.append(i->value * j->value);
		}
		return result.standardize();
	}
	Polynomial operator - () const {
		auto result = *this;
		int index = 0;
		for (auto i = result.data.head; index < result.data.length(); i = i->next, ++index)
			i->value.coe = -(i->value.coe);
		return std::move(result);
	}
};
istream &operator >> (istream &in, Polynomial &poly) {
	int n;
	in >> n;
	Monomial value;
	for (int i = 0; i < n; i++) {
		in >> value;
		poly.append(value);
	}
	poly.standardize();
	return in;
}
ostream &operator << (ostream &out, const Polynomial &poly) {
	out << poly.data;
	return out;
}
int main() {
	Polynomial a, b;
	cin >> a >> b;
	int opt;
	cin >> opt;
	switch (opt) {
	case 0:
		cout << a + b;
		break;
	case 1:
		cout << a * b;
		break;
	case 2:
		cout << a + b << endl << a * b;
		break;
	}
	cout << endl;
	return 0;
}