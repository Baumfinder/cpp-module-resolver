main:
	g++ -fmodules-ts -std=c++20 -o cpp-module-resolver main.cpp

clean:
	rm -rf gcm.cache
	rm -f a.out