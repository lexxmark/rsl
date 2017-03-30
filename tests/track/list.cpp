#include <UnitTest++/UnitTest++.h>

#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>

#include <track/list.h>
#include <vector>

struct ComplexObject
{
	int i = 0;
	std::string s = "test";
};

SUITE(list_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(iterator_list_test)
	{
		list<std::string> a;
		a.assign(3, "hello");

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL("hello", *ptr);
		*ptr = "world";
		CHECK_EQUAL("world", a.front());

		auto it = a.begin();
		std::advance(it, 2);
		auto cptr = cget_ptr(a, it);
		*it = "!";
		CHECK_EQUAL("!", *cptr);
	}

	TEST(delete_list_test)
	{
		auto a = std::make_unique<list<std::vector<int>>>();
		a->resize(3);

		auto ptr1 = get_ptr(*a, a->begin());
		auto ptr2 = cget_ptr(*a, ++(++a->cbegin()));

		CHECK(ptr1);
		CHECK(ptr2);

		a.reset();

		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(clear_list_test)
	{
		list<char> a;
		a.resize(3);

		auto ptr1 = get_ptr(a, a.begin());
		auto ptr2 = cget_ptr(a, ++(++a.cbegin()));

		CHECK(ptr1);
		CHECK(ptr2);

		a.clear();

		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(erase_list_test)
	{
		list<std::pair<int, std::string>> a;
		a.resize(3);

		auto ptr0 = cget_ptr(a, a.cbegin());
		auto ptr1 = cget_ptr(a, ++a.cbegin());
		auto ptr2 = cget_ptr(a, ++(++a.cbegin()));

		CHECK(ptr0);
		CHECK(ptr1);
		CHECK(ptr2);

		a.erase(++a.cbegin());

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(ptr2);

		a.erase(++a.cbegin());

		CHECK(ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);

		a.erase(a.begin());

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(insert_list_test)
	{
		list<int> a{ 0, 0, 0 };

		auto ptr0 = cget_ptr(a, a.cbegin());
		auto ptr1 = cget_ptr(a, ++a.cbegin());
		auto ptr2 = cget_ptr(a, ++(++a.cbegin()));

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(a.begin(), 2);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(++a.begin(), -1);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);

		a.insert(++(++a.begin()), 5);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(0, *ptr1);
		CHECK_EQUAL(0, *ptr2);
	}

	TEST(complex_object_list_test)
	{
		pointer<ComplexObject> ptr1;
		pointer<int> ptr2;
		pointer<std::string> ptr3;

		{
			list<ComplexObject> a;
			a.resize(3);

			ptr1 = get_ptr(a, a.begin());
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
