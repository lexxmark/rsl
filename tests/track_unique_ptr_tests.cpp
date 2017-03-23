#include <UnitTest++/UnitTest++.h>
#include <track/unique_ptr.h>

SUITE(track_unique_ptr_tests)
{
	using namespace rsl::track;

	TEST(basic_test)
	{
		auto object = make_unique<int>(10);

		auto ptr = get_ptr(object);
		CHECK(ptr);
		CHECK_EQUAL(10, *object);

		*ptr = 13;
		CHECK_EQUAL(13, *object);
		CHECK_EQUAL(13, *ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(multi_track_ptrs_test)
	{
		auto object = make_unique<int>(10);

		auto ptr1 = std::make_unique<pointer<int>>(get_ptr(object));
		auto ptr2 = std::make_unique<pointer<int>>(get_ptr(object));
		auto ptr3 = std::make_unique<pointer<int>>(get_ptr(object));
		auto ptr4 = std::make_unique<pointer<int>>(get_ptr(object));

		ptr4.reset();
		ptr2.reset();
		ptr4.reset();
		ptr1.reset();
	}

	TEST(array_test)
	{
		auto object = make_unique<int[]>(10);

		auto ptr = get_ptr(object);
		CHECK(ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(array_get_element_test)
	{
		auto object = make_unique<int[]>(10);
		object[4] = 8;
		CHECK_EQUAL(8, object[4]);

		auto ptr = get_ptr(object, 4);
		CHECK(ptr);
		CHECK_EQUAL(8, object[4]);

		*ptr = 13;
		CHECK_EQUAL(13, object[4]);
		CHECK_EQUAL(13, *ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(array_multi_track_ptrs_test)
	{
		auto object = make_unique<int[]>(10);

		auto ptr1 = get_ptr(object);
		auto ptr2 = get_ptr(object, 4);
		auto ptr3 = ptr1;
		auto ptr4 = ptr2;

		*ptr4 = 15;
		CHECK_EQUAL(15, object[4]);

		auto p = ptr3.get();
		p[3] = 10;
		CHECK_EQUAL(10, object[3]);
	}
}
