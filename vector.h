#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <initializer_list>
#include <cstddef>
#include <stdexcept>

using namespace std;

template <typename T>
class Vector {

	public:

		class ConstIterator;
		class Iterator;
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = Vector::Iterator;
		using const_iterator = Vector::ConstIterator;
	
	private:

		size_type sz;
		size_type max_sz;
		pointer values;
		static constexpr size_type min_sz {5};

	public:

		// Konstruktor mit max_sz und Defaultkonstruktor
		Vector (size_type n = min_sz) {
			if (n < min_sz)
				max_sz = min_sz;
			else
				max_sz = n;
						
			sz = 0;
			values = new value_type [max_sz];
		} 

		//Konstruktor mit Initializer list
		Vector (initializer_list<value_type> il) {
			if (il.size() < min_sz)
				max_sz = min_sz;
			else 	
				max_sz = il.size();
			
			values = new value_type [max_sz];
			sz = il.size();

			size_type a {0};

			for (const auto& elem : il) {
				values [a] = elem;
				a++;
			}
		}

		// Kopierkonstruktor
		Vector (const Vector& v) {
			sz = v.sz;
			max_sz = v.max_sz;
			
			values = new value_type [max_sz];
			for (size_type a {0}; a < v.sz; a++) {
				values[a] = v.values[a];
			}
		}

		//Destruktor
		~Vector () {
			delete [] values;
		}

		iterator begin () {
			return values;
		}

		const_iterator begin () const {
			return values;
		}

		iterator end () {
			return values + sz;
		}

		const_iterator end () const {
			return values + sz;
		}

		iterator insert (const_iterator pos, const_reference val) {
			auto diff = pos-begin();

			size_type current {static_cast<size_type>(diff)};
			
			if (diff < 0 || current > sz)
				throw runtime_error ("Iterator out of bounds");		

			if (sz >= max_sz)
				reserve (max_sz*2+1);

			for (size_type i {sz}; i-- > current;)
				values[i+1] = values[i];
			
			values[current] = val;

			++sz;

			return iterator {values + current};			
		} 

		iterator erase (const_iterator pos) {
			auto diff = pos-begin();

			if (diff < 0 || static_cast<size_type> (diff) >= sz)
				throw runtime_error("Iterator out of bounds");

			size_type current {static_cast<size_type> (diff)};

			for (size_type i {current}; i < sz-1; ++i)
				values[i] = values [i+1];

			--sz;
					
			return iterator {values + current};
		}
	
		size_type size () const {
			return sz;
		}

		bool empty () const {
			if (sz == 0)
				return true;
			else
				return false;
		}

		void clear () {
			sz = 0;
		}

		void reserve (size_type n) {
			if (n > max_sz) {
				max_sz = n;
				value_type* temp = new value_type[max_sz];

				for (size_t a {0}; a < sz; a++)
					temp[a] = values[a];

				delete [] values;
				values = temp;	
			}	
		} 

		void shrink_to_fit () {
			if (sz >= min_sz)
				max_sz = sz;
		}

		void push_back (value_type x) {
			if (sz >= max_sz) 
				reserve (max_sz*2+1);

			values [sz] = x;
			++sz;
		}

		void pop_back () {
			if (this->empty())
				throw runtime_error ("Vektor ist leer.");

			sz--;
		}

		// Kopierzuweisungsoperator
		void operator= (const Vector& b) {
			if (b.size() > size())
				reserve (b.max_sz);

			sz = b.sz;
			max_sz = b.max_sz;
			for (size_type a {0}; a < b.sz; a++) {
				values[a] = b.values[a];
			}
		}

		value_type& operator[] (size_type index) {
			if (index < sz && index >= 0)
				return values[index];
			else 	
				throw runtime_error("Invalid index");
		}

		const value_type& operator[] (size_type index) const {
			if (index < sz && index >= 0)
				return values[index];
			else 	
				throw runtime_error("Invalid index");
		}

		size_type capacity () const {
			return max_sz;
		}
        
        ostream& print (ostream& o) {
            o << '[';

        	bool first {true};

	        for (size_t a {0}; a < sz; a++) {
		        if (first) {
			        o << values[a];
			        first = false;
		        }
		        else {
			        o << ", " << values[a];
		        }
	        }

	        o << ']';

        	return o;
        }

	class Iterator {
	
		public:

			using value_type = Vector::value_type;
			using reference = Vector::reference;
			using pointer = Vector::pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = forward_iterator_tag;
		
		private:
			
			pointer ptr;

		public:

			// Iterator-Konstruktor
			Iterator (pointer p = nullptr) : ptr {p} {}
			
			reference operator* () {
				return *ptr;
			}

			pointer operator-> () {
				return ptr;
			}

			bool operator!= (const const_iterator& other) const {
				return static_cast<ConstIterator> (*this) != other;
			}

			bool operator== (const const_iterator& other) const {
				return static_cast<ConstIterator> (*this) == other;
			}

			iterator& operator++ () {
				++ptr;
 				return *this;
			}

			iterator operator++ (int) {
				iterator temp = *this;
 				++(*this);
  				return temp;
			}

			operator const_iterator () const {
				return ConstIterator{ptr};
			}

		};

	class ConstIterator {
	    
		public:

			using value_type = Vector::value_type;
			using reference = Vector::const_reference;
			using pointer = Vector::const_pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = forward_iterator_tag;

		private:
				
			pointer ptr;

		public:
					
			ConstIterator (pointer p = nullptr) : ptr {p} {}

			Vector::difference_type operator-(const ConstIterator& other) const { 
            	return ptr-other.ptr;
        	}

			bool operator== (const const_iterator& other) const {
				return ptr == other.ptr;
			}

			bool operator!= (const const_iterator& other) const {
				return ptr != other.ptr;
			}			

			reference operator* () const {
				return *ptr;
			}

			pointer operator-> () const {
				return ptr;
			}

			const_iterator& operator++ ()  {
				++ptr;
				return *this;
			}

			const_iterator operator++ (int) {
				const_iterator temp = *this;
 				++(*this);
  				return temp;
			}

	};

};

template <typename T>
ostream& operator<< (ostream& o, Vector<T> v) {
	return v.print(o);
}

#endif
