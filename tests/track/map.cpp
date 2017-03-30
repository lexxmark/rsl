#include <UnitTest++/UnitTest++.h>

#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>

#include <track/map.h>
#include <vector>

struct ComplexObject
{
	int i = 0;
	std::string s = "test";
};

SUITE(map_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(find_map_test)
	{
		map<int, int> a{ {0, 1}, {1, 2}, {2, 3} };

		auto ptr = find_ptr(a, 0);
		CHECK_EQUAL(1, ptr->second);
		ptr->second = 5;
		CHECK_EQUAL(5, a[0]);

		auto cptr = cfind_ptr(a, 2);
		a[2] = 7;
		CHECK_EQUAL(7, cptr->second);
	}

	TEST(iterator_map_test)
	{
		map<std::string, int> a{ {"a", 1}, {"b", 2}, {"c", 3} };

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL("a", ptr->first);
		ptr->second = 4;
		CHECK_EQUAL(4, a["a"]);

		auto cptr = cget_ptr(a, ++a.cbegin());
		a["b"] = 8;
		CHECK_EQUAL(8, cptr->second);
	}

	TEST(delete_map_test)
	{
		auto a = std::make_unique<map<int, char>>();
		*a = { { 0, 'a' },{ 1, 'b' },{ 2, 'c' } };

		auto ptr1 = find_ptr(*a, 0);
		auto ptr2 = cfind_ptr(*a, 1);
		auto ptr3 = get_ptr(*a, a->begin());
		auto ptr4 = cget_ptr(*a, ++(++a->cbegin()));

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

	TEST(clear_map_test)
	{
		map<char, float> a{ {'a', 3.f}, {'b', 5.f}, {'c', 0.2f} };

		auto ptr1 = find_ptr(a, 'a');
		auto ptr2 = cfind_ptr(a, 'b');
		auto ptr3 = get_ptr(a, ++a.begin());
		auto ptr4 = cget_ptr(a, ++(++a.cbegin()));

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

	TEST(erase_map_test)
	{
		map<int, std::pair<int, std::string>> a{ {0, {1, "hello"}}, {1, {2, "world"}}, {2, {3, "!"}} };

		auto ptr0 = cget_ptr(a, a.cbegin());
		auto ptr1 = cget_ptr(a, ++a.cbegin());
		auto ptr2 = cget_ptr(a, a.find(2));

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(ptr2);

		a.erase(++a.cbegin());

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(ptr2);

		a.erase(a.find(2));

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);

		a.erase(a.cbegin());

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(insert_map_test)
	{
		map<int, int> a{ {0, 1}, {2, 3}, {4, 5} };

		auto ptr0 = cget_ptr(a, a.cbegin());
		auto ptr1 = cget_ptr(a, ++a.cbegin());
		auto ptr2 = cget_ptr(a, ++(++a.cbegin()));

		CHECK_EQUAL(1, ptr0->second);
		CHECK_EQUAL(3, ptr1->second);
		CHECK_EQUAL(5, ptr2->second);

		a.insert({ 3, 2 });

		CHECK_EQUAL(1, ptr0->second);
		CHECK_EQUAL(3, ptr1->second);
		CHECK_EQUAL(5, ptr2->second);

		a.insert({ -1 , 1});

		CHECK_EQUAL(1, ptr0->second);
		CHECK_EQUAL(3, ptr1->second);
		CHECK_EQUAL(5, ptr2->second);
	}

	TEST(complex_object_map_test)
	{
		pointer<std::pair<const int, ComplexObject>> ptr1;
		pointer<int> ptr2;
		pointer<std::string> ptr3;

		{
			map<int, ComplexObject> a{ {0, ComplexObject()}, {2, ComplexObject()}, {4, ComplexObject()} };

			ptr1 = find_ptr(a, 2);
			ptr2 = make_ptr(&ptr1->second.i, ptr1);
			ptr3 = make_ptr(&ptr1->second.s, ptr2);

			CHECK(ptr1);
			CHECK(ptr2);
			CHECK(ptr3);

			ptr1->second.i = 100;
			ptr1->second.s = "hello";
			CHECK_EQUAL(100, *ptr2);
			CHECK_EQUAL("hello", *ptr3);

			*ptr2 = 50;
			CHECK_EQUAL(50, ptr1->second.i);

			*ptr3 = "world";
			CHECK_EQUAL("world", ptr1->second.s);
		}

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
	}
}
