#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iterator>

using std::size_t;

namespace sjtu
{
	
	template <class T>
	class Matrix
	{
	    //声明友元函数
        template<class T1,class U1>
	    friend  auto operator*(const Matrix<T1> &mat, const U1 &x);
        template<class U1,class T1>
	    friend  auto operator*(const U1 &x, const Matrix<T1> &mat);
        template<class  U1,class V1>
	    friend auto operator*(const Matrix<U1> &a, const Matrix<V1> &b);
        template<class  U1,class V1>
	    friend auto operator+(const Matrix<U1> &a, const Matrix<V1> &b);
        template<class  U1,class V1>
	    friend auto operator-(const Matrix<U1> &a, const Matrix<V1> &b);
	private:
		// your private member variables here.
		std::size_t Row;
		size_t  Column;

		T* matrix=NULL;
		
	public:
		Matrix() = default;
		
		Matrix(size_t n, size_t m, T _init = T())//可加上鲁棒性
		{
        Row=n,Column=m;
        matrix =new T[n*m];
            for (int i = 0; i <n*m ; ++i)
            {
                matrix[i]=_init;
            }
		}
		explicit Matrix(std::pair<size_t, size_t> sz, T _init = T())
		{
		Row=sz.first,Column=sz.second;
		matrix =new T[Row*Column];
		for (int i = 0; i <Row*Column ; ++i) {
                matrix[i]=_init;
            }
		}
		//构造出一个matrix
		Matrix(const Matrix &o)
		{
		Row=o.Row;
		Column=o.Column;
		matrix=new T[Row*Column];
		for (int i = 0; i <Row*Column ; ++i)
		{
		    matrix[i]=o.matrix[i];
		}
		}
		//拷贝构造
		template <class U>
		Matrix(const Matrix<U> &o)
		{
            Row=o.Row;
            Column=o.Column;
            matrix=new T[Row*Column];
            for (int i = 0; i <Row*Column ; ++i)
            {
                matrix[i]=o.matrix[i];
            }
		}
		
		Matrix &operator=(const Matrix &o)
		{
		if (this==&o)
            return  *this;
            delete [] matrix;
            Row=o.Row;
            Column=o.Column;
            matrix=new T[Row*Column];
            for (int i = 0; i <Row*Column ; ++i)
            {
                matrix[i]=o.matrix[i];
            }
		}
		
		template <class U>
		Matrix &operator=(const Matrix<U> &o)
		{
            if (this==&o)
                return  *this;
            delete [] matrix;
            Row=o.Row;
            Column=o.Column;
            matrix=new T[Row*Column];
            for (int i = 0; i <Row*Column ; ++i)
            {
                matrix[i]=o.matrix[i];
            }
		}
		
		Matrix(Matrix &&o) noexcept
		{
            if (&o== this) return *this;
            delete [] matrix;
            Row=o.Row;
            Column=o.Column;
            matrix=o.matrix;
            o.matrix=NULL;
            return *this;
		}
		
		Matrix &operator=(Matrix &&o) noexcept
		{
            if (&o== this) return *this;
            delete [] matrix;
            Row=o.Row;
            Column=o.Column;
            matrix=o.matrix;
            o.matrix=NULL;
            return *this;
		}
		
        ~Matrix() {if (matrix) delete [] matrix;}
		//initializer
		Matrix(std::initializer_list<std::initializer_list<T>> il)
		{
			Row=il.size();
			Column=(il.begin())->size();
			matrix=new T[Row*Column];
			int k=-1;
            for (auto i = il.begin(); i !=il.end() ; ++i) {
                if(Column!=i->size()) throw std::invalid_argument("inintializer occur error!");
                for (auto j = i->begin(); j !=i->end() ; ++j) {
                    k++;
                    matrix[i]=*j;
                }
            }
		}
		//其他的功能
	public:
		size_t rowLength() const {
            return Row;
		}
		
		size_t columnLength() const {
            return  Column;
		}
		
		void resize(size_t _n, size_t _m, T _init = T())
		{T* temp=new T[_n*_m];
		if (_m*_n<Column*Row){
            for (int i = 0; i <_m*_n ; ++i) {
                temp[i]=matrix[i];
            }
		} else if (_m*_n==Column*Row)
        {
            return;
        }
		else {
            for (int i = 0; i <_m*_n ; ++i) {
                if (i<Column*Row) temp[i]=matrix[i];
                else temp[i]=_init;

            }

		}
            Row=_n,Column=_m;

		}
		
