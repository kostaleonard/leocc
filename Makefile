lint:
	cpplint --recursive --exclude=build --exclude=scrap --exclude=submodules --filter=-legal/copyright,-readability/casting,-build/include_what_you_use .
