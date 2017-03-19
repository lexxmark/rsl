#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <ref_unique_ptr.h>

SUITE(ref_unique_ptr_tests)
{
	using namespace rsl;

	TEST(basic_test)
	{
		auto object = make_unique_ref<int>(10);

		auto ptr = get_ref(object);
		CHECK(ptr);
		CHECK_EQUAL(10, *object);

		*ptr = 13;
		CHECK_EQUAL(13, *object);
		CHECK_EQUAL(13, *ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(multi_ref_ptrs_test)
	{
		auto object = make_unique_ref<int>(10);

		auto ptr1 = std::make_unique<ref_ptr<int>>(get_ref(object));
		auto ptr2 = std::make_unique<ref_ptr<int>>(get_ref(object));
		auto ptr3 = std::make_unique<ref_ptr<int>>(get_ref(object));
		auto ptr4 = std::make_unique<ref_ptr<int>>(get_ref(object));

		ptr4.reset();
		ptr2.reset();
		ptr4.reset();
		ptr1.reset();
	}

	TEST(array_test)
	{
		auto object = make_unique_ref<int[]>(10);

		auto ptr = get_ref(object);
		CHECK(ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(array_get_element_test)
	{
		auto object = make_unique_ref<int[]>(10);
		object[4] = 8;
		CHECK_EQUAL(8, object[4]);

		auto ptr = get_ref(object, 4);
		CHECK(ptr);
		CHECK_EQUAL(8, object[4]);

		*ptr = 13;
		CHECK_EQUAL(13, object[4]);
		CHECK_EQUAL(13, *ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(array_multi_ref_ptrs_test)
	{
		auto object = make_unique_ref<int[]>(10);

		auto ptr1 = get_ref(object);
		auto ptr2 = get_ref(object, 4);
		auto ptr3 = ptr1;
		auto ptr4 = ptr2;

		*ptr4 = 15;
		CHECK_EQUAL(15, object[4]);

		auto p = ptr3.get();
		p[3] = 10;
		CHECK_EQUAL(10, object[3]);
	}
}
