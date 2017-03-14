#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <ref_ptr.h>

SUITE(ref_ptr_tests)
{
	using namespace rsl;

	TEST(basic_test)
	{
		lifetime_trackable object;

		{
			auto ptr = make_ref(&object);
			CHECK(ptr);
		}
	}

	TEST(null_ref_ptr_test)
	{
		{
			ref_ptr<char> ptr;
			CHECK(!ptr);
		}

		{
			ref_ptr<int> ptr = nullptr;
			CHECK(!ptr);
		}
	}

	TEST(null_make_ref_test)
	{
		{
			auto ptr = make_ref<int>(nullptr);
			CHECK(!ptr);
		}

		{
			lifetime_trackable object;
			auto ptr = make_ref<int>(nullptr, object);
			CHECK(!ptr);
		}

		{
			lifetime_trackable* object = nullptr;
			auto ptr = make_ref(object);
			CHECK(!ptr);
		}
	}

	TEST(check_ref_ptr_after_delete_test)
	{
		auto object = std::make_unique<lifetime_trackable>();

		auto ptr = make_ref(object.get());
		CHECK(ptr);

		object.reset();
		CHECK(!ptr);
	}

	TEST(three_ref_ptrs_test)
	{
		lifetime_trackable object;

		{
			auto ptr1 = make_ref(&object);
			CHECK(ptr1);
			{
				auto ptr2 = make_ref(&object);
				CHECK(ptr2);
				{
					auto ptr3 = make_ref(&object);
					CHECK(ptr3);
				}
				CHECK(ptr2);
			}
			CHECK(ptr1);
		}
	}

	TEST(copy_objects_test)
	{
		lifetime_trackable object1;

		auto ptr1 = make_ref(&object1);

		{
			ref_ptr<lifetime_trackable> ptr2;

			{
				auto object2 = object1;
				ptr2 = make_ref(&object2, object2);

				CHECK(ptr1);
				CHECK(ptr2);
			}

			CHECK(ptr1);
			CHECK(!ptr2);
		}

		CHECK(ptr1);
	}
}

int main(int, const char *[])
{
	return UnitTest::RunAllTests();
}