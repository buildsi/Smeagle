.PHONY: all test standalone docs

all:
	cmake --log-level=VERBOSE -DsymtabAPI_DIR=/opt/view/lib/cmake/Dyninst -S all -B build
	cmake --build build


test:
	cmake -S test -B build/test
	cmake --build build/test
	CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test


standalone:
	cmake -S standalone -B build/standalone
	cmake --build build/standalone

docs:
	cmake -S documentation -B build/doc
	cmake --build build/doc --target GenerateDocs