		void resize(std::pair<size_t, size_t> sz, T _init = T())
		{int _n,_m;
		    sz.first=_n,sz.second=_m;
            T* temp=new T[_n*_m];
            if (_m*_n<Column*Row){
                for (int i = 0; i <_m*_n ; ++i)
                {
                    temp[i]=matrix[i];
                }
            } else if (_m*_n==Column*Row)
            {
                return;
            }
            else {
                for (int i = 0; i <_m*_n ; ++i) {
                    if (i<Column*Row) temp[i]=matrix[i];
                    else temp[i]=_init;

                }

            }
            Row=_n,Column=_m;
		}
		
		std::pair<size_t, size_t> size() const
		{
            return std::make_pair(Row,Column);
		};
		
		void clear()
		{
            delete [] matrix;
            matrix=NULL;
            Row=Column=0;
		}
	//元素获取
	public:
		const T &operator()(size_t i, size_t j) const
		{
		if (i>Row||j>Column) 	 throw std::invalid_argument("()operator occur error!");
        else {
            return matrix[(i-1)*Column+j-1];
        }
		}
		
		T &operator()(size_t i, size_t j)
		{
            if (i>Row||j>Column) 	 throw std::invalid_argument("()operator occur error!");
            else {
                return matrix[(i)*Column+j];
            }
		}
		
		Matrix<T> row(size_t i) const
		{if (i<0||i>Row)  throw std::invalid_argument("()operator row occur error!");
         Matrix <T> temp;
         temp.matrix=new T[Column];
         temp.Column=Column;
         temp.Row=1;
            for (int j = 0; j <Column; ++j)
            {
              temp.matrix[j]=matrix[i*Column+j];
            }
		}
		
		Matrix<T> column(size_t i) const
		{if (i<0||i>Column)  throw std::invalid_argument("()operator column occur error!");
            Matrix <T> temp;
            temp.matrix=new T[Row];
            temp.Column=1;
            temp.Row=Row;
            for (int j = 0; j <Row; ++j)
            {
                temp.matrix[j]=matrix[j*Column+i];
            }
		}
		
		
	public:
		template <class U>
		bool operator==(const Matrix<U> &o) const
		{bool flag=true;
            for (int i = 0; i <Row ; ++i) {
                for (int j = 0; j <Column ; ++j) {
                   if(matrix[i*Column+j]==o.matrix[i*Column+j]) flag= false;
                }
            }
            return flag;
		}
		
		template <class U>
		bool operator!=(const Matrix<U> &o) const
		{
			
		}
		
		Matrix operator-() const
		{
			
		}
		
		template <class U>
		Matrix &operator+=(const Matrix<U> &o)
		{
			
		}
		
		template <class U>
		Matrix &operator-=(const Matrix<U> &o)
		{
			
		}
		
		template <class U>
		Matrix &operator*=(const U &x)
		{
			
		}
		
		Matrix tran() const
		{
			
		}
		
	public: // iterator
		class iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type        = T;
			using pointer           = T *;
			using reference         = T &;
			using size_type         = size_t;
			using difference_type   = std::ptrdiff_t;
			
			iterator() = default;
			
			iterator(const iterator &) = default;
			
			iterator &operator=(const iterator &) = default;
			
		private:

			
		public:
			difference_type operator-(const iterator &o)
			{
				
			}
			
			iterator &operator+=(difference_type offset)
			{
				
			}
			
			iterator operator+(difference_type offset) const
			{
				
			}
			
			iterator &operator-=(difference_type offset)
			{
				
			}
			
			iterator operator-(difference_type offset) const
			{
				
			}
			
			iterator &operator++()
			{
				
			}
			
			iterator operator++(int)
			{
				
			}
			
			iterator &operator--()
			{
				
			}
			
			iterator operator--(int)
			{
				
			}
			
			reference operator*() const
			{
				
			}
			
			pointer operator->() const
			{
				
			}
			
			bool operator==(const iterator &o) const
			{
				
			}
			
			bool operator!=(const iterator &o) const
			{
				
			}
		};
		
		iterator begin()
		{
			
		}
		
		iterator end()
		{
			
		}
		
		std::pair<iterator, iterator> subMatrix(std::pair<size_t, size_t> l, std::pair<size_t, size_t> r)
		{
			
        }
	};
		
}

//
namespace sjtu
{
	template <class T, class U>
	auto operator*(const Matrix<T> &mat, const U &x)//todo
	{
		
	}
	
	template <class T, class U>
	auto operator*(const U &x, const Matrix<T> &mat)
	{
		
	}
	
	template <class U, class V>
	auto operator*(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
	template <class U, class V>
	auto operator+(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
	template <class U, class V>
	auto operator-(const Matrix<U> &a, const Matrix<V> &b)
	{
		
	}
	
}

#endif //SJTU_MATRIX_HPP

