namespace ft {

	template <class T1, class T2>
	struct pair {
		public:
			typedef T1 first_type;
			typedef T2 second_type;

			pair();	

			template<class U, class V>
			pair (const pair<U,V>& pr);

			pair (const first_type& a, const second_type& b);

			pair& operator= (const pair& pr);

	
		private:
			first_type first;
			second_type second;
	};
}