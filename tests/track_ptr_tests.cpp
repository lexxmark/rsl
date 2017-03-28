#include <UnitTest++/UnitTest++.h>
#include <track/pointer.h>

SUITE(track_ptr_tests)
{
	using namespace rsl::track;

	TEST(basic_test)
	{
		trackable object;

		{
			auto p = make_ptr(&object);
			CHECK(p);
			CHECK_EQUAL(4*sizeof(void*), sizeof(p));
		}
	}

	TEST(null_track_ptr_test)
	{
		{
			pointer<char> p;
			CHECK(!p);
		}

		{
			pointer<int> ptr = nullptr;
			CHECK(!ptr);
		}
	}

	TEST(null_make_track_ptr_test)
	{
		{
			auto ptr = make_ptr<int>(nullptr);
			CHECK(!ptr);
		}

		{
			trackable object;
			auto ptr = make_ptr<int>(nullptr, object);
			CHECK(!ptr);
		}

		{
			trackable* object = nullptr;
			auto ptr = make_ptr(object);
			CHECK(!ptr);
		}
	}

	TEST(check_track_ptr_after_delete_test)
	{
		auto object = std::make_unique<trackable>();

		auto ptr = make_ptr(object.get());
		CHECK(ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(three_track_ptrs_test)
	{
		trackable object;

		{
			auto ptr1 = make_ptr(&object);
			CHECK(ptr1);
			{
				auto ptr2 = make_ptr(&object);
				CHECK(ptr2);
				{
					auto ptr3 = make_ptr(&object);
					CHECK(ptr3);
				}
				CHECK(ptr2);
			}
			CHECK(ptr1);
		}
	}

	TEST(copy_objects_test)
	{
		trackable object1;

		auto ptr1 = make_ptr(&object1);

		{
			pointer<trackable> ptr2;

			{
				auto object2 = object1;
				ptr2 = make_ptr(&object2, object2);

				CHECK(ptr1);
				CHECK(ptr2);
			}

			CHECK(ptr1);
			CHECK(!ptr2);
		}

		CHECK(ptr1);
	}

	TEST(sub_object_test)
	{
		struct Object : public trackable
		{
			int i = 0;
		};

		Object object;

		auto ptr1 = make_ptr(&object);
		auto ptr2 = make_ptr(&object.i, object);

		CHECK_EQUAL(0, ptr1->i);
		CHECK_EQUAL(0, *ptr2);

		ptr1->i = 1;
		CHECK_EQUAL(1, object.i);
		CHECK_EQUAL(1, *ptr2);

		*ptr2 = 2;
		CHECK_EQUAL(2, object.i);
		CHECK_EQUAL(2, ptr1->i);
	}
}
