lint:
	cpplint --recursive --exclude=build --exclude=scrap --exclude=submodules --exclude=tests/fixtures --filter=-legal/copyright,-readability/casting,-build/include_what_you_use .
