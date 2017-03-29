#include <UnitTest++/UnitTest++.h>

#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>

#include <track/array.h>
#include <track/vector.h>
#include <track/deque.h>
#include <track/list.h>
#include <track/forward_list.h>
#include <track/set.h>
#include <track/map.h>
#include <track/unordered_set.h>
#include <track/unordered_map.h>

struct ComplexObject
{
	int i = 0;
	std::string s = "test";
};

SUITE(array_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(index_array_test)
	{
		array<int, 3> a;
		a.fill(3);

		auto ptr = get_ptr(a, 0);
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);

		auto cptr = cget_ptr(a, 2);
		a[2] = 7;
		CHECK_EQUAL(7, *cptr);
	}

	TEST(iterator_array_test)
	{
		array<std::string, 3> a;
		a.fill("hello");

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL("hello", *ptr);
		*ptr = "world";
		CHECK_EQUAL("world", a[0]);

		auto cptr = cget_ptr(a, a.cbegin() + 2);
		a[2] = "!";
		CHECK_EQUAL("!", *cptr);
	}

	TEST(delete_array_test)
	{
		auto a = std::make_unique<array<std::vector<int>, 3>>();

		auto ptr1 = get_ptr(*a, 0);
		auto ptr2 = cget_ptr(*a, 1);
		auto ptr3 = get_ptr(*a, a->begin());
		auto ptr4 = cget_ptr(*a, a->cbegin()+2);

		CHECK(ptr1);
		CHECK(ptr2);
		CHECK(ptr3);
		CHECK(ptr4);

		a.reset();

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
		CHECK(!ptr4);
	}

	TEST(complex_object_array_test)
	{
		pointer<ComplexObject> ptr1;
		pointer<int> ptr2;
		pointer<std::string> ptr3;

		{
			array<ComplexObject, 3> a;

			ptr1 = get_ptr(a, 1);
			ptr2 = make_ptr(&ptr1->i, ptr1);
			ptr3 = make_ptr(&ptr1->s, ptr2);

			CHECK(ptr1);
			CHECK(ptr2);
			CHECK(ptr3);

			ptr1->i = 100;
			ptr1->s = "hello";
			CHECK_EQUAL(100, *ptr2);
			CHECK_EQUAL("hello", *ptr3);

			*ptr2 = 50;
			CHECK_EQUAL(50, ptr1->i);

			*ptr3 = "world";
			CHECK_EQUAL("world", ptr1->s);
		}

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
	}
}

SUITE(vector_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(index_vector_test)
	{
		vector<int> a{3, 3, 3};

		auto ptr = get_ptr(a, 0);
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);

		auto cptr = cget_ptr(a, 2);
		a[2] = 7;
		CHECK_EQUAL(7, *cptr);
	}

	TEST(iterator_vector_test)
	{
		vector<std::string> a;
		a.assign(3, "hello");

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL("hello", *ptr);
		*ptr = "world";
		CHECK_EQUAL("world", a[0]);

		auto cptr = cget_ptr(a, a.cbegin() + 2);
		a[2] = "!";
		CHECK_EQUAL("!", *cptr);
	}

	TEST(delete_vector_test)
	{
		auto a = std::make_unique<vector<std::vector<int>>>();
		a->resize(3);

		auto ptr1 = get_ptr(*a, 0);
		auto ptr2 = cget_ptr(*a, 1);
		auto ptr3 = get_ptr(*a, a->begin());
		auto ptr4 = cget_ptr(*a, a->cbegin() + 2);

		CHECK(ptr1);
		CHECK(ptr2);
		CHECK(ptr3);
		CHECK(ptr4);

		a.reset();

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
		CHECK(!ptr4);
	}

	TEST(clear_vector_test)
	{
		vector<char> a;
		a.resize(3);

		auto ptr1 = get_ptr(a, 0);
		auto ptr2 = cget_ptr(a, 1);
		auto ptr3 = get_ptr(a, a.begin());
		auto ptr4 = cget_ptr(a, a.cbegin() + 2);

		CHECK(ptr1);
		CHECK(ptr2);
		CHECK(ptr3);
		CHECK(ptr4);

		a.clear();

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
		CHECK(!ptr4);
	}

	TEST(erase_vector_test)
	{
		vector<std::pair<int, std::string>> a;
		a.resize(3);

		auto ptr0 = cget_ptr(a, a.cbegin() + 0);
		auto ptr1 = cget_ptr(a, a.cbegin() + 1);
		auto ptr2 = cget_ptr(a, a.cbegin() + 2);

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(ptr2);

		a.erase(a.cbegin() + 1);

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(!ptr2);

		a.erase(a.cbegin() + 1);

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);

		a.erase(a.cbegin());

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(insert_vector_test)
	{
		vector<int> a;
		a.reserve(6);
		a.push_back(0);
		a.push_back(0);
		a.push_back(0);

		CHECK_EQUAL(6, a.capacity());

		auto ptr0 = cget_ptr(a, a.cbegin() + 0);
		auto ptr1 = cget_ptr(a, a.cbegin() + 1);
		auto ptr2 = cget_ptr(a, a.cbegin() + 2);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin(), 2);

		CHECK_EQUAL(2, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin(), -1);

		CHECK_EQUAL(-1, *ptr0);
		CHECK_EQUAL(2, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin() + 1, 5);

		CHECK_EQUAL(-1, *ptr0);
		CHECK_EQUAL(5, *ptr1);
		CHECK_EQUAL(2, *ptr2);

		a.insert(a.begin(), 8);

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(complex_object_vector_test)
	{
		pointer<ComplexObject> ptr1;
		pointer<int> ptr2;
		pointer<std::string> ptr3;

		{
			vector<ComplexObject> a;
			a.resize(3);

			ptr1 = get_ptr(a, 1);
			ptr2 = make_ptr(&ptr1->i, ptr1);
			ptr3 = make_ptr(&ptr1->s, ptr2);

			CHECK(ptr1);
			CHECK(ptr2);
			CHECK(ptr3);

			ptr1->i = 100;
			ptr1->s = "hello";
			CHECK_EQUAL(100, *ptr2);
			CHECK_EQUAL("hello", *ptr3);

			*ptr2 = 50;
			CHECK_EQUAL(50, ptr1->i);

			*ptr3 = "world";
			CHECK_EQUAL("world", ptr1->s);
		}

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
	}
}

