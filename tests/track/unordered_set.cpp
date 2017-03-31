#include <UnitTest++/UnitTest++.h>

#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>

#include <track/unordered_set.h>

struct ComplexObject
{
	ComplexObject(int i_, std::string s_)
		: i(i_),
		s(s_)
	{}

	bool operator==(const ComplexObject& other) const
	{
		return (i == other.i) && (s == other.s);
	}

	int i = 0;
	std::string s = "test";
};

struct ComplexObjectHash
{
	size_t operator()(const ComplexObject& key) const
	{
		return std::hash<int>()(key.i);
	}
};


SUITE(unordered_set_tests)
{
	using namespace rsl;
	using namespace rsl::track;

	TEST(find_unordered_set_test)
	{
		unordered_set<int> a{ 0, 1, 2 };

		auto ptr = find_ptr(a, 0);
		CHECK_EQUAL(0, *ptr);

		auto cptr = cfind_ptr(a, 2);
		CHECK_EQUAL(2, *cptr);
	}

	TEST(iterator_unordered_set_test)
	{
		unordered_set<std::string> a{ "a", "b", "c" };

		auto ptr = get_ptr(a, a.begin());
		CHECK_EQUAL(*a.begin(), *ptr);

		auto cptr = cget_ptr(a, ++a.cbegin());
		CHECK_EQUAL(*(++a.cbegin()), *cptr);
	}

	TEST(delete_unordered_set_test)
	{
		auto a = std::make_unique<unordered_set<int>>();
		*a = { 0, 1, 2 };

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

	TEST(clear_unordered_set_test)
	{
		unordered_set<char> a{ 'a', 'b', 'c' };

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

	TEST(erase_unordered_set_test)
	{
		unordered_set<std::string> a{ "a", "b", "c"  };

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

		a.erase(a.cbegin());

		CHECK(!ptr0);
		CHECK(!ptr1);
		CHECK(!ptr2);
	}

	TEST(insert_unordered_set_test)
	{
		unordered_set<int> a{ 0, 2, 4 };

		auto ptr0 = cfind_ptr(a, 0);
		auto ptr1 = cfind_ptr(a, 2);
		auto ptr2 = cfind_ptr(a, 4);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(2, *ptr1);
		CHECK_EQUAL(4, *ptr2);

		a.insert(3);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(2, *ptr1);
		CHECK_EQUAL(4, *ptr2);

		a.insert(-1);

		CHECK_EQUAL(0, *ptr0);
		CHECK_EQUAL(2, *ptr1);
		CHECK_EQUAL(4, *ptr2);
	}

	TEST(complex_object_unordered_set_test)
	{
		pointer<const ComplexObject> ptr1;
		pointer<const int> ptr2;
		pointer<const std::string> ptr3;

		{
			unordered_set<ComplexObject, ComplexObjectHash> a{ { 0, "hello" },{ 1, "world" },{ 2, "!" } };

			ptr1 = find_ptr(a, ComplexObject(1, "world"));
			ptr2 = make_ptr(&ptr1->i, ptr1);
			ptr3 = make_ptr(&ptr1->s, ptr2);

			CHECK(ptr1);
			CHECK(ptr2);
			CHECK(ptr3);

			CHECK_EQUAL(1, *ptr2);
			CHECK_EQUAL("world", *ptr3);
		}

		CHECK(!ptr1);
		CHECK(!ptr2);
		CHECK(!ptr3);
	}
}

