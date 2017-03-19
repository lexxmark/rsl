#include <UnitTest++/UnitTest++.h>
#include <ref_array.h>
#include <ref_vector.h>
#include <ref_unordered_set.h>

struct ComplexObject
{
	int i = 0;
	std::string s = "test";
};

SUITE(ref_container_tests)
{
	using namespace rsl;
	
	TEST(get_ref_array_test)
	{
		array_ref<int, 3> a;
		a.fill(3);

		auto ptr = get_ref(a, 0);
		CHECK_EQUAL(3, *ptr);
		*ptr = 5;
		CHECK_EQUAL(5, a[0]);
	}

	TEST(get_cref_array_test)
	{
		array_ref<int, 3> a;
		a.fill(3);

		auto ptr = get_ref(a, 2);
		CHECK_EQUAL(3, *ptr);
		a[2] = 5;
		CHECK_EQUAL(5, *ptr);
	}

	TEST(destroy_array_test)
	{
		auto a = std::make_unique<array_ref<int, 3>>();
		a->fill(3);

		auto ptr = get_ref(*a, 2);
		CHECK_EQUAL(3, *ptr);
		a->at(2) = 5;
		CHECK_EQUAL(5, *ptr);

		a.reset();

		CHECK(!ptr);
	}

	TEST(complex_array_test)
	{
		array_ref<ComplexObject, 3> a;

		auto ptr = get_ref(a, 2);
		CHECK_EQUAL(0, ptr->i);
		CHECK_EQUAL("test", ptr->s);

		auto ptr2 = make_ref(&ptr->i, ptr);
		CHECK_EQUAL(0, *ptr2);

		ptr->i = 2;
		CHECK_EQUAL(2, a[2].i);
		CHECK_EQUAL(2, *ptr2);

		*ptr2 = 4;
		CHECK_EQUAL(4, a[2].i);
		CHECK_EQUAL(4, ptr->i);
	}

	TEST(get_ref_vector_test)
	{
		vector_ref<int> v(3, 0);

		auto ptr = get_ref(v, 0);
		CHECK_EQUAL(0, *ptr);
		*ptr = 3;
		CHECK_EQUAL(3, v[0]);
	}

	TEST(get_cref_vector_test)
	{
		vector_ref<int> v(3, 0);

		auto ptr = get_cref(v, 0);
		CHECK_EQUAL(0, *ptr);
		v[0] = 3;
		CHECK_EQUAL(3, *ptr);
	}

	TEST(get_reg_vector_clear_test)
	{
		vector_ref<int> v(3, 0);

		auto ptr = get_ref(v, 0);
		CHECK_EQUAL(0, *ptr);

		v.clear();

		CHECK(!ptr);
	}

	TEST(get_reg_vector_erase_test)
	{
		vector_ref<int> v(3, 0);

		auto ptr0 = get_ref(v, 0);
		CHECK_EQUAL(0, *ptr0);

		auto ptr2 = get_ref(v, 2);
		CHECK_EQUAL(0, *ptr2);

		v.erase(v.begin() + 2);

		CHECK(ptr0);
		CHECK(!ptr2);
	}

	TEST(unordered_set_test)
	{
		unordered_set_ref<int> set;

		set.insert(3);

		auto ptr = get_ref(set, 3);
		CHECK(ptr);
		CHECK_EQUAL(3, *ptr);

		set.erase(3);
		CHECK(set.empty());
		CHECK(!ptr);
	}
}