SUITE(ref_container_tests)
{
	using namespace rsl::track;
	
	TEST(get_ptr_array_test)
	{
		array<int, 3> a;
		a.fill(3);

		auto ptr = get_ptr(a, 0);
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);
	}

	TEST(cget_ptr_array_test)
	{
		array<int, 3> a;
		a.fill(3);

		auto ptr = cget_ptr(a, 2);
		CHECK_EQUAL(3, *ptr);
		a[2] = 5;
		CHECK_EQUAL(5, *ptr);
	}

	TEST(array_iterator_test)
	{
		array<int, 3> a;
		a.fill(3);

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);
	}

	TEST(array_const_iterator_test)
	{
		array<int, 3> a;
		a.fill(3);

		auto ptr = cget_ptr(a, a.cbegin()+1);
		CHECK_EQUAL(3, *ptr);
		a[1] = 5;
		CHECK_EQUAL(5, *ptr);
	}

	TEST(destroy_array_test)
	{
		auto a = std::make_unique<array<int, 3>>();
		a->fill(3);

		auto ptr = get_ptr(*a, 2);
		CHECK_EQUAL(3, *ptr);
		a->at(2) = 5;
		CHECK_EQUAL(5, *ptr);

		a.reset();

		CHECK(!ptr);
	}

	TEST(complex_array_test)
	{
		array<ComplexObject, 3> a;

		auto ptr = get_ptr(a, 2);
		CHECK_EQUAL(0, ptr->i);
		CHECK_EQUAL("test", ptr->s);

		auto ptr2 = make_ptr(&ptr->i, ptr);
		CHECK_EQUAL(0, *ptr2);

		ptr->i = 2;
		CHECK_EQUAL(2, a[2].i);
		CHECK_EQUAL(2, *ptr2);

		*ptr2 = 4;
		CHECK_EQUAL(4, a[2].i);
		CHECK_EQUAL(4, ptr->i);
	}

	TEST(get_ptr_vector_test)
	{
		vector<int> v(3, 0);

		auto ptr = get_ptr(v, 0);
		CHECK_EQUAL(0, *ptr);
		*ptr = 3;
		CHECK_EQUAL(3, v[0]);
	}

	TEST(cget_ptr_vector_test)
	{
		vector<int> v(3, 0);

		auto ptr = cget_ptr(v, 0);
		CHECK_EQUAL(0, *ptr);
		v[0] = 3;
		CHECK_EQUAL(3, *ptr);
	}

	TEST(get_ptr_vector_clear_test)
	{
		vector<int> v(3, 0);

		auto ptr = get_ptr(v, 0);
		CHECK_EQUAL(0, *ptr);

		v.clear();

		CHECK(!ptr);
	}

	TEST(get_ptr_vector_erase_test)
	{
		vector<int> v(3, 0);

		auto ptr0 = get_ptr(v, 0);
		CHECK_EQUAL(0, *ptr0);

		auto ptr2 = get_ptr(v, 2);
		CHECK_EQUAL(0, *ptr2);

		v.erase(v.begin() + 2);

		CHECK(ptr0);
		CHECK(!ptr2);

		v.shrink_to_fit();

		CHECK(!ptr0);
	}

	TEST(vector_iterator_test)
	{
		vector<int> v(3, 0);

		auto ptr0 = get_ptr(v, v.begin());
		CHECK_EQUAL(0, *ptr0);

		auto ptr2 = get_ptr(v, v.begin() + 2);
		CHECK_EQUAL(0, *ptr2);

		v.erase(v.begin() + 2);

		CHECK(ptr0);
		CHECK(!ptr2);

		v.shrink_to_fit();

		CHECK(!ptr0);
	}

	TEST(unordered_set_get_ptr_test)
	{
		unordered_set<int> set;

		set.insert(3);

		auto ptr = get_ptr(set, set.find(3));
		CHECK(ptr);
		CHECK_EQUAL(3, *ptr);

		set.erase(3);
		CHECK(set.empty());
		CHECK(!ptr);
	}

	TEST(unordered_set_find_ptr_test)
	{
		unordered_set<int> set;

		set.insert(3);

		auto ptr = find_ptr(set, 3);
		CHECK(ptr);
		CHECK_EQUAL(3, *ptr);

		set.erase(3);
		CHECK(set.empty());
		CHECK(!ptr);
	}

	TEST(unordered_map_get_ptr_test)
	{
		unordered_map<int, char> map;

		map[2] = '3';

		auto ptr = get_ptr(map, map.find(2));
		CHECK(ptr);
		CHECK_EQUAL('3', ptr->second);

		map.erase(2);
		CHECK(map.empty());
		CHECK(!ptr);
	}

	TEST(unordered_map_find_ptr_test)
	{
		unordered_map<int, char> map;

		map[2] = '3';

		auto ptr = find_ptr(map, 2);
		CHECK(ptr);
		CHECK_EQUAL('3', ptr->second);

		map.erase(2);
		CHECK(map.empty());
		CHECK(!ptr);
	}
}
